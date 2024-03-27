#ifndef __BSP_FLASH_H__
#define __BSP_FLASH_H__

#include "spi.h"
#include "gpio.h"
#include "bsp_usart.h"

#define FLASH_ID		                0XEF4017     //W25Q64

//#define SPI_FLASH_PageSize            4096
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
/************** 命令定义-开头 ******************/
#define W25X_WriteEnable		     	0x06 
#define W25X_WriteDisable		     	0x04 
#define W25X_ReadStatusReg1		    	0x05
#define W25X_WriteStatusReg		    	0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      	0x0B 
#define W25X_FastReadDual		      	0x3B 
#define W25X_PageProgram		      	0x02 
#define W25X_BlockErase			      	0xD8 
#define W25X_SectorErase		      	0x20 
#define W25X_ChipErase			      	0xC7 
#define W25X_PowerDown			      	0xB9 
#define W25X_ReleasePowerDown	    	0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   		0x90 
#define W25X_JedecDeviceID		    	0x9F

#define WIP_Flag                  		0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                		0xFF
/************** 命令定义-结尾 ******************/
#define SPI_FLASH_Handle                hspi1
#define SPI_FLASH_CS_LOW() 				HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);                
#define SPI_FLASH_CS_HIGH()				HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);

/*等待超时时间*/
#define FLASH_TIMEOUT                   1000

HAL_StatusTypeDef SPI_FLASH_ReadID(uint32_t *JedecID);
HAL_StatusTypeDef SPI_FLASH_WaitForWriteEnd(void);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WriteDisable(void);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);
void SPI_FLASH_BufferRead(uint32_t ReadAddr, uint8_t* pBuffer, uint16_t NumByteToRead);


void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);

#endif /* __BSP_FLASH_H__ */
