#ifndef __INTERNAL_FLASH_H__
#define __INTERNAL_FLASH_H__

#include "stm32f1xx_hal.h"

extern uint16_t Store_Data[];

uint16_t get_flash_size(void);
void Store_Init(void);
void Store_Save(void);

#endif
