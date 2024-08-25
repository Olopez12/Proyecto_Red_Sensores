/*
 * Maestro_RED_Sensores.c
 *
 * Created: 10/08/2024 23:32:00
 * Author : Usuario
 */ 

//Codigo del maestro que recibe los datos de los eslcavos 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "avr/interrupt.h"
#include "SPI/SPI.h"
#include "LCD/LCD8.h"
#include "TWI/TWI.h"
#include "TSL2561/TSL2561.h"
#include "UART/UART.h"

uint8_t valorI2C = 0;
uint8_t door = 0;
char CONV[8];
char DHT[10];
int main(void)
{
	InLCD();
	twi_init(400000);  // Inicializar TWI con 400 kHz
	sei(); // Habilitar interrupciones globales
	TSL2561_init();    // Inicializar el TSL2561
	UART_init();       // Inicializar UART
	while (1)
	{
		// Mostrar títulos en LCD
		LCD_cursor(1, 1);
		LCD_W_String("SM:");
		LCD_cursor(9, 1);
		LCD_W_String("ST:");
		LCD_cursor(9, 2);
		LCD_W_String("SH:");
		LCD_cursor(1, 2);
		LCD_W_String("SL:");
		
		// RUTINA PARA PODER RECIBIR DATOS DE LA PUERTA
		uint8_t door_address = 0x08; // Dirección del esclavo 2
		twi_read(door_address, 0x00, &valorI2C, 1);  // Leer el estado de la puerta

		itoa(valorI2C, CONV, 10);
		
		// Mostrar el valor de valorI2C (0 o 1) en la pantalla LCD
		LCD_cursor(4, 1);
		LCD_W_String(CONV);
		send_via_uart('P', CONV);  // Enviar valor de la puerta por UART
		
		
		// RUTINA PARA RECIBIR DATOS DE TEMPERATURA Y HUMEDAD
		uint8_t sensor_address = 0x20;  // Dirección del esclavo 1
		uint8_t temp_hum_data[4];

		// Leer 4 bytes consecutivos de temperatura y humedad con ACK
		twi_read(sensor_address, 0x00, temp_hum_data, 4);

		// Separar los bytes de temperatura y humedad
		uint16_t temp = (temp_hum_data[0] << 8) | temp_hum_data[1];
		uint16_t hum = (temp_hum_data[2] << 8) | temp_hum_data[3];
		
		// Convertir a float para mostrar
		float temp_f = temp / 10.0;
		float hum_f = hum / 10.0;

		// Mostrar en LCD
		dtostrf(temp_f, 2, 2, DHT);
		LCD_cursor(12, 1);
		LCD_W_String(DHT);
		
		send_via_uart('T', DHT);  // Enviar temperatura por UART
		
		
		dtostrf(hum_f, 2, 2, DHT);
		LCD_cursor(12, 2);
		LCD_W_String(DHT);
		send_via_uart('H', DHT);  // Enviar humedad por UART
		
		// RUTINA PARA LEER DATOS DEL TSL2561
		uint16_t lux = TSL2561_read_lux();
		lux = lux / 10; // Dividir por 10 
		
		itoa(lux, CONV, 10);
		LCD_clr(4,2);
		LCD_clr(5,2);
		LCD_clr(6,2);
		LCD_clr(7,2);
		
		LCD_cursor(4, 2);
		LCD_W_String(CONV);
		 send_via_uart('L', CONV);  // Enviar valor de luz por UART
	}
}


