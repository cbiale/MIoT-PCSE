/*
 * port.h
 *
 *  Archivo que abstrae a las bibliotecas de la HAL, permiten portabilidad.
 *
 *  Created on: Dec 3, 2023
 *      Author: cbiale
 */

#ifndef API_INC_PORT_H_
#define API_INC_PORT_H_

#include "stm32f4xx_hal.h" // hal que voy a usar en el proyecto, permite portabilidad


/**
 * @brief Inicializa el módulo ADC.
 *
 * Esta función realiza las operaciones necesarias para iniciar el módulo ADC.
 *
 * @return void
 */
void ADC_iniciar();

/**
 * @brief Obtiene datos del módulo ADC.
 *
 * Esta función realiza las operaciones necesarias para obtener datos del módulo ADC.
 *
 * @return Valor obtenido del módulo ADC.
 */
uint32_t ADC_obtener_datos();

/**
 * @brief Inicializa el módulo I2C.
 *
 * Esta función realiza las operaciones necesarias para iniciar el módulo I2C.
 *
 * @return void
 */
void I2C_iniciar();


/**
 * @brief Transmite un dato a través del bus I2C.
 *
 * Esta función envía el 'dato' proporcionado a través del bus I2C a la 'dirección' especificada.
 * El parámetro 'tamano' indica la cantidad de bytes a transmitir, y 'tiempoEspera' representa
 * el tiempo que la función debería esperar después de la transmisión para permitir
 * que el bus I2C se estabilice o realizar otras tareas necesarias.
 *
 * @param direccion La dirección de destino.
 * @param dato El dato que se va a transmitir a través del bus I2C.
 * @param tamano La cantidad de bytes a transmitir.
 * @param tiempo_espera El tiempo de espera después de la transmisión.
 *
 * @return void
 */
void I2C_transmitir (uint16_t direccion, uint8_t * dato, uint8_t tamano, uint8_t tiempo_espera);


/**
 * @brief Transmite un dato a través del bus I2C.
 *
 * Esta función recibe en 'dato' datos a través del bus I2C de la 'dirección' especificada.
 * El parámetro 'tamano' indica la cantidad de bytes a recibir.
 *
 * @param direccion La dirección de lectura.
 * @param dato Almacena el dato que se recibe a través del bus I2C.
 * @param tamano La cantidad de bytes a recibir.
 * @param tiempo_espera El tiempo de espera después de la transmisión.
 *
 * @return void
 */
void I2C_recibir(uint16_t direccion, uint8_t * dato, uint8_t tamano, uint8_t tiempo_espera);

/**
 * @brief Lee una dirección de memoria usando el bus I2C.
 *
 * Esta función recibe en 'dato' datos de una dirección de memoria a través del bus I2C del
 * dispositivo con la 'dirección' especificada.
 * El parámetro 'tamano' indica la cantidad de bytes a recibir.
 *
 * @param direccion La dirección del dispositivo.
 * @param direccion_memoria La direccion de memoria dentro del dispositivo.
 * @param dato Almacena el dato que se recibe a través del bus I2C.
 * @param tamano La cantidad de bytes a recibir.
 * @param tiempo_espera El tiempo de espera después de la transmisión.
 *
 * @return void
 */
void I2C_leer_memoria(uint16_t direccion, uint16_t direccion_memoria, uint8_t *dato, uint8_t tamano,
		uint8_t tiempo_espera);
/**
 * @brief Espera durante un período de tiempo especificado.
 *
 * Esta función realiza una espera durante el tiempo especificado.
 * La unidad de tiempo utilizada es de milisegundos.
 *
 * @param tiempo El tiempo a esperar.
 *
 * @return void
 */
void esperar (uint32_t tiempo);

/**
 * @brief Función que detiene la ejecución del programa en caso de error.
 *
 * Esta función se llama cuando se detecta una situación de error en el programa.
 * En caso de detectarse un error se detiene la ejecución del programa.
 *
 * @return void
 */
void manejar_errores(void);

/**
 * @brief Espera durante un período de tiempo especificado.
 *
 * Esta función realiza una espera durante el tiempo especificado.
 * La unidad de tiempo utilizada es de microsegundos.
 *
 * @param microsegundos El tiempo a esperar.
 *
 * @return void
 */
void esperar_ms (uint32_t microsegundos);

#endif /* API_INC_PORT_H_ */
