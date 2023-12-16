/*
 * API_lcd.c
 *
 *  Implementa las funciones que manejan el LCD.
 *
 *  Created on: Nov 29, 2023
 *      Author: cbiale
 */

#include "API_lcd.h"

/* --------------------------------- Constantes --------------------------------- */

#define LCD_DIRECCION 0x27 // direccion I2C del LCD

#define LCD_FILAS     2 // filas del LCD
#define LCD_COLUMNAS 16 // columnas del LCD

#define LCD_RS_BIT 0 // Register select bit
#define LCD_EN_BIT 2 // Enable bit
#define LCD_BL_BIT 3 // luz de fondo bit
#define LCD_D4_BIT 4 // Datos 4 bit
#define LCD_D5_BIT 5 // Datos 5 bit
#define LCD_D6_BIT 6 // Datos 6 bit
#define LCD_D7_BIT 7 // Datos 7 bit

#define LCD_COMANDO 0         // se transmite un comando
#define LCD_DATOS   1         // se transmite datos

#define LCD_CMD_LIMPIAR           0b00000001 // Limpiar la pantalla
#define LCD_CMD_DDRAM_DIRECCION   0b10000000 // Posicionamiento del cursor

// USados en el inicio del lcd (no se usan todas las funcionalidades en este release)
#define LCD_CMD_INICIO_SET1       0x03
#define LCD_CMD_INICIO_SET2       0x02
#define LCD_CMD_INICIO28          0x28
#define LCD_CMD_INICIO0C          0x0C
#define LCD_CMD_INICIO06          0x06
#define LCD_CMD_INICIO01          0x01

#define LCD_LINEA_1_DIRECCION     0x00 // Direccion de primera linea del lcd
#define LCD_LINEA_2_DIRECCION     0x40 // Direccion de segunda linea del lcd

#define LCD_TIEMPO_ESPERA       100 // tiempo de espera
#define LCD_LARGO_VALORES       1   // largo de valores a transmitir

// constantes para definir estado de luz de fondo
#define LCD_LUZ_FONDO_ENCENDIDA 1
#define LCD_LUZ_FONDO_APAGADA   0

// constantes de espera
#define LCD_ESPERAR_50          50
#define LCD_ESPERAR_1           1
#define LCD_ESPERAR_5           5

// constantes de desplazamiento de datos y manejo de bits
#define LCD_DESPLAZAR_1         1
#define LCD_DESPLAZAR_4         4
#define LCD_VALOR_BINARIO_1111  0x0F


/* ----------------------------- Variables privadas ----------------------------- */

static uint8_t estado_luz_fondo = LCD_LUZ_FONDO_APAGADA;

/* ---------------------- Prototipos de funciones privadas ---------------------- */

static void lcd_escribir_nibble(uint8_t pin, uint8_t valor);
static void lcd_escribir_codigo(uint8_t comando, uint8_t modo);

/* ---------------------------------- Funciones --------------------------------- */

// inicia el lcd
void lcd_iniciar() {
	esperar(LCD_ESPERAR_50);
	lcd_escribir_codigo(LCD_CMD_INICIO_SET1, LCD_COMANDO);
	esperar(LCD_ESPERAR_5);
	lcd_escribir_codigo(LCD_CMD_INICIO_SET1, LCD_COMANDO);
	esperar(LCD_ESPERAR_1);
	lcd_escribir_codigo(LCD_CMD_INICIO_SET1, LCD_COMANDO);
	esperar(LCD_ESPERAR_1);
	lcd_escribir_codigo(LCD_CMD_INICIO_SET2, LCD_COMANDO);
	esperar(LCD_ESPERAR_1);
	lcd_escribir_codigo(LCD_CMD_INICIO28, LCD_COMANDO);
	esperar(LCD_ESPERAR_1);
	lcd_escribir_codigo(LCD_CMD_INICIO0C, LCD_COMANDO);
	esperar(LCD_ESPERAR_1);
	lcd_escribir_codigo(LCD_CMD_INICIO06, LCD_COMANDO);
	esperar(LCD_ESPERAR_1);
	lcd_escribir_codigo(LCD_CMD_INICIO01, LCD_COMANDO);
	esperar(LCD_ESPERAR_1);
}

// ubica el cursor en una fila y columna
void lcd_ubicar(uint8_t fila, uint8_t columna) {
	if (fila > LCD_FILAS || columna > LCD_COLUMNAS) {
		manejar_errores();
	}
	uint8_t direccion;
	switch (fila) {
	case 0:
		direccion = LCD_LINEA_1_DIRECCION;
		break;
	case 1:
		direccion = LCD_LINEA_2_DIRECCION;
		break;
	}
	direccion = direccion + columna;
	lcd_escribir_codigo(LCD_CMD_DDRAM_DIRECCION | direccion, LCD_COMANDO);
	esperar(1);
}

// limpia (clear) la pantalla del lcd.
void lcd_limpiar() {
	lcd_escribir_codigo(LCD_CMD_LIMPIAR, LCD_COMANDO);
	esperar(2);
}

// escribe una cadena de texto en el lcd
void lcd_escribir_string(const char *cadena) {
	while (*cadena) {
		lcd_escribir_codigo(*cadena++, LCD_DATOS);
	}
}

// cambia el estado de luz de fondo del lcd
void lcd_luz_fondo(bool estado) {
	if (estado) {
		estado_luz_fondo = LCD_LUZ_FONDO_ENCENDIDA;
	} else {
		estado_luz_fondo = LCD_LUZ_FONDO_APAGADA;
	}
}

/* ------------------------------ Funciones privadas ---------------------------- */

static void lcd_escribir_nibble(uint8_t valor, uint8_t modo) {
	uint8_t dato = valor << LCD_D4_BIT;
	dato |= modo << LCD_RS_BIT;
	dato |= estado_luz_fondo << LCD_BL_BIT;
	dato |= LCD_DESPLAZAR_1 << LCD_EN_BIT;
	I2C_transmitir(LCD_DIRECCION, &dato, LCD_LARGO_VALORES, LCD_TIEMPO_ESPERA);
	esperar(LCD_ESPERAR_1);
	dato &= ~(LCD_DESPLAZAR_1 << LCD_EN_BIT);
	I2C_transmitir(LCD_DIRECCION, &dato, LCD_LARGO_VALORES, LCD_TIEMPO_ESPERA);
}

static void lcd_escribir_codigo(uint8_t comando, uint8_t modo) {
	if (modo != LCD_COMANDO && modo != LCD_DATOS) {
		manejar_errores();
	}
	uint8_t upper_nibble = comando >> LCD_DESPLAZAR_4;
	uint8_t lower_nibble = comando & LCD_VALOR_BINARIO_1111;
	lcd_escribir_nibble(upper_nibble, modo);
	lcd_escribir_nibble(lower_nibble, modo);
}
