/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module adc.
 * --
 * --
 * -- $Id: adc.c 5605 2023-01-05 15:52:42Z frtt $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>


/* user includes */
#include "adc.h"
#include "reg_ctboard.h"

/* -- Macros
 * ------------------------------------------------------------------------- */

#define PERIPH_GPIOF_ENABLE (0x00000020)
#define PERIPH_ADC3_ENABLE  (0x00000400)

/* Configuring pin for ADC: PF.6 */
#define GPIOF_MODER_ANALOG (0x3 << 12)


/* -- Macros used by student code
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

#define BUFFER_SIZE 16


/// END: To be programmed


/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 *  See header file
 */
void adc_init(void)
{
    /* Enable peripheral clocks */
    RCC->AHB1ENR |= PERIPH_GPIOF_ENABLE;
    RCC->APB2ENR |= PERIPH_ADC3_ENABLE;

    /* Configure PF.6 as input */
    GPIOF->MODER |= GPIOF_MODER_ANALOG;

    /* ADC common init */
    ADCCOM->CCR = 0;        // TSVREF = '0'    -> Temp sensor disabled
                            // VBATE = '0'     -> VBAT disabled
                            // ADCPRE = '00'   -> APB2 / 2 -> 21 MHz
                            // DMA = '00'      -> DMA disabled
                            // DELAY = '0000'  -> Delay 5 cycles
                            // MULTI = '00000' -> ADC independent mode

    /* Configure ADC3 */
    
    /// STUDENTS: To be programmed
		ADC3->CR1 = 0x3000000;		// 6bit resolution
		ADC3->CR2 = 0x1;		// singel conversion, enable ADC, right align
		
		ADC3->SMPR1 = 0x0;		// all sampling times 3 cycles
		ADC3->SMPR2 = 0x0;		// all sampling times 3 cycles
		
		ADC3->SQR1 = 0x0;		// sequence length = 1
		ADC3->SQR2 = 0x0;
		ADC3->SQR3 = 0x4; 	// ch6 is first in sequence



    /// END: To be programmed
}


/*
 *  See header file
 */
uint16_t adc_get_value(adc_resolution_t resolution)
{
    uint16_t adc_value;

    /// STUDENTS: To be programmed
			
			ADC3->CR1 = resolution;
			ADC3->CR2 |= (0x1 << 30);
			while(!(ADC3->SR & 0x2)){
			}
			adc_value = ADC3->DR;

    /// END: To be programmed

    return adc_value;
}


/*
 *  See header file
 */
int16_t buffer[BUFFER_SIZE];
int16_t bufferIndex = 0;

uint16_t adc_filter_value(uint16_t adc_value)
{
    uint16_t filtered_value = 0;

    /// STUDENTS: To be programmed
		
		int32_t sum;
		
		buffer[bufferIndex] = adc_value;
    bufferIndex = (bufferIndex + 1) % BUFFER_SIZE;

    sum = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        sum += buffer[i];
    }

    filtered_value = sum / BUFFER_SIZE;

    /// END: To be programmed

    return filtered_value;
}

/*
 *  See header file
 */
uint32_t choose_resolution(void)
{
	adc_resolution_t resolution;
	uint16_t output;
	uint16_t data = CT_HEXSW;

	data &= 0x3;
	switch(data) {
        case 0:
            output = 63;
						resolution = ADC_RES_6BIT;
            break;
        case 1:
            output = 255;
						resolution = ADC_RES_8BIT;
            break;
        case 2:
            output = 1023;
						resolution = ADC_RES_10BIT;
            break;
        case 3:
            output = 4095;
						resolution = ADC_RES_12BIT;
            break;
        default:
            output = 63; // basecase resolution of 6bit
						resolution = ADC_RES_6BIT;
    }
	CT_LED->HWORD.LED15_0 = output;
	return resolution;
}
