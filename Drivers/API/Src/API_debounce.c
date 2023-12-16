/*
 * API_debounce.c
 *
 *  Created on: Nov 8, 2023
 *      Author: cbiale
 */
#include "API_debounce.h"

/*
 * Definiciones para acceso al botón y led 1
 */
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC

/*
 * Enumeración que define los estados posibles
 */
typedef enum
{
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RISING
} debounceState_t;

/*
 * El tiempo de anti-rebote debe ser de 40 ms con un retardo no bloqueante como los
 * implementados en la práctica 3.
 */
const uint32_t duracionRetardo = 40u;
/*
 * Variable que mantiene el estado de la MEF
 */
static debounceState_t estado;
/*
 * Retardo usado para en control anti-retote
 */
static delay_t retardo;
/*
 * Variable interna usada por readKey() la inicializo en este punto
 */
static bool_t estadoReadKey = false;

/*
 * acciones realizadas al presionar el botón
 */
static void buttonPressed() {
	estadoReadKey = true;
	uart_enviar_string((uint8_t *) "Botón presionado\r\n");
}

/*
 * acciones realizadas al liberar el botón
 */
static void buttonReleased() {
	uart_enviar_string((uint8_t *) "Botón liberado\r\n");
}

/*
 * Inicio de la MEF
 */
void debounceFSM_init() {
	// estado inicial
	estado = BUTTON_UP;
	// si no puede iniciar la UART maneja errores y queda en loop
	// para hacer esto disponible se saca static de manejadorErrores() en API_delay.c
	// y se agrega en API_delay.h
	// if (uartInit() == false) {
	// 	manejadorErrores();
	// }
}

/*
 * Actualiza la MEF
 */
void debounceFSM_update() {
	switch (estado) {
	case BUTTON_UP:
		// realiza una lectura del botón y si se encuentra presionado
		if (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) != 0) {
			// cambia de estado
			estado = BUTTON_FALLING;
			// inicia y lee retardo anti-rebote
			delayInit(&retardo, duracionRetardo);
			delayRead(&retardo);
		}
		break;
	case BUTTON_FALLING:
		// verifica que se cumpla el retardo anti-rebote
		if (delayRead(&retardo)) {
			// realiza una lectura del botón y si se encuentra presionado
			if (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) != 0) {
				// cambia de estado
				estado = BUTTON_DOWN;
				// invoca al evento asociado a el nuevo estado
				buttonPressed();
			} else {
				// si no se encuentra presionado el botón vuelve al estado original
				estado = BUTTON_UP;
			}
		}
		break;
	case BUTTON_DOWN:
		// realiza una lectura del botón y si no se encuentra presionado
		if (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == 0) {
			// cambia de estado
			estado = BUTTON_RISING;
			// inicia y lee retardo anti-rebote
			delayInit(&retardo, duracionRetardo);
			delayRead(&retardo);
		}
		break;
	case BUTTON_RISING:
		// verifica que se cumpla el retardo anti-rebote
		if (delayRead(&retardo)) {
			// realiza una lectura del botón y si no se encuentra presionado
			if (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == 0) {
				// cambia de estado
				estado = BUTTON_UP;
				// invoca al evento asociado a el nuevo estado
				buttonReleased();
			} else {
				// si no se encuentra presionado el botón vuelve al estado original
				estado = BUTTON_DOWN;
			}
		}
		break;
	default:
		debounceFSM_init();
		break;
	}

}

/* La función readKey debe leer una variable interna del módulo y devolver true o false si la tecla es presionada.*/
bool_t readKey() {
	bool_t retornar = estadoReadKey;
	estadoReadKey = false;
	return retornar;
}
