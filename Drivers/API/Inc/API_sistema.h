/*
 * API_sistema.h
 *
 *  Created on: Dec 6, 2023
 *      Author: cbiale
 */

#ifndef API_INC_API_SISTEMA_H_
#define API_INC_API_SISTEMA_H_

#include <API_interfaz.h>
#include <API_sensor_temperatura.h>
#include "API_debounce.h"
#include "API_delay.h"
#include "API_lcd.h"
#include "API_uart.h"
#include "port.h"


/**
 * @brief Inicia el sistema.
 *
 * Esta función se encarga de realizar las operaciones necesarias para iniciar los perifericos del sistema.
 *
 * @return void
 *
 * @note Asegúrate de llamar a esta función antes de comenzar cualquier operación en el sistema.
 *
 * Ejemplo de uso:
 * @code
 * sistema_iniciar();
 * @endcode
 */
void sistema_iniciar();

/**
 * @brief Actualiza el estado del sistema.
 *
 * Realiza las operaciones necesarias para actualizar el estado del sistema.
 * Verifica si se debe leer el sensor, actualiza de ser necesario la pantalla y controla el
 * pulsado de botones.
 *
 * @return void
 *
 * Ejemplo de uso:
 * @code
 * sistema_actualizar();
 * @endcode
 */
void sistema_actualizar();

#endif /* API_INC_API_SISTEMA_H_ */
