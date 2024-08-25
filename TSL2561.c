/*
 * TSL2561.c
 *
 * Created: 20/08/2024 17:11:24
 *  Author: Usuario
 */ 

#include "TSL2561.h"

// Función para inicializar el sensor TSL2561
void TSL2561_init() {
	uint8_t control = 0x03; // El valor 0x03 enciende el sensor configurando el registro de control.
	
	// Enviar el comando para escribir en el registro de control del TSL2561.
	// La dirección del TSL2561 se combina con el bit de comando y la dirección del registro de control.
	twi_wire(TSL2561_ADDRESS, TSL2561_COMMAND_BIT | TSL2561_CONTROL_REG, &control, 1);
}

// Función para leer el valor de luz del TSL2561
uint16_t TSL2561_read_lux() {
	uint8_t data0_low, data0_high, data1_low, data1_high; // Variables para almacenar los bytes leídos.
	uint16_t ch0, ch1; // Variables para almacenar los valores de los canales 0 y 1.

	// Leer el byte bajo del canal 0 (canal de luz visible + IR).
	twi_read(TSL2561_ADDRESS, TSL2561_COMMAND_BIT | TSL2561_DATA0LOW, &data0_low, 1);
	
	// Leer el byte alto del canal 0.
	twi_read(TSL2561_ADDRESS, TSL2561_COMMAND_BIT | TSL2561_DATA0HIGH, &data0_high, 1);
	
	// Combinar los bytes alto y bajo para obtener el valor completo del canal 0.
	ch0 = (data0_high << 8) | data0_low;

	// Leer el byte bajo del canal 1 (canal de luz IR).
	twi_read(TSL2561_ADDRESS, TSL2561_COMMAND_BIT | TSL2561_DATA1LOW, &data1_low, 1);
	
	// Leer el byte alto del canal 1.
	twi_read(TSL2561_ADDRESS, TSL2561_COMMAND_BIT | TSL2561_DATA1HIGH, &data1_high, 1);
	
	// Combinar los bytes alto y bajo para obtener el valor completo del canal 1.
	ch1 = (data1_high << 8) | data1_low;

	// Calcular el valor de la luz usando una fórmula simplificada (ch0 - ch1).
	uint16_t lux = ch0 - ch1;
	return lux; // Devolver el valor calculado de luz.
}