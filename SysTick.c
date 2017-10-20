/***************************************************************************//**
  @file     SysTick.c
  @brief    SysTick driver
  @author   Nicol�s Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "SysTick.h"
#include "Template/LDM.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define NUM_CALLBACK 10

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static int16_t callbacks_counter=0;
static void (*callback[NUM_CALLBACK])(void);
static uint32_t divider_counter[NUM_CALLBACK];
static uint32_t timer_count = 0;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
uint32_t get_time(void){
	return timer_count;
}

void SysTick_Init (void)
{
	static uint8_t yaInit = false;

	if (yaInit)
		return;

	SysTick->CTRL = 0x00;
	SysTick->LOAD = (uint32_t) ((__CORE_CLOCK__/SYSTICK_ISR_FREQUENCY_HZ)-1);
	SysTick->VAL  = 0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
	yaInit = true;
}

uint8_t SysTick_RegisterCallBack (void (*funcallback)(void),uint32_t divider)
{
	uint8_t ret=false;
	if(funcallback!=NULL && callbacks_counter<NUM_CALLBACK)
	{
		callback[callbacks_counter]=funcallback;
		divider_counter[callbacks_counter] = divider;
		callbacks_counter++;
		ret=1;									//devuelve uno si pudo hacerlo
	}
	return ret;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

__ISR__ SysTick_Handler (void)
{
	uint16_t i;
	timer_count++;




	for (i=0 ; i<callbacks_counter ; ++i)
	{
		if(callback[i]!=NULL && timer_count%divider_counter[i])  //doble chequeo
		{
			callback[i]();
		}
	}
}


