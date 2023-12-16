/*
 * API_debounce.h
 *
 *  Created on: Nov 8, 2023
 *      Author: cbiale
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include <stdbool.h> // por uso de bool_t
#include "API_delay.h" // por uso de retardos
#include "API_uart.h" // por uso de funciones de UART

/**
 * @brief Inicializa la maquina de estados finito.
 *        Nota: se debe llamar antes que debounceFSM_update() o readKey().
 *
 * @return void.
 */
void debounceFSM_init();

/**
 * @brief Actualiza la maquina de estados finito.
 *
 * @return void.
 */
void debounceFSM_update();

/**
 * @brief Lee una variable interna del módulo y devolver true o false si la 
   tecla fue presionada. Si devuelve true, resetea (poner en false) el estado de
  la variable.
 *
 * @return bool_t donde especifica si la tecla fue presionada o no..
 */
bool_t readKey();

#endif /* API_INC_API_DEBOUNCE_H_ */
