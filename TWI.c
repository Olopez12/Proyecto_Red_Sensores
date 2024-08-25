/*
 * TWI.c
 *
 * Created: 20/08/2024 16:48:00
 *  Author: Usuario
 */ 
#include "TWI.h"
volatile uint8_t status = 0xF8;

// Interrupci�n del vector TWI, utilizada para capturar el estado del bus TWI

ISR(TWI_vect){
	status = (TWSR & 0xF8); // Captura el estado actual del bus TWI
}

// Funci�n para iniciar una condici�n START en el bus TWI
static uint8_t twi_start(void){
	uint16_t i = 0;
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
	
	while(status != TWI_START){ // Espera a que se env�e la condici�n START
		i++;
		if(i >= TWI_TIMEOUT){   // Verifica si se ha superado el tiempo de espera
			return TWI_ERROR_START; // Retorna un error si se supera el timeout
		}
	}
	return TWI_OK; // Retorna �xito si se envi� la condici�n START correctamente
}

static void twi_stop(void){
	
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN) | (1 << TWIE); // Env�a la condici�n STOP
}

// Funci�n para enviar una condici�n RESTART en el bus TWI

static uint8_t twi_restart(void){
	uint16_t i = 0;
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE); 
	
	while(status != TWI_RSTART){ // Espera a que se env�e la condici�n RESTART
		i++;
		if(i >= TWI_TIMEOUT){ // Verifica si se ha superado el tiempo de espera
			return TWI_ERROR_START; // Retorna un error si se supera el timeout
		}
	}
	return TWI_OK; // Retorna �xito si se envi� la condici�n RESTART correctamente
}

static uint8_t twi_addr_write_ack(void){
	uint16_t i = 0;
	
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
	while(status != TWIT_ADDR_ACK){ // Espera el ACK de la direcci�n en modo escritura
		i++;
		if(i >= TWI_TIMEOUT){ // Verifica si se ha superado el tiempo de espera
			return TWI_ERROR_START; // Retorna un error si se supera el timeout
		}
	}
	return TWI_OK;
}


static uint8_t twi_data_write_ack(void){
	uint16_t i = 0;
	
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
	while(status != TWIT_DATA_ACK){ // Espera el ACK del dato enviado
		i++; 
		if(i >= TWI_TIMEOUT){ // Verifica si se ha superado el tiempo de espera
			return TWI_ERROR_START; // Retorna un error si se supera el timeout
		}
	}
	return TWI_OK;
}


// Funci�n para enviar la direcci�n del esclavo en modo de lectura y recibir ACK
static uint8_t twi_addr_read_ack(void) {
	uint16_t i = 0;
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
	
	while (status != TWIR_ADDR_ACK) { // Espera el ACK de la direcci�n en modo lectura
		i++;
		if (i >= TWI_TIMEOUT) {       // Verifica si se ha superado el tiempo de espera
			return TWI_ERROR_START;   // Retorna un error si se supera el timeout
		}
	}
	return TWI_OK; // Retorna �xito si se recibi� el ACK
}


// Funci�n para leer un dato del bus TWI, con opci�n de enviar ACK o NACK
static uint8_t twi_data_read_ack(uint8_t ack) {
    uint16_t i = 0;
    if (ack != 0) {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA); // Env�a ACK despu�s de leer el dato
        while (status != TWIR_DATA_ACK) { // Espera el ACK del dato recibido
            i++;
            if (i >= TWI_TIMEOUT) {      // Verifica si se ha superado el tiempo de espera
                return TWI_ERROR_START;  // Retorna un error si se supera el timeout
            }
        }
    } else {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE); // Env�a NACK despu�s de leer el dato
        while (status != TWIR_DATA_NACK) { // Espera el NACK del dato recibido
            i++;
            if (i >= TWI_TIMEOUT) {      // Verifica si se ha superado el tiempo de espera
                return TWI_ERROR_START;  // Retorna un error si se supera el timeout
            }
        }
    }
    return TWI_OK; // Retorna �xito si se recibi� el ACK o NACK seg�n lo esperado
}


// Funci�n para leer datos desde un dispositivo esclavo a trav�s del bus TWI
uint8_t twi_read(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len) {
	uint16_t i = 0;
	uint8_t err = TWI_OK;
	
	err = twi_start(); // Env�a la condici�n START
	if (err != TWI_OK) {
		twi_stop();
		return err;
	}
	TWDR = (addr << 1) | 0; // Env�a la direcci�n del esclavo con bit de escritura (0)
	
	err = twi_addr_write_ack(); // Espera el ACK de la direcci�n enviada
	if (err != TWI_OK) {
		twi_stop();
		return err;
	}
	
	TWDR = reg; // Env�a el registro que se desea leer
	err = twi_data_write_ack(); // Espera el ACK del dato enviado
	if (err != TWI_OK) {
		twi_stop();
		return err;
	}
	
	err = twi_restart(); // Env�a la condici�n RESTART
	if (err != TWI_OK) {
		twi_stop();
		return err;
	}
	
	TWDR = (addr << 1) | 1; // Env�a la direcci�n del esclavo con bit de lectura (1)
	err = twi_addr_read_ack(); // Espera el ACK de la direcci�n enviada
	if (err != TWI_OK) {
		twi_stop();
		return err;
	}
	
	for (i = 0; i < (len - 1); i++) {
		err = twi_data_read_ack(1); // Lee el dato y env�a ACK para continuar
		if (err != TWI_OK) {
			twi_stop();
			return err;
		}
		data[i] = TWDR; // Almacena el dato le�do
	}
	
	err = twi_data_read_ack(0); // Lee el �ltimo dato y env�a NACK para finalizar
	if (err != TWI_OK) {
		twi_stop();
		return err;
	}
	data[len - 1] = TWDR; // Almacena el �ltimo dato le�do
	
	twi_stop(); // Env�a la condici�n STOP
	return TWI_OK; // Retorna �xito si la operaci�n se complet� sin errores
}


// Funci�n para escribir datos en un dispositivo esclavo a trav�s del bus TWI
uint8_t twi_wire(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len) {
    uint16_t i = 0;
    uint8_t err = TWI_OK;
    
    err = twi_start(); // Env�a la condici�n START
    if (err != TWI_OK) {
        twi_stop();
        return err;
    }
    
    TWDR = (addr << 1) | 0; // Env�a la direcci�n del esclavo con bit de escritura (0)
    
    err = twi_addr_write_ack(); // Espera el ACK de la direcci�n enviada
    if (err != TWI_OK) {
        twi_stop();
        return err;
    }
    
    TWDR = reg; // Env�a el registro donde se desea escribir
    err = twi_data_write_ack(); // Espera el ACK del dato enviado
    if (err != TWI_OK) {
        twi_stop();
        return err;
    }
    
    for (i = 0; i < len; i++) {
        TWDR = data[i]; // Env�a el dato a escribir
        err = twi_data_write_ack(); // Espera el ACK del dato enviado
        if (err != TWI_OK) {
            twi_stop();
            return err;
        }
    }
    
    twi_stop(); // Env�a la condici�n STOP
    return TWI_OK; // Retorna �xito si la operaci�n se complet� sin errores
}


void twi_init(uint32_t speed){
	
	uint32_t gen_t = 0;
	gen_t = (((F_CPU/speed) - 16) / 2) & 0xFF;
	TWBR = gen_t & 0xFF;
	TWCR = (1 << TWEN) | (1 << TWIE);
	//PORTC |= (1 << PORTC5 | 1 << PORTC4);
	
}