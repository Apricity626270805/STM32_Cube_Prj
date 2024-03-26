#include "bsp_usart.h"

uint8_t rx_data[10] = {0};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		switch(rx_data[0])
		{
			case '0':LED_Colour_Ctrl(LED_Colour_black	);break;
			case '1':LED_Colour_Ctrl(LED_Colour_red		);break;
			case '2':LED_Colour_Ctrl(LED_Colour_yellow	);break;
			case '3':LED_Colour_Ctrl(LED_Colour_green	);break;
			case '4':LED_Colour_Ctrl(LED_Colour_cyan	);break;
			case '5':LED_Colour_Ctrl(LED_Colour_blue	);break;
			case '6':LED_Colour_Ctrl(LED_Colour_purple	);break;
			case '7':LED_Colour_Ctrl(LED_Colour_white	);break;
			default: LED_Colour_Ctrl(LED_Colour_black	);break;
		}
		HAL_UART_Receive_IT(&huart1, rx_data, 1 );
	} 
}

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
	
	return ch;
}



