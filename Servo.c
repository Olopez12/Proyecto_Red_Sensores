/*
 * Servo.c
 *
 * Created: 13/08/2024 23:20:23
 *  Author: Usuario
 */ 

#include "Servo.h"

void timer1_init(void) {
	// Configurar el pin OC1A (PB1) como salida
	DDRB |= (1 << PINB1);

	// Configurar Timer1 en modo Fast PWM con ICR1 como top
	TCCR1A |= (1 << WGM11) | (1 << COM1A1); // Modo 14, Clear OC1A on Compare Match, set OC1A at BOTTOM
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler de 8

	// Configurar ICR1 para un periodo de 20ms (50Hz) (F_CPU/(Prescaler * F_PWM) - 1)
	ICR1 = 39999; // 16MHz / (8 * 50Hz) - 1 = 39999

	// Iniciar el servomotor en ángulo 0° (duty cycle de 1ms)
	OCR1A = 1999; // 1ms * 50Hz = 1999
}



// Función para mover el servomotor a 0° o 90°
void mover_servomotor(uint8_t angulo) {
	if (angulo == 0) {
		OCR1A = 1999; // 1ms para 0°
		} else if (angulo == 90) {
		OCR1A = 4999; // 2ms para 90°
	}
}