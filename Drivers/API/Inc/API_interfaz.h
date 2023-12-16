/*
 * API_interfaz.h
 *
 *  Created on: Dec 6, 2023
 *      Author: cbiale
 */

#ifndef API_INC_API_INTERFAZ_H_
#define API_INC_API_INTERFAZ_H_

#include <API_sensor_temperatura.h>
#include <stdint.h>
#include "stdbool.h"
#include "API_debounce.h"
#include "API_lcd.h"

/**
 * @brief Inicia la interfaz de usuario.
 *
 * Esta función se encarga de realizar las operaciones necesarias para iniciar la interfaz de usuario.
 *
 * @return void
 *
 * @note Asegúrate de llamar a esta función antes de comenzar cualquier operación en la interfaz de usuario.
 * @note Debe llamarse luego de inicializar los periféricos de la aplicación.
 *
 * Ejemplo de uso:
 * @code
 * interfaz_iniciar();
 * @endcode
 */
void interfaz_iniciar();

/**
 * @brief Actualiza la interfaz de usuario.
 *
 * Realiza las operaciones necesarias para actualizar la interfaz de usuario. Esto incluye la actualización
 * de elementos visuales.
 *
 * @return void
 * Ejemplo de uso:
 * @code
 * interfaz_actualizar();
 * @endcode
 */
void interfaz_actualizar();

#endif /* API_INC_API_INTERFAZ_H_ */
