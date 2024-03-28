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
	uint8_t FLASH_CMD[4] = {W25X_ReadData, 0x00, 0x00, 0x00};
	
	FLASH_CMD[1] = (uint8_t)(ReadAddr >> 16);
	FLASH_CMD[2] = (uint8_t)((ReadAddr & 0xFF00) >> 8);
	FLASH_CMD[3] = (uint8_t)(ReadAddr & 0xFF);	
	
	SPI_FLASH_CS_LOW();
	
	HAL_SPI_Transmit(&SPI_FLASH_Handle, FLASH_CMD, 4, FLASH_TIMEOUT);
	HAL_SPI_Receive(&SPI_FLASH_Handle, pBuffer, NumByteToRead, FLASH_TIMEOUT);
	
	SPI_FLASH_CS_HIGH();	
}

void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
	uint8_t FLASH_CMD[4] = {W25X_SectorErase, 0x00, 0x00, 0x00};
	
	FLASH_CMD[1] = (uint8_t)(SectorAddr >> 16);
	FLASH_CMD[2] = (uint8_t)((SectorAddr & 0xFF00) >> 8);
	FLASH_CMD[3] = (uint8_t)(SectorAddr & 0xFF);	
	
	SPI_FLASH_WriteEnable();
	
	SPI_FLASH_CS_LOW();
	
	HAL_SPI_Transmit(&SPI_FLASH_Handle, FLASH_CMD, 4, FLASH_TIMEOUT);
	
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_BulkErase(void)
{
	uint8_t FLASH_CMD[1] = {W25X_ChipErase};	
	
	SPI_FLASH_WriteEnable();
	
	SPI_FLASH_CS_LOW();
	
	HAL_SPI_Transmit(&SPI_FLASH_Handle, FLASH_CMD, 1, FLASH_TIMEOUT);
	
	SPI_FLASH_CS_HIGH();	
	
	SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_PageWrite(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)
{
	uint8_t FLASH_CMD[4] = {W25X_PageProgram, 0x00, 0x00, 0x00};
	
	FLASH_CMD[1] = (uint8_t)(WriteAddr >> 16);
	FLASH_CMD[2] = (uint8_t)((WriteAddr & 0xFF00) >> 8);
	FLASH_CMD[3] = (uint8_t)(WriteAddr & 0xFF);	
	
	if(NumByteToWrite > 256)
	{
		NumByteToWrite = 256;
	}
	
	SPI_FLASH_WriteEnable();
	
	SPI_FLASH_CS_LOW();
	
	HAL_SPI_Transmit(&SPI_FLASH_Handle, FLASH_CMD, 4, FLASH_TIMEOUT);
	HAL_SPI_Transmit(&SPI_FLASH_Handle, pBuffer, NumByteToWrite, FLASH_TIMEOUT);
	
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_BufferWrite(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	Addr = WriteAddr % SPI_FLASH_PageSize;
	count = SPI_FLASH_PageSize - Addr;
	NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	
	if(Addr == 0)
	{
		if(NumOfPage == 0)
		{
			SPI_FLASH_PageWrite(WriteAddr, pBuffer, NumByteToWrite);
		}
		else
		{
			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(WriteAddr, pBuffer, SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}
			
			if(NumOfSingle != 0)
			{
				SPI_FLASH_PageWrite(WriteAddr, pBuffer, NumOfSingle);
			}			
		}
	}
	else
	{
		if(NumOfPage == 0)
		{
			if(NumOfSingle > count)
			{
				temp = NumOfSingle - count;
				SPI_FLASH_PageWrite(WriteAddr, pBuffer, count);
				WriteAddr += count;
				pBuffer += count;				
				
				SPI_FLASH_PageWrite(WriteAddr, pBuffer, temp);
			}
			else
			{
				SPI_FLASH_PageWrite(WriteAddr, pBuffer, NumOfSingle);
			}
		}
		else
		{
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

			SPI_FLASH_PageWrite(WriteAddr, pBuffer, count);
			WriteAddr +=  count;
			pBuffer += count;
			
			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(WriteAddr, pBuffer, SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;				
			}
			
			if(NumOfSingle != 0)
			{
				SPI_FLASH_PageWrite(WriteAddr, pBuffer, NumOfSingle);
			}			
		}
	}
}


