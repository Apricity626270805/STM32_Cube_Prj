#ifndef __BSP_EEPROM_H__
#define __BSP_EEPROM_H__

#include "i2c.h"
#include "main.h"

#define EEPROM_ADDR  			0xA0

#define EEPROM_PAGESIZE  		8
#define EEPROM_SECTORSIZE  		32
#define EEPROM_MEMADDRSIZE  	I2C_MEMADD_SIZE_8BIT

#define EEPROM_MAX_TRIALS		300
#define EEPROM_TIMEOUT_MAX		300

#define EEPROM_I2C_Handle		hi2c1


extern uint8_t eeprom_readdata[];
extern uint8_t eeprom_writedata[];


HAL_StatusTypeDef byte_write(uint16_t addr, uint8_t *pdata);
HAL_StatusTypeDef EEPROM_Write(uint16_t addr, uint8_t *pdata, uint16_t datasize);

HAL_StatusTypeDef byte_read(uint16_t addr, uint8_t *pdata);
HAL_StatusTypeDef EEPROM_read(uint16_t addr, uint8_t *pdata, uint16_t datasize);

#endif /* __BSP_EEPROM_H__ */

