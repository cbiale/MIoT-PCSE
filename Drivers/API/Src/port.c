/*
 * port.c
 *
 *  Archivo que implementa las funciones que abstraen a las bibliotecas de la HAL.
 *  Permite portabilidad.
 *
 *  Created on: Dec 3, 2023
 *      Author: cbiale
 */

#include "port.h"

/* --------------------------------- Constantes --------------------------------- */

// constantes para manejo de leds
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB

// Variables privadas que manejan perifericos
static I2C_HandleTypeDef hi2c1; // manejador de I2C1
static ADC_HandleTypeDef hadc1; // manejador de ADC1

/* ---------------------------------- Funciones --------------------------------- */

// Función que detiene la ejecución del programa en caso de error.
void manejar_errores(void) {
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
	__disable_irq();
	while (1) {
	}
}

// Inicializa el módulo ADC.
void ADC_iniciar() {
	ADC_ChannelConfTypeDef sConfig = { 0 };
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		manejar_errores();
	}


	/* Sensor de temperatura interno */
	 sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
	 sConfig.Rank = 1;
	 sConfig.SamplingTime = ADC_SAMPLETIME_112CYCLES;
	 if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		 manejar_errores();
	 }
}

// obtener datos del ADC
uint32_t ADC_obtener_datos() {
	uint32_t valor;

	HAL_ADC_Start(&hadc1);
	// Espera hasta que la conversión esté completa
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	// Obtiene el valor del ADC
	valor = HAL_ADC_GetValue(&hadc1);
	// Detiene el ADC
	HAL_ADC_Stop(&hadc1);
	return valor;
}

// Inicializa el módulo I2C.
void I2C_iniciar(void) {
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		manejar_errores();
	}

	/** Configura el filtro analogico
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		manejar_errores();
	}

	/** Configura el filtro digital
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		manejar_errores();
	}
}

// Transmite un dato a través del bus I2C.
void I2C_transmitir(uint16_t direccion, uint8_t *dato, uint8_t tamano,
		uint8_t tiempo_espera) {
	if (HAL_I2C_Master_Transmit(&hi2c1, direccion << 1, dato, tamano,
			tiempo_espera) != HAL_OK) {
		manejar_errores();
	}
}

// Lee un dato a través del bus I2C.
void I2C_recibir(uint16_t direccion, uint8_t *dato, uint8_t tamano,
		uint8_t tiempo_espera) {
	if (HAL_I2C_Master_Receive(&hi2c1, direccion << 1, dato, tamano,
			tiempo_espera) != HAL_OK) {
		manejar_errores();
	}
}

// lee datos usando el bus I2C
void I2C_leer_memoria(uint16_t direccion, uint16_t direccion_memoria,
		uint8_t *dato, uint8_t tamano, uint8_t tiempo_espera) {
	if (HAL_I2C_Mem_Read(&hi2c1, direccion_memoria, direccion << 1, 1, dato,
			tamano, tiempo_espera) != HAL_OK) {
		manejar_errores();
	}
}

// Espera durante un período de tiempo especificado en milisegundos
void esperar(uint32_t tiempo) {
	HAL_Delay(tiempo);
}

// espera en tienpos especificados en ms
void esperar_ms(uint32_t microsegundos) {
	uint32_t clk_ciclo_inicio = DWT->CYCCNT;
	// HAL_RCC_GetHCLKFreq devuelve la frecuencia del reloj AHB (Advanced High-Performance Bus)
	microsegundos *= (HAL_RCC_GetHCLKFreq() / 1000000);
	// Se obtiene la cantidad de ciclos de reloj y se la resta con el ciclo
	// de inicio para verificar si han pasado los microsengundos solicitados
	while ((DWT->CYCCNT - clk_ciclo_inicio) < microsegundos) {
	}
}
