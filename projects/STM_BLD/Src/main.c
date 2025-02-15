#include "../Inc/main.h"
#include "../Inc/memory.h"
#include "../../Common/Inc/vartype_convert.h"
#include "../../Common/Inc/checksum.h"
#include "../Inc/memory_map.h"
#include "../../../Common/Inc/util.h"
#include "../Inc/hal_i2c.h"

/*============================CONSTANTS====================================*/
#define JUMP_TO_APP 0
#define USE_CUSTOM_HAL_I2C 1

bld_cmd_e bld_cmd = CMD_NOP;

uint8_t gRxData[NUM_BYTE_MAX] = {};
uint8_t gTxData[TXDATA_SIZE] = {};

union
{
  uint8_t byte[256];
  uint32_t word[64];
} data;
uint8_t line_ptr;
uint8_t data_ptr;
uint8_t line_len;

uint32_t main_address;
uint32_t sector_start_adr;
bool is_rewrite_sector = false;
bool start_timeout = false;
uint32_t uploading_timer = 0;

uint8_t state = READ_BYTECOUNT;
uint32_t address;

uint8_t counter = 0;
hal_i2c_hdl_t *i2chandle;
hal_i2c_cfg_t cfg;
hal_i2c_instance_e i2c0 = HAL_I2C0;

/*============================FUNCTION VARIABLES====================================*/

// I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart1;

/*============================DECLARE FUNCTIONS====================================*/
void process_hex_file(void);
void erase_memory(uint32_t adr, uint32_t size);
static void I2C_fill_data(uint8_t *u8_data);

/*============================FUNCTIONS====================================*/

/**
 * @brief read and upload the data to flash
 * @note The data will be devided into 6 parts, using switch case:
 * @note data length, data address, data type, flashing data, checksum, check
 crc
 * because each interrupt only sent 1 byte,
 * counter "line_ptr" as well as all used variables are defined globally
 * and will be reset at the end of successful read if needed
 */
void process_hex_file(void)
{
  if (state == START_WRITE)
  {
    /* Send response: currently processing */
    gTxData[0] = RES_UPLOAD_INPROG;
  }
  else
  {
    // compute check sum
    bool checksum = check_cs(&data.byte[0], data.byte[line_len - 1], line_len - 1);
    if (checksum)
    {
      uint8_t record_type = data.byte[3];
      uint32_t address = main_address | (data.byte[1] << 8) | (data.byte[2]);

      if (record_type == RT_MAIN_ADDR)
      { // If RT is main address
        main_address = (data.byte[4] << 24) | (data.byte[5] << 16);
        gTxData[0] = RES_UPLOAD_SUCCESS;
      }
      else if (record_type == RT_DATA)
      {
        // padding left
        uint32_t *two_words;
        uint8_t prepad_len;
        prepad_len = address & 0x07;
        if (prepad_len > 0)
        {
          // increase length
          data.byte[0] += prepad_len;
          // memory move
          for (int i = (data.byte[0] + 4); i >= (4 + prepad_len); i--)
          {
            data.byte[i] = data.byte[i - prepad_len];
          }
          for (int i = 0; i < prepad_len; i++)
          {
            data.byte[4 + i] = 0xff;
          }
          address -= prepad_len;
        }
        // padding right double word
        uint8_t num_double_words = (data.byte[0] + 7) >> 3;
        if ((num_double_words * 8) > data.byte[0])
        {
          uint8_t *p_start = &data.byte[4] + data.byte[0];
          uint8_t *p_end = &data.byte[4] + num_double_words * 8;
          while (p_start < p_end)
          {
            *p_start++ = 0xff;
          }
        }

        /* Write 1 word at a time*/
        uint8_t num_word = (data.byte[0] + 3) >> 2;
        uint8_t m = 0;
        while (m < num_word)
        {
          // little endian format
          two_words = &data.word[m + 1];
          // Only condition that dont want to be written to flash is
          // when process in rewrite sector mode when CRC fail triggered
          // and when the address is outside of the sector
          if (!(is_rewrite_sector && (address < sector_start_adr)))
          {
            HAL_FLASH_Unlock();
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, *two_words);
            CLEAR_BIT(FLASH->CR, (FLASH_CR_PG));
            HAL_FLASH_Lock();
          }
          address += 0x04;

          // Check if write the sector complete
          if (is_rewrite_sector &&
              (address >= (sector_start_adr + SECTOR_SIZE)))
          {
            // Move to next stage
            break; // break out of write loop
          }
          m++;
        }
        // response
        gTxData[0] = RES_UPLOAD_SUCCESS;
      }
      else if (record_type == RT_EOF)
      {
        // Send line read/write complete
        gTxData[0] = RES_UPLOAD_SUCCESS;
      }
    }
    else
    {
      /* If fail checksum, ask for resend line */
      gTxData[0] = RES_UPLOAD_FAIL;
    }
  }
}

