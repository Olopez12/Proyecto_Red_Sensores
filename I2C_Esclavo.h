/*
 * I2C.h
 *
 * Created: 2/08/2024 00:13:49
 *  Author: Usuario
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
///// TIW //////

void TWI_Init(unsigned long SCL_CLOCK, uint8_t Prescaler);
uint8_t TWI_startCond(void);
void TWI_stopCond();
uint8_t TWI_write(uint8_t data2write);
uint8_t TWI_read(uint8_t *buffer, uint8_t ACK_NACK);
void I2C_Slave_Init(uint8_t address);

#endif /* I2C_H_ */