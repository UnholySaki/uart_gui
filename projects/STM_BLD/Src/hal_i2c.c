/**
 ******************************************************************************
 * @file    hal_i2c.c
 * @author  VCD
 * @brief   header file for hal i2c master mode
 * @date    Jan 19 2024
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Austin Circuit Design.
 * All rights reserved.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "hal_i2c.h"
#include "main.h"
#include "hal_gpio.h"
#include "board_config.h"
#include "memory_map.h"
#include "string.h"
/* MACRO   define ------------------------------------------------------------*/
#define I2C_WAIT_TIMEOUT 2000

/* Module variable define ------------------------------------------------------------*/
I2C_HandleTypeDef handle_i2c[HAL_I2C_NUM];

static uint8_t slave_addr[HAL_I2C_NUM];
const I2C_TypeDef *i2c_module[HAL_I2C_NUM] = {I2CI0_MODULE, I2CI1_MODULE};
const uint32_t i2c_timing[HAL_I2C_NUM] = {100000, 400000};
const hal_gpio_t sda_pin[HAL_I2C_NUM] = {
    {.pin = I2CI0_SDA_GPIO_PIN, .port = I2CI0_SDA_GPIO_PORT},
    {.pin = I2CI0_SDA_GPIO_PIN, .port = I2CI0_SDA_GPIO_PORT},
};
const hal_gpio_t scl_pin[HAL_I2C_NUM] = {
    {.pin = I2CI0_SCL_GPIO_PIN, .port = I2CI0_SCL_GPIO_PORT},
    {.pin = I2CI0_SCL_GPIO_PIN, .port = I2CI0_SCL_GPIO_PORT},
};
const uint16_t sda_pin_func[HAL_I2C_NUM] = {I2CI0_SDA_GPIO_FUNC, I2CI0_SDA_GPIO_FUNC};
const uint16_t scl_pin_func[HAL_I2C_NUM] = {I2CI0_SCL_GPIO_FUNC, I2CI0_SCL_GPIO_FUNC};

/* Function prototype ------------------------------------------------------------*/
static void hal_i2c_semaphore_give(hal_i2c_instance_e hali2c);
static bool hal_i2c_semaphore_wait(hal_i2c_instance_e hali2c, uint32_t timeout);
static int32_t hal_i2c_handle_idx(hal_i2c_hdl_t *handle);

/* Functions          ------------------------------------------------------------*/
/**
 * @brief  Initialize hal i2c module
 * @param  None: pointer to daq instance object
 * @retval HAL_I2C_SUCCESS
 */
hal_i2c_stt_e hal_i2c_init(void)
{
  hal_i2c_stt_e stt = HAL_I2C_SUCCESS;
#if I2C_USE_DMA
  memset(handle_i2c, 0, sizeof(handle_i2c));
  memset(hdma_i2c_rx, 0, sizeof(hdma_i2c_rx));
  memset(hdma_i2c_tx, 0, sizeof(hdma_i2c_tx));
#endif
  return stt;
}

/**
 * @brief  Open an i2c port instance
 * @param  idx: i2c module instance
 * @param  cfg: i2c module cofiguration
 * @retval pointer to i2c handle
 */
hal_i2c_hdl_t *hal_i2c_open(hal_i2c_instance_e idx, hal_i2c_cfg_t cfg)
{
  hal_i2c_hdl_t *handle = NULL;

  /*##-1- Configure the I2C peripheral ######################################*/
  handle_i2c[idx].Instance = (I2C_TypeDef *)i2c_module[idx];
  handle_i2c[idx].Init.ClockSpeed = i2c_timing[cfg.speed];
  handle_i2c[idx].Init.DutyCycle = I2C_DUTYCYCLE_2;
  handle_i2c[idx].Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  handle_i2c[idx].Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  handle_i2c[idx].Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  handle_i2c[idx].Init.NoStretchMode = I2C_NOSTRETCH_ENABLED;
  handle_i2c[idx].Init.OwnAddress1 = 16;
  handle_i2c[idx].Init.OwnAddress2 = 0;

  if (HAL_I2C_Init(&handle_i2c[idx]) != HAL_OK)
  {
    /* Initialization Error */
    return handle;
  }
  slave_addr[idx] = cfg.slave_addr;
  handle = &handle_i2c[idx];

  return handle;
}

