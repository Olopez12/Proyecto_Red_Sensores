/*
 * Esclavo_I2C_Puerta.c
 *
 * Created: 13/08/2024 22:33:30
 * Author : Usuario
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "avr/interrupt.h"
#include "I2C/I2C.h"
#include "Servo/Servo.h"

// Declarar la variable movimiento
volatile uint8_t movimiento = 0;

ISR(TWI_vect) {
	switch (TWSR & 0xF8) {
		case 0x60: //
		case 0x70: //
		TWCR |= (1 << TWINT) | (1 << TWEA); // Responder con ACK
		break;
		case 0xA8: // Solicitud de datos del maestro
		TWDR = movimiento; // Enviar el valor actual de movimiento
		TWCR |= (1 << TWINT) | (1 << TWEA); // Responder con ACK
		break;
		case 0xC0: // Datos enviados y NACK recibido
		case 0xC8: // Último byte enviado
		TWCR |= (1 << TWINT) | (1 << TWEA); // Responder con ACK
		break;
		default:
		TWCR |= (1 << TWINT) | (1 << TWEA); // Responder con ACK por defecto
		break;
	}
}

int main(void)
{
	// Configurar PINB0 como entrada y PINB1 como salida
	DDRD &= ~(1<<PIND2); 
	DDRD |= (1 << PIND3) | (1 << PIND5); 
	PORTD &= ~(1<<PIND3);
	I2C_Slave_Init(0x08); // Inicializar I2C
	sei(); // Habilitar interrupciones globales
	timer1_init();
	while (1)
	{
		
		// Leer el estado del pin PIND2
		if (PIND & (1<<PIND2)) {
			PORTD &= ~(1<<PIND3); // Apagar el LED si PIND2 está en high
			movimiento = 1;
			mover_servomotor(90);
			
			} else {
			PORTD |= (1<<PIND3);  // Encender el LED si PIND2 está en low
			movimiento = 0;
			mover_servomotor(0);
			
		}
		
	}
}
