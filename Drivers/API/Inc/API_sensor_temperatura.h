/*
 * API_sensor_temperatura.h
 *
 *  Created on: Dec 3, 2023
 *      Author: cbiale
 */

#ifndef API_INC_API_SENSOR_TEMPERATURA_H_
#define API_INC_API_SENSOR_TEMPERATURA_H_

#include "port.h"
#include "API_delay.h"
#include "API_uart.h"
#include "stdbool.h"


/**
 * @brief Inicia el sensor de temperatura.
 *
 * Realiza las operaciones necesarias para iniciar el sensor de temperatura.
 *
 * @return void
 */
void sensor_temperatura_iniciar();

/**
 * @brief Obtiene los datos del sensor y deja disponible los valores obtenidos.
 *
 * Realiza las operaciones necesarias para obtener los datos del sensor y
 * actualiza las variables que contienen los datos de temperatura.
 *
 * @return void
 */
void sensor_temperatura_actualizar ();

/**
 * @brief Obtiene la temperatura actual desde el sensor.
 *
 * Retorna la temperatura actual medida por el sensor.
 *
 * @return Temperatura actual en grados Celsius (float).
 */
float sensor_temperatura_obtener_temperatura();


/**
 * @brief Obtiene la secuencia de lectura del sensor.
 *
 * Retorna la secuencia de lectura del sensor, se usa para controlar si hay una nueva lectura.
 *
 * @return Secuencia de lectura actual.
 */
uint8_t sensor_temperatura_nueva_lectura();

#endif /* API_INC_API_SENSOR_TEMPERATURA_H_ */
