/*******************************************************************************
  @file     UART.h
  @brief    Driver de UART para el microcontrolador
  @author   Grupo I
 ******************************************************************************/

#ifndef SOURCES_UART_H_
#define SOURCES_UART_H_

/*******************************************************************************
				INCLUDE HEADER FILES
*******************************************************************************/

#include "Template/LDM.h"
#include <stdbool.h>

/*******************************************************************************
				CONSTANT AND MACRO DEFINITIONS USING #DEFINE
*******************************************************************************/

#define UART_HAL_DEFAULT_BAUDRATE 115200
#define UART_HARDWARE_BUFFER_SIZE 8
#define UART_SOFTWARE_BUFFER_SIZE 100
#define TIME 2000

/*******************************************************************************
				FUNCTION PROTOTYPES WITH GLOBAL SCOPE
*******************************************************************************/

/**
 * @brief Función de inicialización del UART
*/
void UART_Init (void);

/**
 * @brief Función que retorna true si se puede transmitir
 * 		  o false si no se puede transmitir
*/
unsigned char UART_Get_Status(void);

/**
 * @brief Funcion para transmitir por UART
 * @param data Puntero a la data a enviar
 * @param length longitud de lo que se va a enviar
*/
void UART_Send_Data(char * data, uint16_t length);


#endif /* SOURCES_UART_H_ */
