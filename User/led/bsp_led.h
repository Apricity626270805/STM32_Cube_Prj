#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "gpio.h"
#include "stdio.h"
#include "string.h"

typedef enum
{
	LED_Colour_black	= 0u, //ºÚ
	LED_Colour_red			, //ºì
	LED_Colour_yellow		, //»Æ
	LED_Colour_green		, //ÂÌ
	LED_Colour_cyan			, //Çà
	LED_Colour_blue			, //À¶
	LED_Colour_purple		, //×Ï
	LED_Colour_white		  //°×	
}LED_Colour_State;

void LED_Colour_Ctrl(LED_Colour_State LED_Colour);

#endif /* __BSP_LED_H__ */
