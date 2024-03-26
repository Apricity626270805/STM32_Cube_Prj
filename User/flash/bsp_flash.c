#include "bsp_flash.h"


uint8_t flash_readdata[256] = {0};
uint8_t flash_writedata[256] = {0};

uint8_t FLASH_CMD[]= {0x90 ,0};
uint8_t FLASH_Dummy_Byte 	= 0xFF; 

uint32_t SPI_FLASH_ReadID(void)
{
	HAL_StatusTypeDef status = HAL_OK;	
	uint32_t Temp[4] = {0};
	uint8_t ID[2] = {0};
	
	//FLASH_CMD = W25X_JedecDeviceID;
	
	SPI_FLASH_CS_LOW();
	
	status = HAL_SPI_Transmit(&hspi1, FLASH_CMD, 4, 1000);
	status = HAL_SPI_Receive(&hspi1, ID, 2, 1000);

	
	SPI_FLASH_CS_HIGH();
	printf("%d\r\n",status);
	printf("%X\r\n",Temp[0]);
	printf("%X\r\n",Temp[1]);
	printf("%X\r\n",Temp[2]);
	Temp[3] = ID[0]<<8 | ID[1];
	
	return Temp[3];
}



