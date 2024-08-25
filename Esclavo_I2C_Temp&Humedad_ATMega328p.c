/*
 * Esclavo_SPI_LLUVIA.c
 *
 * Created: 12/08/2024 22:39:49
 * Author : Usuario
 */ 

//Codigo del esclavo para le lectura de temperatura y humedad
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "avr/interrupt.h"
#include "I2C/I2C.h"
#include "DHT11/DHT11.h"
#include "LCD/LCD8.h"

void initDC();
volatile uint16_t temperatura1 = 0;
volatile uint16_t humedad1 = 0;



ISR(PCINT0_vect) {
	// Verificar si el botón está presionado
	if (!(PINB & (1<<PINB0))) {
		PORTD |= (1<<PORTD0);  // Encender el motor (PD0 en alto)
		} else {
		PORTD &= ~(1<<PORTD0); // Apagar el motor (PD0 en bajo)
	}
}


ISR(TWI_vect) {
    static uint8_t byte_count = 0;
    static uint32_t data_to_send = 0;
    
    switch (TWSR & 0xF8) {
        case 0x60:  // SLA+W recibido, ACK retornado
        case 0x68:  // Pérdida de arbitraje, SLA+W recibido, ACK retornado
        // Reiniciar el contador de bytes y preparar los datos para enviar
        byte_count = 0;
        data_to_send = ((uint32_t)temperatura1 << 16) | humedad1;
        TWCR |= (1 << TWINT) | (1 << TWEA); // Aceptar SLA+W recibido
        break;

        case 0xA8:  // SLA+R recibido, ACK retornado
        case 0xB0:  // Pérdida de arbitraje, SLA+R recibido, ACK retornado
        // Cargar el siguiente byte en TWDR y ACK
        TWDR = (data_to_send >> (24 - 8 * byte_count)) & 0xFF;
        byte_count++;
        if (byte_count < 4) {
            TWCR |= (1 << TWINT) | (1 << TWEA); // ACK después de enviar byte de datos
        } else {
            TWCR |= (1 << TWINT); // NACK después de enviar el último byte de datos
        }
        break;

        case 0xB8:  // Byte de datos transmitido, ACK recibido
        TWDR = (data_to_send >> (24 - 8 * byte_count)) & 0xFF;
        byte_count++;
        if (byte_count < 4) {
            TWCR |= (1 << TWINT) | (1 << TWEA); // ACK después de enviar byte de datos
        } else {
            TWCR |= (1 << TWINT); // NACK después de enviar el último byte de datos
        }
        break;

        case 0xC0:  // Byte de datos transmitido, NACK recibido
        case 0xC8:  // Último byte transmitido, ACK recibido, condición STOP recibida
        TWCR |= (1 << TWINT) | (1 << TWEA); // Reiniciar para estar listo para la próxima comunicación
        break;

        case 0xA0:  // Condición STOP o START repetido recibida mientras todavía está direccionado como esclavo
        TWCR |= (1 << TWINT) | (1 << TWEA); // ACK y listo para la próxima comunicación
        break;

        default:
        TWCR |= (1 << TWINT) | (1 << TWEA); // Caso por defecto: ACK y listo para la próxima comunicación
        break;
    }
}


int main(void)
{
	float temperatura;
	float Humedad;
	I2C_Slave_Init(0x20);
	SPCR |= (1<<SPIE);
	sei();
	DHT11_Init();
	initDC();
	//InLCD();
	//char IN[10];
	
    /* Replace with your application code */
    while (1) 

    {
		//Dependiendo del estado del TWI, la rutina envía los datos de temperatura y humedad al maestro o prepara el esclavo para una nueva comunicación.	
		uint8_t status = DHT11_Read(&temperatura, &Humedad);
		if (status){
			temperatura1 = (uint16_t)(temperatura * 10);  // Escala a 2 decimales
			humedad1 = (uint16_t)(Humedad * 10);  // Escala a 2 decimales
			/*LCD_clr(1,1);
			LCD_cursor(1,1);
			dtostrf(temperatura,2,2,IN);
			LCD_W_String(IN);
			
			LCD_clr(1,2);
			LCD_cursor(1,2);
			dtostrf(Humedad,2,2,IN);
			LCD_W_String(IN);
			
			LCD_clr(7,1);
			LCD_cursor(7,1);
			dtostrf(temperatura1,2,2,IN);
			LCD_W_String(IN);
			
			LCD_clr(7,2);
			LCD_cursor(7,2);
			dtostrf(humedad1,2,2,IN);
			LCD_W_String(IN);*/
			
		}
		_delay_ms(1000);
    }
}

void initDC() {
	DDRD |= (1<<PIND0); // Configurar PD0
	PORTD &= ~(1<<PIND0); // Apagar los LEDs inicialmente

	DDRB &= ~((1<<PINB0)); // Configurar PB0 como entrada
	PORTB |= (1<<PINB0); // Habilitar pull-up resistors en PB0 

	PCICR |= (1<<PCIE0); // Habilitar interrupciones de cambio de pin para PCINT0
	PCMSK0 |= (1<<PCINT0); // Habilitar interrupciones para PB0
}