/*
 * I2C.c
 *
 * Created: 2/08/2024 00:13:39
 *  Author: Usuario
 */ 

#include "I2C.h"

void TWI_Init(unsigned long SCL_CLOCK, uint8_t Prescaler)
{

	DDRC &= ~((1<<DDC4)|(1<<DDC5));
	
	switch (Prescaler){
		case 1:
		TWSR &= ~ ((1<<TWPS1)|(1<<TWPS0));
		break;
		case 4:
		TWSR &= ~(1<<TWPS1);
		TWSR |= (1<<TWPS0);
		break;
		case 16:
		TWSR &= ~(1<<TWPS0);
		TWSR |= (1<<TWPS1);
		break;
		case 64:
		TWSR |= (1<<TWPS1)|(1<<TWPS0);
		break;
		default:
		TWSR &= ~ ((1<<TWPS1)|(1<<TWPS0));
		Prescaler = 1;
		break;
		
	}
	TWBR  = ((F_CPU/SCL_CLOCK)-16)/(2*Prescaler);
	TWCR |= (1<<TWEN);
	
}

uint8_t TWI_startCond(void){
	uint8_t estado;
	TWCR = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN));
	while(!(TWCR & (1<<TWINT)));

	estado = TWSR & 0xF8;
	if ((estado != 0x08)||(estado != 0x10)){
		return 1;
		}else{
		return estado;
	}
	
}

void TWI_stopCond(){
	TWCR |= ((1<<TWINT) | (1<<TWSTO) | (1<<TWEN));
	while (TWCR &(1<<TWSTO));
}


uint8_t TWI_write(uint8_t data2write){
	
	uint8_t estado;
	
	TWDR = data2write;
	
	TWCR = ((1<<TWINT) | (1<<TWEN));
	
	while(!(TWCR & (1<<TWINT)));
	estado = TWSR & 0xF8;
	
	
	if (estado == 0x18 || estado == 0x28 || estado == 0x40){
		return 1;
		}else{
		return estado;
	}
	
	
	
	
}
uint8_t TWI_read(uint8_t *buffer, uint8_t ACK_NACK){
	
	uint8_t estado;
	if (ACK_NACK){
		TWCR |= (1<<TWEA); //lectura con ack
		}else{
		TWCR &= ~(1<<TWEA); //Lectura sin acknowledge
	}
	TWCR |= (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	estado = TWSR & 0xF8;
	//verificar dato leido con ack o sin ack
	if (estado== 0x58 || estado == 0x50){
		*buffer = TWDR;
		return 1;
		}else{
		return estado;
	}

}

void I2C_Slave_Init(uint8_t address) 
{
	TWAR = (address << 1); // Configurar direcci?n del esclavo
	TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWIE); 	
}