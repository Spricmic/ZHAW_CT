/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : CT2 Lab GPIO
 * -- Description : Configure and use GPIO port B as output and 
 * --               GPIO port A is input.
 * --               Lab version without additional hardware 
 * --               except for three wires.
 * --
 * -- $Id: main.c ostt $
 * ------------------------------------------------------------------------- */

#include <stdint.h>
#include <reg_stm32f4xx.h>

#include "reg_ctboard.h"

/* user macros */

#define MASK_3_BITS  0x0007

/* ----------------------------------------------------------------------------
 * Main
 * ------------------------------------------------------------------------- */

int main(void)
{
    uint16_t data_gpio_in;              // use to read input values from gpio
    uint8_t data_dip_switch;            // use to read values from dip switches
		uint8_t data_read_pa;
		uint8_t read_dipsw;
		uint8_t read_odr;
	
    GPIOA_ENABLE();                     // peripheral clk enable
    GPIOB_ENABLE();                     // peripheral clk enable

    /* Reset GPIOA specific values */
    GPIOA->MODER = 0xa8000000;           // mode register
    GPIOA->OSPEEDR = 0x00000000;         // output speed register
    GPIOA->PUPDR = 0x64000000;           // pull resistor register
    GPIOA->OTYPER = 0x00000000;          // type register (pp / f. gate)
    GPIOA->AFRL = 0x00000000;
    GPIOA->AFRH = 0x00000000;
    GPIOA->ODR = 0x00000000;             // output data register
    
    /* Reset GPIOB specific values */
    GPIOB->MODER = 0x00000280;           // mode register
    GPIOB->OSPEEDR = 0x000000c0;         // output speed register
    GPIOB->PUPDR = 0x00000100;           // pull resistor register
    GPIOB->OTYPER = 0x00000000;          // type register (pp / f. gate)
    GPIOB->AFRL = 0x00000000;
    GPIOB->AFRH = 0x00000000;
    GPIOB->ODR = 0x00000000;             // output data register
    

    /* configure GPIO pins
     * clear register bits: GPIOx->xxxx &= ~(clear_mask << bit_pos);
     * set register bits:   GPIOx->xxxx |=  (set_value << bit_pos);
     * On GPIOA and GPIOB only pins 11 down to 0 are available to the user. 
     * Pins 15 down to 12 are used for system functions of the discovery board, 
     * e.g. connection of the debugger.
     * These pins must not be reconfigured. 
     * OTHERWISE THE DEBUGGER CANNOT BE USED ANY MORE!!!
     */
    /// STUDENTS: To be programmed

		
		GPIOA->MODER &= ~(0x03 << 0);
		GPIOA->MODER &= ~(0x03 << 2);
		GPIOA->MODER &= ~(0x03 << 4);
		
		GPIOA->MODER |= (0x00 << 0);
		GPIOA->MODER |= (0x00 << 2);
		GPIOA->MODER |= (0x00 << 4);
		
		GPIOA->PUPDR &= ~(0x03 << 0);
		GPIOA->PUPDR &= ~(0x03 << 2);
		GPIOA->PUPDR &= ~(0x03 << 4);
		
		GPIOA->PUPDR |= ~(0x02 << 0);
		GPIOA->PUPDR |= ~(0x01 << 2);
		GPIOA->PUPDR |= ~(0x00 << 4);


		GPIOB->MODER &= ~(0x03 << 0);
		GPIOB->MODER &= ~(0x03 << 2);
		GPIOB->MODER &= ~(0x03 << 4);
		
		GPIOB->MODER |= (0x01 << 0);
		GPIOB->MODER |= (0x01 << 2);
		GPIOB->MODER |= (0x01 << 4);
		
		GPIOB->OTYPER &= ~(0x01 << 0);
		GPIOB->OTYPER &= ~(0x01 << 1);
		GPIOB->OTYPER &= ~(0x01 << 2);
		
		GPIOB->OTYPER |= (0x00 << 0);
		GPIOB->OTYPER |= (0x01 << 1);
		GPIOB->OTYPER |= (0x01 << 2);
		
		GPIOB->PUPDR &= ~(0x03 << 0);
		GPIOB->PUPDR &= ~(0x03 << 2);
		GPIOB->PUPDR &= ~(0x03 << 4);
		
		GPIOB->PUPDR |= (0x00 << 0);
		GPIOB->PUPDR |= (0x00 << 2);
		GPIOB->PUPDR |= (0x01 << 4);
		
		GPIOB->OSPEEDR &= ~(0x03 << 0);
		GPIOB->OSPEEDR &= ~(0x03 << 2);
		GPIOB->OSPEEDR &= ~(0x03 << 4);
		
		GPIOB->OSPEEDR |= (0x00 << 0);
		GPIOB->OSPEEDR |= (0x01 << 2);
		GPIOB->OSPEEDR |= (0x02 << 4);

    /// END: To be programmed

    while (1) {
        /* implement tasks 6.1 to 6.2 below */
        /// STUDENTS: To be programmed
				///write data from input to LED's and output 3 from LSB
				
				read_dipsw = CT_DIPSW->BYTE.S15_8;	/// read from dipswitches
				read_dipsw &= 0x07;	/// mask output gotten from dipswitches
			
				GPIOB->ODR = read_dipsw;	///write read and masked date to output pins
			
				read_odr = GPIOA->IDR;		/// read signal transfered trough cable
				read_odr &= 0x07;		/// mask read signal
			
				/// write to LED's
				CT_LED->BYTE.LED23_16 = read_odr;
				CT_LED->BYTE.LED15_8 = read_dipsw;
				CT_LED->BYTE.LED7_0 = GPIOB->ODR;
				


        /// END: To be programmed

    }
}
