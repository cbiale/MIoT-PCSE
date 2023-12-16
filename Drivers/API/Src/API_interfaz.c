/*
 * M_interfaz.c
 *
 *  Created on: Dec 6, 2023
 *      Author: cbiale
 */

#include <API_interfaz.h>

/* ------------------------------- tipos de datos ------------------------------- */

/**
 * @brief Enumeración que representa los estados de la interfaz.
 *
 * Define los posibles estados que incluyen "MOSTRANDO_DATOS" y "DESCANSO".
 */
typedef enum {
	MOSTRANDO_DATOS, DESCANSO
} estado_sistema_t;

/**
 * @brief Estructura que representa el estado interno de la interfaz en el sistema.
 *
 * Contiene información relacionada con los últimos valores de temperatura y humedad obtenidos.
 * Por otro lado, contiene un valor que determina la secuencia de lectura obtenida.
 */
typedef struct {
	uint8_t lectura_anterior;
	float temperatura;
} estado_interno_t;

/* --------------------------------- Constantes --------------------------------- */

#define SIN_DATOS -1

/* ----------------------------- Variables privadas ----------------------------- */

static estado_sistema_t estado;
static estado_interno_t estado_interno;

/* ---------------------- Prototipos de funciones privadas ---------------------- */

static void iniciar_mostrar_datos();
static void mostrar_datos(bool mostrar_lcd, bool mostrar_uart,
		bool forzar_lectura);
static void mostrar_datos_lcd();
static void mostrar_datos_uart();
static void descansar();

/* ---------------------------------- Funciones --------------------------------- */

// inicializa la interfaz de usuario en el estado que muestra datos en el lcd y por la uart
void interfaz_iniciar() {
	estado = MOSTRANDO_DATOS;
	estado_interno.lectura_anterior = 0;
	iniciar_mostrar_datos();
}

/* Actualiza la interfaz de usuario
 *
 *  En el estado MOSTRANDO_DATOS se visualizan los valores de temperatura y humedad
 *  en el lcd y se pasan datos por la UART.
 *  En el estado DESCANSO solo se pasan datos por la UART.
 *
 *  El cambio de estados se produce al pulsar el boton USER de la placa.
 *
 */
void interfaz_actualizar() {
	switch (estado) {
	case MOSTRANDO_DATOS:
		if (readKey()) {
			estado = DESCANSO;
			descansar();
		} else {
			mostrar_datos(true, true, false);
		}
		break;
	case DESCANSO:
		if (readKey()) {
			estado = MOSTRANDO_DATOS;
			iniciar_mostrar_datos();
			mostrar_datos(true, false, true);
		} else {
			mostrar_datos(false, true, false);
		}
		break;
	default:
		estado = DESCANSO;
	}
}

/* ------------------------------ Funciones privadas ---------------------------- */


/**
 * @brief Muestra los datos según las opciones especificadas.
 *
 * Esta función muestra los datos en dispositivos como LCD y UART, según las opciones proporcionadas.
 * También permite forzar una nueva lectura si se especifica.
 *
 * @param mostrar_lcd Indica si se deben mostrar los datos en un LCD.
 * @param mostrar_uart Indica si se deben mostrar los datos a través de UART.
 * @param forzar_lectura Indica si se debe forzar una nueva lectura antes de mostrar los datos.
 *
 * @return void
 */
static void mostrar_datos(bool mostrar_lcd, bool mostrar_uart,
		bool forzar_lectura) {
	// verifica si hay una nueva lectura
	uint8_t nueva_lectura = sensor_temperatura_nueva_lectura();
	// si hay una nueva lectura o forzar en true
	if (nueva_lectura != estado_interno.lectura_anterior || forzar_lectura) {
		estado_interno.lectura_anterior = nueva_lectura;
		estado_interno.temperatura = sensor_temperatura_obtener_temperatura();
		if (mostrar_lcd) {
			mostrar_datos_lcd();
		}
		if (mostrar_uart) {
			mostrar_datos_uart();
		}
	}
}

/**
 * @brief Muestra los datos en un LCD.
 *
 * Esta función se encarga de mostrar los datos en un LCD.
 *
 * @return void
 */
static void mostrar_datos_lcd() {
	char cadena[100];

	lcd_ubicar(0, 2);
	if (SIN_DATOS == estado_interno.temperatura) {
		sprintf(cadena, "s/d           ");
	} else {
		sprintf(cadena, "%2.2f         ", estado_interno.temperatura);
	}
	lcd_escribir_string(cadena);
}

/**
 * @brief Muestra los datos a través de UART.
 *
 * Esta función se encarga de mostrar los datos a través de la interfaz UART.
 *
 * @return void
 */
static void mostrar_datos_uart() {
	char cadena[100];
	sprintf(cadena, "Temperatura: %2.2f\r\n",
			estado_interno.temperatura);
	uart_enviar_string((uint8_t*) cadena);
}

/**
 * @brief Muestra los datos iniciales en el lcd en el estado MOSTRANDO_DATOS.
 *
 * Esta función muestra los datos iniciales en el lcd, los cuales no cambian constantemente.
 * Se usa en el estado MOSTRANDO_DATOS.
 *
 * @return void
 */
static void iniciar_mostrar_datos() {
	lcd_limpiar();
	lcd_luz_fondo(true);
	lcd_ubicar(0, 0);
	lcd_escribir_string("T: ");
}


/**
 * @brief Muestra los datos que se visualizan en el lcd en el estado DESCANSAR.
 *
 * Esta función muestra los datos que se visualizan en el lcd en el estado DESCANSAR.
 *
 * @return void
 */
static void descansar() {
	lcd_limpiar();
	lcd_luz_fondo(true);
	lcd_ubicar(0, 0);
	lcd_escribir_string("Descansando");
	lcd_ubicar(1, 0);
	lcd_escribir_string("Datos por UART");
}
