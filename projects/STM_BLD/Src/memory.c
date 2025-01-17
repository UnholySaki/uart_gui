#include "../Inc/memory.h"
#include "../Inc/memory_map.h"

static const uint8_t num_of_vectors = 4; // Check number of reset vectors when check validation

/*============================DEFINE====================================*/
typedef void (*pfunction)(void);

/*============================FUNCTIONS====================================*/

/**
 * @brief set MSP to jump to main image
 * @param jump_adr address to jump to
 */
void go_to_main_app(uint32_t jump_adr)
{
  uint32_t JumpAddress;
  pfunction Jump_To_Application;

  // check for code
  JumpAddress = *(uint32_t *)(jump_adr + 4);
  Jump_To_Application = (pfunction)JumpAddress;

  __set_MSP(*(uint32_t *)jump_adr);
  Jump_To_Application();
}

/**
 * @brief set MSP to jump to update image, clear the jump indicator
 * @param jump_adr address to jump to
 * @param indicator_adr address that indicate jump signal
 */
void go_to_update_app(uint32_t jump_adr, uint32_t indicator_adr)
{
  uint32_t JumpAddress;
  pfunction Jump_To_Application;

  // check for code
  if (((*(uint32_t *)jump_adr) & 0x2FFE0000) == 0x20000000)
  {
    __disable_irq();

    // clear flag
    HAL_FLASH_Unlock();
    FLASH_PageErase(indicator_adr);
    CLEAR_BIT(FLASH->CR, (FLASH_CR_PER));
    HAL_FLASH_Lock();

    // jump to app
    JumpAddress = *(uint32_t *)(jump_adr + 4);
    Jump_To_Application = (pfunction)JumpAddress;

    __set_MSP(*(uint32_t *)jump_adr);
    Jump_To_Application();
  }
}

/**
 * @brief Erase memory section (32kB)
 * @param adr start of memory section
 * @param size how much data to be erased
 */
void erase_memory(uint32_t adr, uint32_t size)
{
  uint32_t start_adr = adr;
  while (start_adr < adr + size)
  {
    HAL_FLASH_Unlock();
    FLASH_PageErase(start_adr);
    CLEAR_BIT(FLASH->CR, (FLASH_CR_PER));
    HAL_FLASH_Lock();

    start_adr += 0x400; // 1 PAGE
  }
}

/**
 * @brief Verify that app image exists
 * @return true if verification is successful, false otherwise
 */
bool verify_memory(uint32_t start_adr)
{
  uint32_t *p32 = (uint32_t *)(start_adr);
  if ((p32[0] < RAM_START_ADDR) || (p32[0] > RAM_END_ADDR))
  {
    return false;
  }
  else
  {
    for (uint8_t i = 1; i <= num_of_vectors; i++)
    {
      if ((p32[i] < FLASH_START_ADDR) || (p32[i] > FLASH_END_ADDR))
      {
        return false;
      }
    }
  }
  return true;
}