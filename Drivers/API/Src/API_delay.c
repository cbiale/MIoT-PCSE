/*
 * API_delay.c
 *
 *  Created on: Nov 1, 2023
 *      Author: cbiale
 */

#include "API_delay.h"



void delayInit( delay_t * delay, tick_t duration ) {
	// se comprueba variable existente
	if (delay == NULL) {
		manejar_errores();
	} else {
		delay->duration = duration;
		delay->running = false;
		delay->startTime = 0; // siempre inicializar con valores
	}
}

bool_t delayRead( delay_t * delay ) {
	// varible que se usa para tener un unico punto de retorno
	bool resultado = false;
	if (delay == NULL) {
		manejar_errores();
	} else {
		// no es necesario comparar explicitamente, pero es simple de leer
		if (delay->running == false) {
			delay->startTime = HAL_GetTick();
			delay->running = true;
		} else {
			if (HAL_GetTick() - delay->startTime >= delay->duration) {
				delay->running = false;
				resultado = true;
			}
		}
	}
	return resultado;
}

void delayWrite( delay_t * delay, tick_t duration ) {
	if (delay == NULL) {
		manejar_errores();
	} else {
		delay->duration = duration;
	}
}
