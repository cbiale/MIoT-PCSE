/*
 * API_uart.c
 *
 *  Created on: Nov 16, 2023
 *      Author: cbiale
 */


#include "API_uart.h"

/*
 * Variable que mantiene el manejador de la UART
 */
static UART_HandleTypeDef UartHandle;

bool_t uart_iniciar() {

	// retorna el estado del inicio de la UART
	bool_t estado = false;
	// variable usada para imprimir datos de la UART
    char cadena[100];

	UartHandle.Instance = USART3;
	UartHandle.Init.BaudRate = 115200;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	// si se inicia la UART, cambia estado e imprime datos de la UART
	if (HAL_UART_Init(&UartHandle) == HAL_OK) {
		estado = true;
	    snprintf(cadena, sizeof(cadena), "Valores de configuración de USART:\r\n");
	    uart_enviar_string((uint8_t *) cadena);
	    snprintf(cadena, sizeof(cadena), "Velocidad en baudios: %" PRIu32 " \r\n", UartHandle.Init.BaudRate);
	    uart_enviar_string((uint8_t *) cadena);
	    snprintf(cadena, sizeof(cadena), "Largo de palabra: %" PRIu32 " \r\n", UartHandle.Init.WordLength);
	    uart_enviar_string((uint8_t *) cadena);
	    snprintf(cadena, sizeof(cadena), "Bits de parada: %" PRIu32 " \r\n", UartHandle.Init.StopBits);
	    uart_enviar_string((uint8_t *) cadena);
	    snprintf(cadena, sizeof(cadena), "Paridad: %" PRIu32 " \r\n", UartHandle.Init.Parity);
	    uart_enviar_string((uint8_t *) cadena);
	    snprintf(cadena, sizeof(cadena), "Modo: %" PRIu32 " \r\n", UartHandle.Init.Mode);
	    uart_enviar_string((uint8_t *) cadena);
	    snprintf(cadena, sizeof(cadena), "Control de flujo de hardware: %" PRIu32 " \r\n", UartHandle.Init.HwFlowCtl);
	    uart_enviar_string((uint8_t *) cadena);
	    snprintf(cadena, sizeof(cadena), "Sobre muestreo 8: %" PRIu32 " \r\n", UartHandle.Init.OverSampling);
	    uart_enviar_string((uint8_t *) cadena);
	}
    return estado;
}

void uart_enviar_string(uint8_t * pstring) {
	// control defensivo
	if (pstring == NULL) {
		return;
	}
	// transmite
	HAL_UART_Transmit(&UartHandle, pstring, strlen((char *)pstring), HAL_MAX_DELAY);
}

void uart_enviar_string_tamano (uint8_t * pstring, uint16_t size) {
	// control defensivo
	if (pstring == NULL) {
		return;
	}
	// obtengo largo de pstring
	uint16_t strSize = strlen((char *) pstring);
	// si el largo es menor al argumento size
	if (strSize < size) {
		// el argumento size toma el valor de strSize
		// esto es para que no imprima "basura"
		size = strSize;
	}
	// no es necesario hacer un control si strSize > size
	// debido a que como size en HAL_UART_Transmit() esa es la
	// cantidad a transmitir y el sobrante se descarta

	// transmite
    HAL_UART_Transmit(&UartHandle, pstring, size, HAL_MAX_DELAY);
}

void uart_recibir_string_tamano (uint8_t * pstring, uint16_t size) {
	// recepciona datos
	HAL_UART_Receive(&UartHandle, pstring, size, HAL_MAX_DELAY);
}

void uart_actualizar() {

}
