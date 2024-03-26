#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#include "gpio.h"

typedef enum
{
	KEY1	= 0u, //����1
	KEY2		 //����2
}KEY_ID;

GPIO_PinState KEY_Read_Value(KEY_ID Key_id);

#endif /* __BSP_KEY_H__ */
