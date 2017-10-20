/***************************************************************************//**
  @file     SysTick.h
  @brief    SysTick driver
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _SYSTICK_H_
#define _SYSTICK_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define SYSTICK_ISR_FREQUENCY_HZ 10000.0


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialize SysTic driver
*/
void SysTick_Init (void);

/**
 * @brief Register the function to be call every SysTick
 * @param funcallback Function to register
 * @return Function registered OK
*/
uint8_t SysTick_RegisterCallBack (void (*funcallback)(void),uint32_t divider);
uint32_t get_time(void);


/*******************************************************************************
 ******************************************************************************/

#endif // _SYSTICK_H_
