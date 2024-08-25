/*
 * TWI.h
 *
 * Created: 20/08/2024 16:48:11
 *  Author: Usuario
 */ 


#ifndef TWI_H_
#define TWI_H_
#define F_CPU 16000000
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Definición del tiempo de espera (timeout) para las operaciones TWI
#define TWI_TIMEOUT 1600

// Definición de los códigos de estado de TWI (según la especificación del ATmega328P)
#define TWI_START       0x08    // Se ha enviado la condición de START
#define TWI_RSTART      0x10    // Se ha enviado la condición de RESTART
#define TWIT_ADDR_ACK   0x18    // ACK recibido después de enviar la dirección en modo de escritura
#define TWIT_ADDR_NACK  0x20    // NACK recibido después de enviar la dirección en modo de escritura
#define TWIT_DATA_ACK   0x28    // ACK recibido después de enviar un dato en modo de escritura
#define TWIT_DATA_NACK  0x30    // NACK recibido después de enviar un dato en modo de escritura

#define TWIR_ADDR_ACK   0x40    // ACK recibido después de enviar la dirección en modo de lectura
#define TWIR_ADDR_NACK  0x48    // NACK recibido después de enviar la dirección en modo de lectura
#define TWIR_DATA_ACK   0x50    // ACK recibido después de recibir un dato en modo de lectura
#define TWIR_DATA_NACK  0x58    // NACK recibido después de recibir un dato en modo de lectura

#define TWI_ERROR       0x38    // Error en el bus TWI
#define TWI_NONE        0xF8    // No hay ningún estado relevante en el bus TWI

enum {
	TWI_OK,             // Operación exitosa
	TWI_ERROR_START,    // Error al enviar la condición de START
	TWI_ERROR_RSTART,   // Error al enviar la condición de RESTART
	TWI_NACK            // No se recibió un ACK esperado
};

// Declaración de las funciones de la librería TWI
void twi_init(uint32_t speed);
uint8_t twi_wire(uint8_t addr,uint8_t reg,uint8_t *data,uint16_t len);
uint8_t twi_read(uint8_t addr,uint8_t reg,uint8_t *data,uint16_t len);


#endif /* TWI_H_ */