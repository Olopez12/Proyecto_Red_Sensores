/*
 * LCD8.c
 *
 * Created: 19/07/2024 00:00:11
 *  Author: Usuario
 */ 
#define F_CPU 16000000UL
#include "LCD8.h"
#include <util/delay.h>
#include <avr/io.h>


void InLCD(void)
{
	DDRD |= (1<<PIND4)|(1<<PIND3)|(1<<PIND2);// Bit como salida
	DDRD |= (1<<PIND5)|(1<<PIND6)|(1<<PIND7);
	DDRB |= 0xFF;
	PORTB = 0;

	//ASEGURAR QUE YA PODEMOS TRABAJAR
	Puerto(0x00);
	_delay_ms(20);
	
	//
	LCD_CMD(0x38);
	_delay_us(150);
	
	LCD_CMD(0x0C);
	_delay_us(150);
	
	LCD_CMD(0x06);
	_delay_us(150);
	
	LCD_CMD(0x01);
}

void LCD_CMD(char b)
{
	//RS =0
	PORTB &= ~(1<<PORTB0);
	Puerto(b);
	
	//enable 1 
	PORTB |= (1<<PORTB1);
	_delay_ms(4);
	//enable 0 
	PORTB &=~(1<<PORTB1);
	
}


void Puerto(char a)
{
	//D0 = 1 Y 0
	if (a & 1)

		PORTB |= (1<<PORTB3);	

	else 

		PORTB &= ~(1<<PORTB3);

	
	//D1 = 1 Y 0
	if (a & 2)

		PORTB |= (1<<PORTB2);

	else

		PORTB &= ~(1<<PORTB2);

	
	//D2 = 1 Y 0
	if (a & 4)

		PORTD |= (1<<PORTD2);

	else

		PORTD &= ~(1<<PORTD2);

	
	//D3 = 1 Y 0
	if (a & 8)

		PORTD |= (1<<PORTD3);

	else

		PORTD &= ~(1<<PORTD3);

	
	//D4 = 1 Y 0
	if (a & 16)

		PORTD |= (1<<PORTD4);

	else

		PORTD &= ~(1<<PORTD4);

	//D5 = 1 Y 0
	if (a & 32)

		PORTD |= (1<<PORTD5);

	else

		PORTD &= ~(1<<PORTD5);

	
	//D6 = 1 Y 0
	if (a & 64)
		PORTD |= (1<<PORTD6);
	else
		PORTD &= ~(1<<PORTD6);
	
	
	//D7 = 1 Y 0
	if (a & 128)
		PORTD |= (1<<PORTD7);
	else

		PORTD &= ~(1<<PORTD7);
	
	
}

void LCD_Write(char c)
{
	//RS =1
	PORTB |= (1<<PORTB0);
	Puerto(c);
	
	//enable 1
	PORTB |= (1<<PORTB1);
	_delay_ms(4);
	//enable 0
	PORTB &=~(1<<PORTB1);
	
}

void LCD_W_String(char *d)
{
	int i;
	for (i =0; d[i] !='\0'; i++)
	LCD_Write(d[i]);
}

void shift_izq(void)
{
	LCD_CMD(0x18);
}

void shift_der(void)
{
	LCD_CMD(0x1C);
}

void LCD_cursor(char c, char f)
{
	char pos;

	if (f == 1)  // Si es la primera fila
	{
		pos = 0x80 + (c - 1);
	}
	else if (f == 2)  // Si es la segunda fila
	{
		pos = 0xC0 + (c - 1);
	}
	
	LCD_CMD(pos);  // Enviar el comando con la posición calculada
}
void LCD_clr(char col, char row)
{
	LCD_cursor(col, row);  // Posicionar el cursor en la posición deseada
	LCD_Write(' ');        // Escribir un espacio en la posición para borrar el contenido
}