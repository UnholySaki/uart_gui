#ifndef __MEM_JUMP_FUNCTION_H
#define __MEM_JUMP_FUNCTION_H

#include "stm32f1xx_hal.h"




void go_to_main_app(uint32_t jump_adr);
void go_to_update_app(uint32_t jump_adr, uint32_t indicator_adr);


#endif /* __MEM_JUMP_FUNCTION_H */