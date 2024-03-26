#include "bsp_key.h"

GPIO_PinState KEY_Read_Value(KEY_ID Key_id)
{
	switch(Key_id)
    {
        case KEY1:
            return HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);          
        case KEY2:
            return HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);   
        default:
            return GPIO_PIN_RESET;
    }	
}







