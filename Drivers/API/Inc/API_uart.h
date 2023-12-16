/*
 * API_uart.h
 *
 *  Created on: Nov 16, 2023
 *      Author: cbiale
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include <stdbool.h>  // por uso de bool_t
#include <string.h>   // por uso de strlen()
#include <stdio.h>    // por uso de snprintf()
#include <inttypes.h> // para imprimir valores uint_t al usar snprintf()
#include "port.h"     // manejador de errores

/**
 * @brief Tipo de datos que define un valor lógico.
 */
typedef bool bool_t;

/**
 * @brief Inicia la UART
 *
 * @param Ninguno
 * @retval Ninguno
 */
bool_t uart_iniciar();

/**
 * @brief Transfiere una cadena de texto
 *
 * @param Puntero a la cadena de texto a transferir
 *
 * @retval void
 */
void uart_enviar_string (uint8_t * pstring);

/**
 * @brief Transfiere una cadena de texto
 *
 * @param Puntero a la cadena de texto a transferir
 * @param Largo máximo que puede tener la cadena de texto a transferir
 *
 * @retval void
 */
void uart_enviar_string_tamano(uint8_t * pstring, uint16_t size);

/**
 * @brief Recepciona datos y los almacena en una cadena
 *
 * @param Puntero donde se almacenan los datos recepcionados
 * @param Largo máximo de datos a recepcionar
 *
 * @retval void
 */
void uart_recibir_string_tamano(uint8_t * pstring, uint16_t size);

#endif /* API_INC_API_UART_H_ */
