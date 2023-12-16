/*
 * API_lcd.h
 *
 *  Archivo que define las funciones que manejan el LCD.
 *
 *  Created on: Nov 29, 2023
 *      Author: cbiale
 */

#ifndef API_INC_API_LCD_H_
#define API_INC_API_LCD_H_

#include <stdint.h>
#include <stdbool.h>
#include <port.h>

/**
 * @brief Inicializa la pantalla del LCD.
 *
 * Esta función envía los comandos necesarios al LCD para poder inicializarlo.
 *
 * @return void
 */
void lcd_iniciar();

/**
 * @brief Limpia la pantalla del LCD.
 *
 * Esta función envía un comando al LCD para limpiar la pantalla y espera
 * un tiempo para asegurarse de que la operación se complete.
 *
 * @return void
 */
void lcd_limpiar();

/**
* @brief Ubica el cursor en una posición específica del LCD.
 *
 * Esta función ubica el cursor en la posición indicada por los parámetros 'fila' y 'columna'
 * del LCD.
 *
 * @param fila Número de fila donde se desea ubicar el cursor.
 * @param columna Número de columna donde se desea ubicar el cursor.
 *
 * @return void
 */
void lcd_ubicar(uint8_t fila, uint8_t columna);

/**
 * @brief Escribe una cadena de texto en el LCD.
 *
 * Esta función recorre cada caracter de la cadena y envía cada caracter como dato al LCD.
 *
 * @param cadena Cadena de caracteres a escribir en el LCD.
 *
 * @return void
 */
void lcd_escribir_string(const char *cadena);

/**
 * @brief Controla el estado de la luz de fondo del LCD.
 *
 * Esta función permite encender o apagar la luz de fondo del LCD según el valor
 * del parámetro 'estado'.
 *
 * @param estado Indica si se debe encender (true) o apagar (false) la luz de fondo.
 *
 * @return void
 */
void lcd_luz_fondo(bool estado);

#endif /* API_INC_API_LCD_H_ */
