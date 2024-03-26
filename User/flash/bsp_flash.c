#include "bsp_flash.h"


uint8_t flash_readdata[256] = {0};
uint8_t flash_writedata[256] = {0};

uint8_t FLASH_CMD[10]= {0x4B ,0};
uint8_t FLASH_Dummy_Byte 	= 0xFF; 

uint32_t SPI_FLASH_ReadID(void)
{
	HAL_StatusTypeDef status = HAL_OK;	
	uint32_t Temp[4] = {0};
	uint8_t ID[3] = {0};
	
	//FLASH_CMD = W25X_JedecDeviceID;
	
	SPI_FLASH_CS_LOW();
	
	status = HAL_SPI_Transmit(&hspi1, FLASH_CMD, 1, 1000);
	status = HAL_SPI_Receive(&hspi1, ID, 3, 1000);

	
	SPI_FLASH_CS_HIGH();
	printf("%d\r\n",status);
	printf("%X\r\n",ID[0]);
	printf("%X\r\n",ID[1]);
	printf("%X\r\n",ID[2]);
	Temp[3] = ID[0]<<16| ID[1]<<8 | ID[2];
	
	return Temp[3];
}

uint64_t Read_Unique_ID(void)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	uint64_t Temp = 0,b;	
	uint8_t ID[8] = {0};
	
	SPI_FLASH_CS_LOW();
	
	status = HAL_SPI_Transmit(&hspi1, FLASH_CMD, 5, 1000);
	status = HAL_SPI_Receive(&hspi1, ID, 8, 1000);

	
	SPI_FLASH_CS_HIGH();
	printf("%d\r\n",status);
	printf("0x%X",ID[0]);
	printf("%X",ID[1]);
	printf("%X",ID[2]);
	printf("%X",ID[3]);
	printf("%X",ID[4]);
	printf("%X",ID[5]);
	printf("%X",ID[6]);
	printf("%X\r\n",ID[7]);

	Temp |= ID[0];
	Temp <<= 56;
	Temp |= ID[1];
	Temp <<= 48;	
	Temp |= ID[2];
	Temp <<= 40;	
	Temp |= ID[3];
	Temp <<= 32;	
	Temp |= ID[4];
	Temp <<= 24;	
	Temp |= ID[5];
	Temp <<= 16;
	Temp |= ID[6];
	Temp <<= 8;
	Temp |= ID[7];	


    b = Temp>>32;
	printf("0x%X %X\r\n",(uint32_t)b, (uint32_t)Temp);
	
	return Temp;
}

