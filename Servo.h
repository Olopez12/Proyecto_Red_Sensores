/*
 * Servo.h
 *
 * Created: 13/08/2024 23:22:44
 *  Author: Usuario
 */ 


#ifndef SERVO_H_
#define SERVO_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

void timer1_init(void);
// Función para mover el servomotor a 0° o 90°
void mover_servomotor(uint8_t angulo);
#endif /* SERVO_H_ */