#include "bsp_flash.h"

HAL_StatusTypeDef SPI_FLASH_ReadID(uint32_t *JedecID)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t FLASH_CMD[1] = {W25X_JedecDeviceID};
	uint8_t ID[3] = {0};

	SPI_FLASH_CS_LOW();

	status = HAL_SPI_Transmit(&SPI_FLASH_Handle, FLASH_CMD, 1, FLASH_TIMEOUT);
	status = HAL_SPI_Receive(&SPI_FLASH_Handle, ID, 3, FLASH_TIMEOUT);

	SPI_FLASH_CS_HIGH();

	*JedecID = (uint32_t)ID[0] << 16 | (uint32_t)ID[1] << 8 | (uint32_t)ID[2];

	return status;
}

HAL_StatusTypeDef SPI_FLASH_WaitForWriteEnd(void)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t FLASH_CMD[1] = {W25X_ReadStatusReg1};
	uint8_t BUSY[1] = {1};	
	
	do
	{
		SPI_FLASH_CS_LOW();

		status = HAL_SPI_Transmit(&SPI_FLASH_Handle, FLASH_CMD, 1, FLASH_TIMEOUT);
		status = HAL_SPI_Receive(&SPI_FLASH_Handle, BUSY, 1, FLASH_TIMEOUT);

		SPI_FLASH_CS_HIGH();
	}while((BUSY[0]&0x01) == WIP_Flag);
	
	return status;
}

void SPI_FLASH_WriteEnable(void)
{
	uint8_t FLASH_CMD[1] = {W25X_WriteEnable};
	
	SPI_FLASH_CS_LOW();
	
	HAL_SPI_Transmit(&SPI_FLASH_Handle, FLASH_CMD, 1, FLASH_TIMEOUT);

	SPI_FLASH_CS_HIGH();	
}

void SPI_FLASH_WriteDisable(void)
{
	uint8_t FLASH_CMD[1] = {W25X_WriteDisable};
	
	SPI_FLASH_CS_LOW();
	
	HAL_SPI_Transmit(&SPI_FLASH_Handle, FLASH_CMD, 1, FLASH_TIMEOUT);

	SPI_FLASH_CS_HIGH();	
}

void SPI_Flash_PowerDown(void)
{	
	uint8_t FLASH_CMD[1] = {W25X_PowerDown};
	
	SPI_FLASH_CS_LOW();
	
	HAL_SPI_Transmit(&SPI_FLASH_Handle, FLASH_CMD, 1, FLASH_TIMEOUT);

	SPI_FLASH_CS_HIGH();
}

void SPI_Flash_WAKEUP(void)
{
	uint8_t FLASH_CMD[4] = {W25X_ReleasePowerDown};
	uint8_t ID[1] = {0};
	
	SPI_FLASH_CS_LOW();
	
	HAL_SPI_Transmit(&SPI_FLASH_Handle, FLASH_CMD, 4, FLASH_TIMEOUT);
	HAL_SPI_Receive(&SPI_FLASH_Handle, ID, 1, FLASH_TIMEOUT);
	
	SPI_FLASH_CS_HIGH();
}

void SPI_FLASH_BufferRead(uint32_t ReadAddr, uint8_t* pBuffer, uint16_t NumByteToRead)
{
	uint8_t FLASH_CMD[4] = {W25X_ReadData,0};
	
	FLASH_CMD[1] = (uint8_t)(ReadAddr>>16);
	FLASH_CMD[2] = (uint8_t)((ReadAddr>>8) & 0xFF);
	FLASH_CMD[3] = (uint8_t)(ReadAddr & 0xFF);	
	
	SPI_FLASH_CS_LOW();
	
	HAL_SPI_Transmit(&SPI_FLASH_Handle, FLASH_CMD, 4, FLASH_TIMEOUT);
	HAL_SPI_Receive(&SPI_FLASH_Handle, pBuffer, NumByteToRead, FLASH_TIMEOUT);
	
	SPI_FLASH_CS_HIGH();	
}







