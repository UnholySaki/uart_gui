#include "mem_jump_function.h"

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