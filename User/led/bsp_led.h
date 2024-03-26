#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "gpio.h"
#include "stdio.h"
#include "string.h"

typedef enum
{
	LED_Colour_black	= 0u, //��
	LED_Colour_red			, //��
	LED_Colour_yellow		, //��
	LED_Colour_green		, //��
	LED_Colour_cyan			, //��
	LED_Colour_blue			, //��
	LED_Colour_purple		, //��
	LED_Colour_white		  //��	
}LED_Colour_State;

void LED_Colour_Ctrl(LED_Colour_State LED_Colour);

#endif /* __BSP_LED_H__ */