static interrupt_rq_e I2C_select_command(uint8_t *u8_data)
{
  interrupt_rq_e i2c_cmd = IRQ_CMD_NOP;
  switch (u8_data[0])
  {
  case RQ_ERASE:
  {
    i2c_cmd = IRQ_CMD_ERASE;
  }
  break;

  case RQ_CHECK_BLANKING:
  {
    i2c_cmd = IRQ_CMD_CHECK_BLANKING;
  }
  break;

  case RQ_CHECK_CRC:
  {
    i2c_cmd = IRQ_CMD_CHECK_CRC;
  }
  break;

  case RQ_UPLOADING:
  {
    i2c_cmd = IRQ_CMD_UPLOADING;
  }
  break;

  case RQ_SYSTEM_RESET:
  {
    i2c_cmd = IRQ_CMD_SYSTEM_RESET;
  }
  break;

  case RQ_GET_BLD_VER:
  {
    i2c_cmd = IRQ_CMD_GET_BLD_VER;
  }
  break;

  case RQ_ENTER_BLD:
  {
    i2c_cmd = IRQ_CMD_ENTER_BLD;
  }
  break;

  case RQ_EXIT_BLD:
  {
    i2c_cmd = IRQ_CMD_EXIT_BLD;
  }
  break;

  default:
    break;
  }
  return i2c_cmd;
}

