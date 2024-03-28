#include "bsp_adc.h"

uint32_t Get_ADC_V(void)
{
	HAL_ADC_Start(&hadc1);
	
	while(HAL_ADC_GetState(&hadc1) == HAL_ADC_STATE_REG_EOC );
	
	return HAL_ADC_GetValue(&hadc1);
}


