/*
 * DHT11.h
 *
 * Created: 16/08/2024 11:30:30
 *  Author: Usuario
 */ 


#ifndef DHT11_H_
#define DHT11_H_
#include <avr/io.h>


void DHT11_Init(void);
uint8_t DHT11_Read(float *DHT_Temperatura, float *DHT_humedad);

#endif /* DHT11_H_ */