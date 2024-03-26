#include "bsp_eeprom.h"

uint8_t eeprom_readdata[EEPROM_PAGESIZE * EEPROM_SECTORSIZE] = {0};
uint8_t eeprom_writedata[EEPROM_PAGESIZE * EEPROM_SECTORSIZE] = {0};

HAL_StatusTypeDef byte_read(uint16_t addr, uint8_t *pdata)
{
	return HAL_I2C_Mem_Read(&EEPROM_I2C_Handle, EEPROM_ADDR, addr, EEPROM_MEMADDRSIZE, pdata, 1, EEPROM_TIMEOUT_MAX);
}

HAL_StatusTypeDef EEPROM_read(uint16_t addr, uint8_t *pdata, uint16_t datasize)
{
	return HAL_I2C_Mem_Read(&EEPROM_I2C_Handle, EEPROM_ADDR, addr, EEPROM_MEMADDRSIZE, pdata, datasize, EEPROM_TIMEOUT_MAX);
}

HAL_StatusTypeDef byte_write(uint16_t addr, uint8_t *pdata)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(&EEPROM_I2C_Handle, EEPROM_ADDR, addr, EEPROM_MEMADDRSIZE, pdata, 1, EEPROM_TIMEOUT_MAX);

	while (HAL_I2C_IsDeviceReady(&EEPROM_I2C_Handle, EEPROM_ADDR, EEPROM_MAX_TRIALS, EEPROM_TIMEOUT_MAX) != HAL_OK)
	{
		/*  */
	}

	return status;
}

static HAL_StatusTypeDef page_write(uint16_t addr, uint8_t *pdata, uint16_t datasize)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(&EEPROM_I2C_Handle, EEPROM_ADDR, addr, EEPROM_MEMADDRSIZE, pdata, datasize, EEPROM_TIMEOUT_MAX);

	while (HAL_I2C_IsDeviceReady(&EEPROM_I2C_Handle, EEPROM_ADDR, EEPROM_MAX_TRIALS, EEPROM_TIMEOUT_MAX) != HAL_OK)
	{
		/*  */
	}

	return status;
}

HAL_StatusTypeDef EEPROM_Write(uint16_t addr, uint8_t *pdata, uint16_t datasize)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

	Addr = addr % EEPROM_PAGESIZE;
	count = EEPROM_PAGESIZE - Addr;
	NumOfPage = datasize / EEPROM_PAGESIZE;
	NumOfSingle = datasize % EEPROM_PAGESIZE;

	if (Addr == 0)
	{
		if (NumOfPage == 0)
		{
			status = page_write(addr, pdata, NumOfSingle);
		}
		else
		{
			while (NumOfPage--)
			{
				status = page_write(addr, pdata, EEPROM_PAGESIZE);
				addr += EEPROM_PAGESIZE;
				pdata += EEPROM_PAGESIZE;
			}

			if (NumOfSingle != 0)
			{
				status = page_write(addr, pdata, NumOfSingle);
			}
		}
	}
	else
	{
		if (NumOfPage == 0)
		{
			status = page_write(addr, pdata, NumOfSingle);
		}
		else
		{
			datasize -= count;
			NumOfPage = datasize / EEPROM_PAGESIZE;
			NumOfSingle = datasize % EEPROM_PAGESIZE;

			if (count != 0)
			{
				status = page_write(addr, pdata, count);
				addr += count;
				pdata += count;
			}

			while (NumOfPage--)
			{
				status = page_write(addr, pdata, EEPROM_PAGESIZE);
				addr += EEPROM_PAGESIZE;
				pdata += EEPROM_PAGESIZE;
			}

			if (NumOfSingle != 0)
			{
				status = page_write(addr, pdata, NumOfSingle);
			}
		}
	}
	return status;
}