static bld_cmd_e I2C_fill_data(uint8_t *u8_data)
{
  interrupt_rq_e irq_cmd = I2C_select_command(u8_data);

  line_ptr = 0;
  line_len = 0;
  uploading_timer = 0;

  switch (irq_cmd)
  {
  case IRQ_CMD_UPLOADING:
  {
    while (true)
    {
      data.byte[line_ptr] = u8_data[1 + line_ptr];
      line_ptr++;
      if (line_ptr == 1)
      {
        line_len = 5 + data.byte[0];
      }
      else if (line_len == line_ptr)
      {
        break;
      }
    }
    bld_cmd = CMD_UPLOADING;
    state = WRITE_DATA;
    start_timeout = true;
    line_ptr = 0;
    irq_cmd = IRQ_CMD_NOP;
  }
  break;

  case IRQ_CMD_CHECK_CRC:
  {
    // 4 byte start + 4 len + 4 bytes 32 bits crc + 1 byte cs
    while (line_ptr != 13)
    {
      data.byte[line_ptr] = u8_data[1 + line_ptr];
      line_ptr++;
    }
    bld_cmd = CMD_CHECK_CRC;
    line_ptr = 0;
    start_timeout = true;
    irq_cmd = IRQ_CMD_NOP;
  }
  break;

  case IRQ_CMD_CHECK_BLANKING:
  {
    // 4 bytes for address and 4 bytes for size, 1 byte cs
    while (line_ptr != 9)
    {
      data.byte[line_ptr] = u8_data[1 + line_ptr];
      line_ptr++;
    }
    bld_cmd = CMD_BLANKING;
    line_ptr = 0;
    start_timeout = true;
    irq_cmd = IRQ_CMD_NOP;
  }
  break;

  case IRQ_CMD_ERASE:
  {
    while (line_ptr != 9)
    {
      data.byte[line_ptr] = u8_data[1 + line_ptr];
      line_ptr++;
    }
    // 4 bytes for address and 4 bytes for size, 1 byte cs
    bld_cmd = CMD_ERASE;
    line_ptr = 0;
    start_timeout = true;
    irq_cmd = IRQ_CMD_NOP;
  }
  break;

  case IRQ_CMD_ENTER_BLD:
  {
    // 4 bytes rq key, 1 byte cs
    while (line_ptr != 5)
    {
      data.byte[line_ptr] = u8_data[1 + line_ptr];
      line_ptr++;
    }
    bld_cmd = CMD_ENTER_BLD;
    line_ptr = 0;
    start_timeout = true;
    irq_cmd = IRQ_CMD_NOP;
  }
  break;

  case IRQ_CMD_EXIT_BLD:
  {
    // 4 bytes res key, 1 byte cs
    while (line_ptr != 5)
    {
      data.byte[line_ptr] = u8_data[1 + line_ptr];
      line_ptr++;
    }
    bld_cmd = CMD_EXIT_BLD;
    line_ptr = 0;
    start_timeout = true;
    irq_cmd = IRQ_CMD_NOP;
  }
  break;

  case IRQ_CMD_SYSTEM_RESET:
    bld_cmd = CMD_SYSTEM_RESET;
    break;

  case IRQ_CMD_GET_BLD_VER:
    bld_cmd = CMD_GET_BLD_VER;
    break;

  default:
    break;
  }
  return bld_cmd;
}

/*============================MAIN====================================*/

int main(void)
{
  bool enter_app = true;
  bld_cmd_e cmd_temp = CMD_NOP;
  image_info_t *page_info = (image_info_t *)FLASH_IMAGE_INFO_START;

  // Initialize
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  /* Init I2C*/
  cfg.speed = HAL_I2C_100K;
  cfg.slave_addr = 0x08; // Not use
  i2chandle = hal_i2c_open(i2c0, cfg);

  HAL_I2C_EnableListen_IT(i2chandle); // enable I2C istenning
  HAL_I2C_EV_IRQHandler(i2chandle);   // enable I2C interrupt

  // MX_USART1_UART_Init();

  /* Jump to main here*/
#if JUMP_TO_APP
  if (enter_app)
  {
    /* Check BOOT pin to go to main*/
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8))
    {
      if (verify_memory(FLASH_MAIN_APP_ADDR))
      {
        go_to_main_app(FLASH_MAIN_APP_ADDR);
      }
    }
  }