/**
 * @brief  Set slave address when the slave address needs to be changed
 * @param  handle: pointer to i2c handle return by open function
 * @param  addr: New slave address
 * @retval status  HAL_I2C_FAILED, HAL_I2C_SUCCESS
 */
hal_i2c_stt_e hal_i2c_set_slave_addr(hal_i2c_hdl_t *handle, uint8_t addr)
{
  hal_i2c_stt_e stt = HAL_I2C_FAILED;
  int32_t idx = hal_i2c_handle_idx(handle);
  if (idx >= 0)
  {
    slave_addr[idx] = addr;
    stt = HAL_I2C_SUCCESS;
  }
  return stt;
}

/**
 * @brief  Read data from i2c port
 * @param  handle: pointer to i2c handle return by open function
 * @param  subaddr: Device register address
 * @param  len_addr: Length of device address to send
 * @param  buf: Pointer to input buffer
 * @param  len: Number of byte to read from port
 * @retval status  HAL_I2C_FAILED, HAL_I2C_SUCCESS
 */
hal_i2c_stt_e hal_i2c_read(hal_i2c_hdl_t *handle, uint16_t subaddr, uint16_t len_addr, uint8_t *buf, uint16_t len)
{
  hal_i2c_stt_e stt = HAL_I2C_FAILED;
  HAL_StatusTypeDef hwstt;
  int32_t idx = hal_i2c_handle_idx(handle);
  if (len_addr == 0)
  {
#if I2C_USE_DMA
    hwstt = HAL_I2C_Master_Receive_DMA((I2C_HandleTypeDef *)handle, (uint16_t)slave_addr[idx], (uint8_t *)buf, len);
#else
    hwstt = HAL_I2C_Master_Receive_IT((I2C_HandleTypeDef *)handle, (uint16_t)slave_addr[idx], (uint8_t *)buf, len);
#endif
  }
  else
  {
#if I2C_USE_DMA
    hwstt = HAL_I2C_Mem_Read_DMA((I2C_HandleTypeDef *)handle, (uint16_t)slave_addr[idx], subaddr,
                                 len_addr, buf, len);
#else
    hwstt = HAL_I2C_Mem_Read_IT((I2C_HandleTypeDef *)handle, (uint16_t)slave_addr[idx], subaddr,
                                len_addr, buf, len);
#endif
  }
  if (hwstt == HAL_OK)
  {

    stt = HAL_I2C_SUCCESS;
  }
  return stt;
}
/**
 * @brief  write data too i2c port
 * @param  handle: pointer to i2c handle return by open function
 * @param  subaddr: Device register address
 * @param  len_addr: Length of device address to send
 * @param  buf: Pointer to out buffer
 * @param  len: Number of byte to write to port
 * @retval status  HAL_I2C_FAILED, HAL_I2C_SUCCESS
 */
hal_i2c_stt_e hal_i2c_write(hal_i2c_hdl_t *handle, uint16_t subaddr, uint16_t len_addr, uint8_t *buf, uint16_t len)
{
  hal_i2c_stt_e stt = HAL_I2C_FAILED;
  HAL_StatusTypeDef hwstt;
  int32_t idx = hal_i2c_handle_idx(handle);
  if (len_addr == 0)
  {
#if I2C_USE_DMA
    hwstt = HAL_I2C_Master_Transmit_DMA((I2C_HandleTypeDef *)handle, (uint16_t)slave_addr[idx], (uint8_t *)buf, len);
#else
    hwstt = HAL_I2C_Master_Transmit_IT((I2C_HandleTypeDef *)handle, (uint16_t)slave_addr[idx], (uint8_t *)buf, len);
#endif
  }
  else
  {
#if I2C_USE_DMA
    hwstt = HAL_I2C_Mem_Write_DMA((I2C_HandleTypeDef *)handle, (uint16_t)slave_addr[idx], subaddr,
                                  len_addr, buf, len);
#else
    hwstt = HAL_I2C_Mem_Write_IT((I2C_HandleTypeDef *)handle, (uint16_t)slave_addr[idx], subaddr,
                                 len_addr, buf, len);
#endif
  }
  if (hwstt == HAL_OK)
  {
    stt = HAL_I2C_SUCCESS;
  }
  return stt;
}
/**
 * @brief  Cancel a transfer
 * @param  handle: pointer to i2c handle return by open function
 * @Note   Not support currently
 * @retval status  HAL_I2C_FAILED, HAL_I2C_SUCCESS
 */
