/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur             -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences)           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : CT2 lab - Timer PWM
 * -- Description : Main program and interrupt service routine
 * --
 * --               Task 1: - Setup timer TIM4 to 1s.
 * --                       - Setup interrupt to toggle LED
 * --               Task 2: - Setup timer TIM3 to PWM mode
 * --                       - Read DIP switches to set duty cycles of channels
 * --               Task 3: - Use interrupt of TIM4 to create a transition
 * --                         from one colour to another
 * --
 * -- $Id: main.c 5605 2023-01-05 15:52:42Z frtt $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>
#include <reg_ctboard.h>

/* user includes */
#include "timer.h"


/* -- Macros used as by student code
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed



/// END: To be programmed

/* -- Function prototypes
 * ------------------------------------------------------------------------- */
 void TIM4_IRQHandler(void);
 

/* -- global variables visible only within this module
 * ------------------------------------------------------------------------- */

/* cyclic counter value between 0 and 0xF */
static uint16_t cycle_counter_4bit = 0;

/* -- M A I N
 * ------------------------------------------------------------------------- */

int main(void)
{
    /// STUDENTS: To be programmed
	
	uint8_t dips7_0;
  uint8_t dips15_8;
	uint8_t dips23_16;
	
	tim4_init();
	tim3_init();
	
	while(1){
		dips7_0 = CT_DIPSW->BYTE.S7_0;
		tim3_set_compare_register(0, dips7_0);
		dips15_8 = CT_DIPSW->BYTE.S15_8;
		tim3_set_compare_register(1, dips15_8);
		dips23_16 = CT_DIPSW->BYTE.S23_16;
		tim3_set_compare_register(2, dips23_16);
	}



    /// END: To be programmed
}


/* -- Interrupt service routine0
 * ------------------------------------------------------------------------- */

void TIM4_IRQHandler(void)
{
    /// STUDENTS: To be programmed
		tim4_reset_uif();
		if (CT_LED->BYTE.LED31_24 == 0xFF){
			CT_LED->BYTE.LED31_24 = 0x00;
		}
		else{
			CT_LED->BYTE.LED31_24 = 0xFF;
		}
		



    /// END: To be programmed
}