#endif

  while (1)
  {
    switch (cmd_temp)
    {
    case CMD_ERASE:
    case CMD_BLANKING:
    {
      /* Check checksum first*/
      bool checksum = check_cs(&data.byte[0], data.byte[8], 8);
      if (!checksum)
      {
        gTxData[0] = RES_UPLOAD_FAIL;
      }
      else
      {
        uint32_t adr = (data.byte[0] << 24) | (data.byte[1] << 16) |
                       (data.byte[2] << 8) | (data.byte[3]);
        uint32_t len = (data.byte[4] << 24) | (data.byte[5] << 16) |
                       (data.byte[6] << 8) | (data.byte[7]);
        /* If pass checksum*/
        if (cmd_temp == CMD_ERASE)
        {
          /* Erase memory and send response*/
          erase_memory(adr, len);
          gTxData[0] = RES_ERASE_COMPLETE;
        }
        else if (cmd_temp == CMD_BLANKING)
        {
          /* Check blanking and send response*/
          uint32_t *p32 = (uint32_t *)(adr);
          bool is_blanking = true;
          len = len >> 2; // number of words
          for (uint32_t i = 0; i < len; i++)
          {
            if (p32[i] != 0xFFFFFFFF)
            {
              is_blanking = false;
              break;
            }
          }
          if (is_blanking)
          {
            gTxData[0] = RES_APP_FLASH_CLEAN;
          }
          else
          {
            gTxData[0] = RES_APP_FLASH_DIRTY;
          }
        }
      }
      start_timeout = false;
    }
    break;

    case CMD_UPLOADING:
    {
      process_hex_file();
      start_timeout = false;
    }
    break;

    case CMD_CHECK_CRC:
    {
      /* Check checksum first*/
      bool checksum = check_cs(&data.byte[0], data.byte[12], 12);
      if (!checksum)
      {
        gTxData[0] = RES_UPLOAD_FAIL;
      }
      else
      {
        uint32_t adr = (data.byte[0] << 24) | (data.byte[1] << 16) |
                       (data.byte[2] << 8) | (data.byte[3]);
        uint32_t len = (data.byte[4] << 24) | (data.byte[5] << 16) |
                       (data.byte[6] << 8) | (data.byte[7]);
        uint32_t crc_rx = (data.byte[8] << 24) | (data.byte[9] << 16) |
                          (data.byte[10] << 8) | (data.byte[11]);

        sector_start_adr = adr;
        uint32_t crc_calc;
        /* Calculate crc and compare it with ref crc*/
        crc_calc = CalcCrc32(CRC32_INIT_VAL, len, (uint8_t *)adr);
        if (crc_rx == crc_calc)
        {
          gTxData[0] = RES_CRC_SUCCESS;
          is_rewrite_sector = false;
        }
        else
        {
          gTxData[0] = RES_CRC_FAIL;
          is_rewrite_sector = true;
        }
        start_timeout = false;
      }
    }
    break;

    case CMD_SYSTEM_RESET:
    {
      NVIC_SystemReset();
    }
    break;

    case CMD_GET_BLD_VER:
    {
      const uint32_t bld_version = BLD_VERSION;
      for (int i = 0; i < 4; i++)
      {
        gTxData[0] = (bld_version >> (i * 8)) & 0xFF;
      }
    }
    break;

    case CMD_ENTER_BLD:
    case CMD_EXIT_BLD:
    {
      /* Check checksum first*/
      bool checksum = check_cs(&data.byte[0], data.byte[4], 4);
      if (!checksum)
      {
        gTxData[0] = RES_UPLOAD_FAIL;
      }
      else
      {
        uint32_t write_key = (data.byte[0] << 24) | (data.byte[1] << 16) |
                             (data.byte[2] << 8) | (data.byte[3]);
        /* In case of send data is not match with ref keys
         * Break out of loop and send fail response
         */
        if (cmd_temp == CMD_ENTER_BLD)
        {
          if (write_key != RQ_KEY)
          {
            gTxData[0] = RES_ENTER_BLD_FAIL;
            break;
          }
        }
        else if (cmd_temp == CMD_EXIT_BLD)
        {
          if (write_key != RES_KEY)
          {
            gTxData[0] = RES_EXIT_BLD_FAIL;
            break;
          }
        }

        /* If key are identified*/
        uint32_t write_adr = BLANK_KEY;
        bool is_in_bld = false;
        bool is_exit_bld_valid = true;

        for (int i = 0; i < BLD_ENTER_NUM_KEY; i++)
        {
          /* In case a request key slot is found clean*/
          if (page_info->bld_enter_key[i].rq_key == BLANK_KEY)
          {
            if (cmd_temp == CMD_ENTER_BLD)
            {
              /* If trying to enter, write the key to the blank spot*/
              write_adr = (uint32_t)&page_info->bld_enter_key[i].rq_key;
            }
            else
            {
              /* If trying to exit, with out proper enter, exit is not permitted*/
              is_exit_bld_valid = false;
            }
            break;
            /* In case a request key is found matched but response key
             * is empty
             */
          }
          else if (page_info->bld_enter_key[i].rq_key == RQ_KEY &&
                   page_info->bld_enter_key[i].res_key == BLANK_KEY)
          {
            /* If trying to enter, response already in*/
            if (cmd_temp == CMD_ENTER_BLD)
            {
              is_in_bld = true;
              /* If trying to exit, write the exit response to the blank spot*/
            }
            else
            {
              is_exit_bld_valid = true;
              write_adr = (uint32_t)&page_info->bld_enter_key[i].rq_key;
            }
            break;
          }
        }

        bool allow_write = false;
        bld_enter_rq_res_t key;
        key.rq_key = BLANK_KEY;
        key.res_key = BLANK_KEY;
        /*
         * By default, write_adr need to be defined above to
         * Enter or Exit BLD.
         * Also, proper enter or exit flag needs to be checked
         */
        if (cmd_temp == CMD_ENTER_BLD)
        {
          if (!is_in_bld && (write_adr != BLANK_KEY))
          {
            allow_write = true;
            key.rq_key = RQ_KEY;
            gTxData[0] = RES_ENTER_BLD_SUCCESS;
          }
          else if (is_in_bld)
          {
            gTxData[0] = RES_ENTER_BLD_SUCCESS;
          }
          else
          {
            gTxData[0] = RES_ENTER_BLD_FAIL;
          }
        }
        else if (cmd_temp == CMD_EXIT_BLD)
        {
          if (is_exit_bld_valid && (write_adr != BLANK_KEY))
          {
            allow_write = true;
            key.res_key = RES_KEY;
            gTxData[0] = RES_EXIT_BLD_SUCCESS;
          }
          else
          {
            gTxData[0] = RES_EXIT_BLD_FAIL;
          }
        }

        /* Write corresponding key to the blank spot*/
        if (allow_write)
        {
          HAL_FLASH_Unlock();
          HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, write_adr, (uint32_t *)&key);
          CLEAR_BIT(FLASH->CR, (FLASH_CR_PG));
          HAL_FLASH_Lock();
        }
      }
    }
    break;

    default:
      break;
    };

    cmd_temp = CMD_NOP;
    if (bld_cmd != CMD_NOP)
    {
      cmd_temp = bld_cmd;
      bld_cmd = CMD_NOP;
    }

    /* Set timeout*/
    if (start_timeout)
    {
      uploading_timer++;
    }
    else
    {
      uploading_timer = 0;
    }

    /* UPLOADING TIMEOUT: Reset flags in case of uploading not success*/
    if ((uploading_timer >= UPLOADING_TIMEOUT) && start_timeout)
    {
      uploading_timer = 0;
      state = START_WRITE;
    }
  }
}

/* -------------------i2c interrupt -------------------------------------------*/
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
  bld_cmd = I2C_fill_data(&gRxData);
  hi2c->pBuffPtr = gTxData;
  HAL_I2C_EnableListen_IT(i2chandle);
  gTxData[0] = 0x11;
}

/**
 * @brief Send/Receive data
 * @note 243: erase command
 * @note 242: upload command
 */
volatile uint32_t cnt = 0;
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
  /*Master requests data*/
  if (TransferDirection == I2C_DIRECTION_RECEIVE)
  {
    HAL_I2C_Slave_Seq_Transmit_IT(i2chandle, gTxData, TXDATA_SIZE, I2C_FIRST_AND_LAST_FRAME);
  }
  /*Master sends data*/
  else if (TransferDirection == I2C_DIRECTION_TRANSMIT)
  {
    memset(gRxData, 0, NUM_BYTE_MAX);
    HAL_I2C_Slave_Seq_Receive_IT(i2chandle, (uint8_t *)gRxData, NUM_BYTE_MAX, I2C_FIRST_FRAME); // I2C_FIRST_AND_LAST_FRAME
    counter++;
  }
}
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
  cnt++;
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