hal_i2c_stt_e hal_i2c_cancel(hal_i2c_hdl_t *handle)
{
  hal_i2c_stt_e stt = HAL_I2C_FAILED;
  return stt;
}
/**
 * @brief  Close an i2c port, to let other thread can use
 * @param  handle: pointer to a pointer of i2c handle return by open function
 * @retval status  HAL_I2C_FAILED, HAL_I2C_SUCCESS
 */
hal_i2c_stt_e hal_i2c_close(hal_i2c_hdl_t **handle)
{
  hal_i2c_stt_e stt = HAL_I2C_SUCCESS;
  int32_t idx = hal_i2c_handle_idx(*handle);
  hal_i2c_MspDeInit(*handle);
  *handle = NULL;
  return stt;
}

/**
 * @brief  Wait spi to complete transfer
 * @param  hali2c: poiter to i2c handle
 * @param  timeout: timeout
 * @retval HAL_SPI_SUCCESS
 */
static bool hal_i2c_semaphore_wait(hal_i2c_instance_e hali2c, uint32_t timeout)
{
}
/* Private function  -----------------------------------------------*/
/**
 * @brief  Get handle index
 * @param  handle: poiter to spi handle
 * @retval I2C module index
 */
static int32_t hal_i2c_handle_idx(hal_i2c_hdl_t *handle)
{
  int32_t idx = -1;
  for (int i = 0; i < HAL_I2C_NUM; i++)
  {
    if ((uint32_t)handle == (uint32_t)&handle_i2c[i])
    {
      idx = i;
      break;
    }
  }
  return idx;
}
/**
 * @brief  Give semaphore from tranfer callback
 * @param  halspi: spi instance
 * @retval HAL_SPI_SUCCESS
 */
static void hal_i2c_semaphore_give(hal_i2c_instance_e hali2c)
{
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  int32_t idx = hal_i2c_handle_idx(hi2c);
  if (idx >= 0)
  {
    hal_i2c_semaphore_give(idx);
  }
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  int32_t idx = hal_i2c_handle_idx(hi2c);
  if (idx >= 0)
  {
    hal_i2c_semaphore_give(idx);
  }
}
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  int32_t idx = hal_i2c_handle_idx(hi2c);
  if (idx >= 0)
  {
    hal_i2c_semaphore_give(idx);
  }
}
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  int32_t idx = hal_i2c_handle_idx(hi2c);
  if (idx >= 0)
  {
    hal_i2c_semaphore_give(idx);
  }
}
/**
 * @brief I2C MSP Initialization
 *        This function configures the hardware resources used in this example:
 *           - Peripheral's clock enable
 *           - Peripheral's GPIO Configuration
 *           - DMA configuration for transmission request by peripheral
 *           - NVIC configuration for DMA interrupt request enable
 * @param hi2c: I2C handle pointer
 * @retval None
 */
