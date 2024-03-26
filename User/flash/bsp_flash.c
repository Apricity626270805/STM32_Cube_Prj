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
