#ifndef __MEM_JUMP_FUNCTION_H
#define __MEM_JUMP_FUNCTION_H

#include <stm32f1xx_hal.h>
#include <stm32f103xb.h>
#include <stdint.h>
#include <stdbool.h>


void go_to_main_app(uint32_t jump_adr);
void go_to_update_app(uint32_t jump_adr, uint32_t indicator_adr);
void erase_memory(uint32_t adr, uint32_t size);

#endif /* __MEM_JUMP_FUNCTION_H */