void hal_i2c_MspInit(I2C_HandleTypeDef *hi2c)
{
  int32_t idx;
  idx = hal_i2c_handle_idx(hi2c);
  if ((idx >= 0) && (idx < HAL_I2C_NUM))
  {
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable I2Cx clock */
    switch (idx)
    {
    case HAL_I2C0:
      I2CI0_CLK_ENABLE();
      break;
    case HAL_I2C1:
      I2CI1_CLK_ENABLE();
      break;
    default:
      break;
    }
    /* Enable DMAx clock */

    hal_gpio_mode_t mode;
    mode.mode = HAL_GPIO_AF_OD;
    mode.pull = HAL_GPIO_PULL_UP;
    mode.func = (hal_gpio_alt_e)sda_pin_func[idx];
    mode.int_cb = NULL;
    hal_gpio_pin_config((hal_gpio_t)sda_pin[idx], mode);
    mode.func = (hal_gpio_alt_e)scl_pin_func[idx];
    hal_gpio_pin_config((hal_gpio_t)scl_pin[idx], mode);

/*##-3- Configure the DMA Channels #########################################*/
/* Configure the DMA handler for Transmission process */
#if I2C_USE_DMA
    DMA2_CLK_ENABLE();
    switch (idx)
    {
    case HAL_I2C0:
      hdma_i2c_tx[idx].Instance = I2CI0_DMA_INSTANCE_TX;
      hdma_i2c_tx[idx].Init.Request = I2CI0_DMA_REQUEST_TX;
      hdma_i2c_rx[idx].Instance = I2CI0_DMA_INSTANCE_RX;
      hdma_i2c_rx[idx].Init.Request = I2CI0_DMA_REQUEST_RX;
      break;
    case HAL_I2C1:
      hdma_i2c_tx[idx].Instance = I2CI1_DMA_INSTANCE_TX;
      hdma_i2c_tx[idx].Init.Request = I2CI1_DMA_REQUEST_TX;
      hdma_i2c_rx[idx].Instance = I2CI1_DMA_INSTANCE_RX;
      hdma_i2c_rx[idx].Init.Request = I2CI1_DMA_REQUEST_RX;
      break;
    case HAL_I2C2:
      hdma_i2c_tx[idx].Instance = I2CI2_DMA_INSTANCE_TX;
      hdma_i2c_tx[idx].Init.Request = I2CI2_DMA_REQUEST_TX;
      hdma_i2c_rx[idx].Instance = I2CI2_DMA_INSTANCE_RX;
      hdma_i2c_rx[idx].Init.Request = I2CI2_DMA_REQUEST_RX;
      break;
    }

    hdma_i2c_tx[idx].Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_i2c_tx[idx].Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c_tx[idx].Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c_tx[idx].Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c_tx[idx].Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c_tx[idx].Init.Mode = DMA_NORMAL;
    hdma_i2c_tx[idx].Init.Priority = DMA_PRIORITY_LOW;

    HAL_DMA_Init(&hdma_i2c_tx[idx]);

    /* Associate the initialized DMA handle to the the I2C handle */
    __HAL_LINKDMA(hi2c, hdmatx, hdma_i2c_tx[idx]);

    /* Configure the DMA handler for Transmission process */

    hdma_i2c_rx[idx].Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_i2c_rx[idx].Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c_rx[idx].Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c_rx[idx].Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c_rx[idx].Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c_rx[idx].Init.Mode = DMA_NORMAL;
    hdma_i2c_rx[idx].Init.Priority = DMA_PRIORITY_HIGH;

    HAL_DMA_Init(&hdma_i2c_rx[idx]);

    /* Associate the initialized DMA handle to the the I2C handle */
    __HAL_LINKDMA(hi2c, hdmarx, hdma_i2c_rx[idx]);
#endif
    /*##-4- Configure the NVIC for DMA #########################################*/
    switch (idx)
    {
    case HAL_I2C0:
#if I2C_USE_DMA
      /* NVIC configuration for DMA transfer complete interrupt (I2Cx_TX) */
      HAL_NVIC_SetPriority(I2CI0_DMA_TX_IRQn, 0, 1);
      HAL_NVIC_EnableIRQ(I2CI0_DMA_TX_IRQn);

      /* NVIC configuration for DMA transfer complete interrupt (I2Cx_RX) */
      HAL_NVIC_SetPriority(I2CI0_DMA_RX_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(I2CI0_DMA_RX_IRQn);
#endif
      /*##-5- Configure the NVIC for I2C ########################################*/
      /* NVIC for I2Cx */
      HAL_NVIC_SetPriority(I2CI0_ER_IRQn, 1, 1);
      HAL_NVIC_EnableIRQ(I2CI0_ER_IRQn);
      HAL_NVIC_SetPriority(I2CI0_EV_IRQn, 1, 2);
      HAL_NVIC_EnableIRQ(I2CI0_EV_IRQn);
      break;
    case HAL_I2C1:
/* NVIC configuration for DMA transfer complete interrupt (I2Cx_TX) */
#if I2C_USE_DMA
      HAL_NVIC_SetPriority(I2CI1_DMA_TX_IRQn, 0, 1);
      HAL_NVIC_EnableIRQ(I2CI1_DMA_TX_IRQn);

      /* NVIC configuration for DMA transfer complete interrupt (I2Cx_RX) */
      HAL_NVIC_SetPriority(I2CI1_DMA_RX_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(I2CI1_DMA_RX_IRQn);
#endif
      /*##-5- Configure the NVIC for I2C ########################################*/
      /* NVIC for I2Cx */
      HAL_NVIC_SetPriority(I2CI1_ER_IRQn, 1, 1);
      HAL_NVIC_EnableIRQ(I2CI1_ER_IRQn);
      HAL_NVIC_SetPriority(I2CI1_EV_IRQn, 1, 2);
      HAL_NVIC_EnableIRQ(I2CI1_EV_IRQn);
      break;
    }
  }
}

/**
 * @brief I2C MSP De-Initialization
 *        This function frees the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 *          - Revert GPIO, DMA and NVIC configuration to their default state
 * @param hi2c: I2C handle pointer
 * @retval None
 */
void hal_i2c_MspDeInit(I2C_HandleTypeDef *hi2c)
{
  int32_t idx;
  idx = hal_i2c_handle_idx(hi2c);
  if ((idx >= 0) && (idx < HAL_I2C_NUM))
  {
    switch (idx)
    {
    case HAL_I2C0:
      /*##-1- Reset peripherals ##################################################*/
      I2CI0_FORCE_RESET();
      I2CI0_RELEASE_RESET();

/*##-2- Disable peripherals and GPIO Clocks #################################*/
/* Configure I2C Tx as alternate function  */
// HAL_GPIO_DeInit(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_PIN);
// /* Configure I2C Rx as alternate function  */
// HAL_GPIO_DeInit(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_PIN);

/*##-4- Disable the NVIC for DMA ###########################################*/
#if I2C_USE_DMA
      HAL_NVIC_DisableIRQ(I2CI0_DMA_TX_IRQn);
      HAL_NVIC_DisableIRQ(I2CI0_DMA_RX_IRQn);
#endif
      /*##-5- Disable the NVIC for I2C ##########################################*/
      HAL_NVIC_DisableIRQ(I2CI0_ER_IRQn);
      HAL_NVIC_DisableIRQ(I2CI0_EV_IRQn);
      break;
    case HAL_I2C1:
      /*##-1- Reset peripherals ##################################################*/
      I2CI1_FORCE_RESET();
      I2CI1_RELEASE_RESET();

/*##-2- Disable peripherals and GPIO Clocks #################################*/
/* Configure I2C Tx as alternate function  */
// HAL_GPIO_DeInit(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_PIN);
// /* Configure I2C Rx as alternate function  */
// HAL_GPIO_DeInit(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_PIN);

/*##-4- Disable the NVIC for DMA ###########################################*/
#if I2C_USE_DMA
      HAL_NVIC_DisableIRQ(I2CI1_DMA_TX_IRQn);
      HAL_NVIC_DisableIRQ(I2CI1_DMA_RX_IRQn);
#endif
      /*##-5- Disable the NVIC for I2C ##########################################*/
      HAL_NVIC_DisableIRQ(I2CI1_ER_IRQn);
      HAL_NVIC_DisableIRQ(I2CI1_EV_IRQn);
      break;
    }

/*##-3- Disable the DMA Channels ###########################################*/
/* De-Initialize the DMA Channel associated to transmission process */
#if I2C_USE_DMA
    HAL_DMA_DeInit(&hdma_i2c_tx[idx]);
    /* De-Initialize the DMA Channel associated to reception process */
    HAL_DMA_DeInit(&hdma_i2c_rx[idx]);
    memset(&hdma_i2c_rx[idx], 0, sizeof(DMA_HandleTypeDef));
    memset(&hdma_i2c_tx[idx], 0, sizeof(DMA_HandleTypeDef));
#endif
    memset(&handle_i2c[idx], 0, sizeof(I2C_HandleTypeDef));
  }
}
