#include <API_sensor_temperatura.h>

/* ------------------------------- tipos de datos ------------------------------- */

/**
 * @brief Enumeración que representa los estados del sensor de temperatura.
 *
 * Define los posibles estados del sensor ficticio, que incluyen "ESPERA" y "LECTURA".
 */
typedef enum {
	ESPERA,      ///< Estado de espera del sensor de temperatura.
	LECTURA      ///< Estado de lectura del sensor de temperatura.
} estado_sensor_temperatura_t;

/**
 * @brief Estructura que representa el estado interno del sensor en el sistema.
 *
 * Contiene información relacionada con la temperatura, una bandera de nueva lectura y un índice de siguiente valor a leer.
 */
typedef struct {
	float temperatura;       ///< Temperatura actual del sensor.
	uint8_t nueva_lectura; ///< Bandera (en formato índice) que indica si se realizó una nueva lectura.
} estado_interno_t;

/* --------------------------------- Constantes --------------------------------- */

#define SENSOR_TEMPERATURA_CONTROL_LECTURAS      5      // mayor valor de contador de lecturas realizadas
#define SENSOR_TEMPERATURA_ESPERA_ENTRE_LECTURAS 3000   // espera entre lecturas del sensor
#define SENSOR_TEMPERATURA_PENDIENTE_PROMEDIO    0.0025 // valor para calcular la temperatura obtenida por el ADC
#define SENSOR_TEMPERATURA_VOLTAJE_25_GRADOS     0.76   // voltaje a los 25 grados celcius
#define SENSOR_TEMPERATURA_ADC_MAXIMO            4095   // mayor valor posible de ADC
#define SENSOR_TEMPERATURA_VOLTAJE_REFERENCIA    3.3    // voljate de referencia tipico

/* ----------------------------- Variables privadas ----------------------------- */

/**
 * @brief Estado actual del sensor de temperatura.
 *
 * Representa el estado actual del sensor de temperatura en la aplicación.
 */
static estado_sensor_temperatura_t estado;

/**
 * @brief Estado interno actual del sensor de temperatura.
 *
 * Representa el estado interno del sensor de temperatura en la aplicación.
 */
static estado_interno_t estado_interno;

/**
 * @brief Maneja la espera entre lecturas del sensor.
 *
 */
static delay_t espera;

/* ---------------------- Prototipos de funciones privadas ---------------------- */

// Realiza una lectura del sensor de temperatura.
static void sensor_temperatura_realizar_lectura();

/* ---------------------------------- Funciones --------------------------------- */

// inicia el sensor de temperatura
void sensor_temperatura_iniciar() {
	estado_interno.temperatura = -1.0;
	estado_interno.nueva_lectura = -1;
	estado = LECTURA;
}

// actualiza el estado del sensor de temperatura
void sensor_temperatura_actualizar() {
	switch (estado) {
	case LECTURA:
		sensor_temperatura_realizar_lectura();
		estado = ESPERA;
		delayInit(&espera, SENSOR_TEMPERATURA_ESPERA_ENTRE_LECTURAS);
		if (estado_interno.nueva_lectura < SENSOR_TEMPERATURA_CONTROL_LECTURAS) {
			estado_interno.nueva_lectura++;
		} else {
			estado_interno.nueva_lectura = 0;
		}
		break;
	case ESPERA:
		if (delayRead(&espera)) {
			estado = LECTURA;
		}
		break;
	default:
		estado = ESPERA;
	}
}

// retorna el valor actual de temperatura
// permite que se obtenga de forma externa el valor de temperatura
float sensor_temperatura_obtener_temperatura() {
	return estado_interno.temperatura;
}

// retorna si ha realizado una nueva lectura
uint8_t sensor_temperatura_nueva_lectura() {
	return estado_interno.nueva_lectura;
}

/* ------------------------------ Funciones privadas ---------------------------- */

/**valor
 * @brief Realiza una lectura del sensor de temperatura.
 *
 * Esta obtiene usando ADC una lectura del sensor de temperatura.
 *
 * @return void
 */
static void sensor_temperatura_realizar_lectura() {
	uint32_t valor = ADC_obtener_datos();
	estado_interno.temperatura = valor;
	// aproximadamente debe dar 944 con 25 grados celcius
	float voltaje_sensor = valor * SENSOR_TEMPERATURA_VOLTAJE_REFERENCIA
			/ SENSOR_TEMPERATURA_ADC_MAXIMO;
	float temperatura_calculada = ((voltaje_sensor
			- SENSOR_TEMPERATURA_VOLTAJE_25_GRADOS)
			/ SENSOR_TEMPERATURA_PENDIENTE_PROMEDIO) + 25;
	estado_interno.temperatura = temperatura_calculada;
}
