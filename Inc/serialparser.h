#pragma once
#include "stm32f1xx_hal.h"

typedef struct{
   uint8_t state;
   uint8_t index;
   uint8_t command;
   unsigned char data[4];
   uint32_t value;
} HandleTypeDef_Parsermanager;
