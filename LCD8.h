/*
 * LCD8.h
 *
 * Created: 19/07/2024 00:00:34
 *  Author: Usuario
 */ 

#include <avr/io.h>
#include <util/delay.h>

#ifndef LCD8_H_
#define LCD8_H_

//iniciar lcd
void InLCD(void);
void Puerto(char a);
void LCD_CMD(char b);
void LCD_Write(char c);
void LCD_W_String(char *d);
void shift_izq(void);
void shift_der(void);
void LCD_cursor(char c, char f); 
void LCD_clr(char col, char row);
#endif /* LCD8_H_ */