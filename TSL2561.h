/*
 * TSL2561.h
 *
 * Created: 20/08/2024 17:11:36
 *  Author: Usuario
 */ 


#ifndef TSL2561_H_
#define TSL2561_H_
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>


// Direcciones del TSL2561 (con ADDR desconectado)
#define TSL2561_ADDRESS 0x39 // Dirección I2C del TSL2561

// Registros del TSL2561
#define TSL2561_COMMAND_BIT 0x80
#define TSL2561_CONTROL_REG 0x00
#define TSL2561_TIMING_REG 0x01
#define TSL2561_DATA0LOW 0x0C
#define TSL2561_DATA0HIGH 0x0D
#define TSL2561_DATA1LOW 0x0E
#define TSL2561_DATA1HIGH 0x0F

void TSL2561_init();
uint16_t TSL2561_read_lux();



#endif /* TSL2561_H_ */