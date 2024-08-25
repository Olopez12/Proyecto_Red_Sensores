/*
 * DHT11.c
 *
 * Created: 16/08/2024 11:30:14
 *  Author: Usuario
 */ 

#include "DHT11.h"
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

//Funciones para la lectura 
void DHT11_Init(void)
{
	DDRD |= (1<<PIND1); 
	PORTD |= (1<<PIND1);
}
uint8_t DHT11_Read(float *DHT_Temperatura, float *DHT_humedad)
{
	uint8_t info[5];
	uint8_t i, j = 0;
	uint8_t cont = 0;
	
	// Primer paso, pulso de 18ms
	PORTD &= ~(1<<PIND1);
	_delay_ms(18);
	PORTD |= (1<<PIND1);
	DDRD &= ~(1<<PIND1);
	
	cont = 0;
	// Esperar los 20 o 40 microsegundos pero con una función de seguridad
	while (PIND & (1<<PIND1))
	{
		_delay_us(2);
		cont += 2;
		if (cont > 60)
		{
			DDRD |= (1<<PIND1);
			PORTD |= (1<<PIND1);
			return 0;
		}
	}
	
	// paso 3
	cont = 0;
	while (!(PIND &(1<<PIND1))){
		_delay_us(2);
		cont += 2;
		if (cont > 100)
		{
			DDRD |= (1<<PIND1);
			PORTD |= (1<<PIND1);
			return 0;
		}
	}
	
	// Paso 4
	cont = 0;
	while (PIND &(1<<PIND1))
	{
		_delay_us(2);
		cont += 2;
		if (cont > 100)
		{
			DDRD |= (1<<PIND1);
			PORTD |= (1<<PIND1);
			return 0;
		}
	}
	
	for (j = 0; j < 5; j++)
	{
		uint8_t data = 0;
		for (i = 0; i < 8; i++) 
				{
			while (!(PIND &(1<<PIND1)));
			_delay_us(35);
			
			if (PIND & (1<<PIND1))
			data |= (1<<(7-i));
			
			while (PIND & (1<<PIND1));
		}
		info[j] = data;
	}
	
	DDRD |= (1<<PIND1);
	PORTD |= (1<<PIND1);
	
	// Conversión de variables
	if ((uint8_t) (info[0] + info[1]+info[2] + info[3]) == info[4])  // Para DHT11 la suma se hace de info[0] y info[2]
	{
		*DHT_humedad = info[0];   // Humedad sin decimales
		*DHT_Temperatura = info[2];  // Temperatura sin decimales
		
		return 1;
	}
	return 1;  // Corrige el return al final en caso de fallo
}