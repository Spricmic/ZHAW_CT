/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Application for testing external memory
 * --
 * -- $Id: main.c 5605 2023-01-05 15:52:42Z frtt $
 * ------------------------------------------------------------------
 */

#include <stdint.h>
#include "hal_rcc.h"
#include "hal_fmc.h"
#include "hal_ct_lcd.h"
#include "reg_ctboard.h"
#include "hal_ct_buttons.h"
#include "hal_ct_seg7.h"

#define NR_OF_DATA_LINES           8
#define NR_OF_ADDRESS_LINES       11
#define CHECKER_BOARD           0xAA
#define INVERSE_CHECKER_BOARD   0x55
#define WALKING_ONES_ADDRESS    0x7FF

/* Set-up the macros (#defines) for your test */
/// STUDENTS: To be programmed

#define START_SRAM2 0x64000000
#define END_SRAM2 0x640007FF
#define CHECKERBOARD 0xAA
#define CHECKERBOARD_INV 0x55

uint64_t current_address;
uint32_t* address_ptr;
uint8_t testing_value;
uint8_t walking_errors = 0x0;
uint32_t* checker_address_ptr;
int address_shifts[] ={0x0, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800};
uint16_t address_errors = 0x0;
uint16_t error_display = 0x0;
/// END: To be programmed

int main(void)
{
    hal_fmc_sram_init_t init;
    hal_fmc_sram_timing_t timing;
    
    /* add your required automatic (local) variables here */ 
    /// STUDENTS: To be programmed
		



    /// END: To be programmed

    init.address_mux = DISABLE;                             // setup peripheral
    init.type = HAL_FMC_TYPE_SRAM;
    init.width = HAL_FMC_WIDTH_8B;
    init.write_enable = ENABLE;

    timing.address_setup = 0xFF;                            // all in HCLK
                                                            // cycles
    timing.address_hold = 0xFF;
    timing.data_setup = 0xFF;

    hal_fmc_init_sram(HAL_FMC_SRAM_BANK2, init, timing);    // init external bus
                                                            // bank 2 (NE2)
                                                            // asynch
    
    /* Data Bus Test - Walking ONES test */
    /// STUDENTS: To be programmed
		// inkrementiert die current_address von anfang des speichers bis zum ende
		current_address = START_SRAM2;
		address_ptr = (uint32_t*)(uintptr_t)current_address;
		uint8_t counter = 0;
		testing_value = 0x1;
		walking_errors = 0x0;
		while (counter < 7){
			*address_ptr = testing_value;
			if (*address_ptr != testing_value){
				walking_errors = walking_errors | testing_value;
			}
			testing_value = testing_value << 1;
		}
		CT_LED->BYTE.LED7_0 = walking_errors;
		
		/*while (current_address <= END_SRAM2){
			address_ptr = (uint32_t*)(uintptr_t)current_address;
			// führt den Wert 1 durch alle 8 bits im current_address register
			uint8_t counter = 0;
			*address_ptr = 0x1;
			while (counter < 7){
				uint32_t value = *address_ptr;
				value = value << 1;
				*address_ptr = value;
			}
			current_address++;
		}*/
		


    /// END: To be programmed
    
    /* Address Bus Test 
     * (1)  Write default values
     *
     *      Write the memory at all the power of 2 addresses (including 0x0000)
     *      to the default value of CHECKER_BOARD
     *
     * (2)  Perform tests
     *
     *      Select one power of 2 addresses after the other as test_address 
     *      (starting from the highest all the way down to 0x0000) --> 
     *          - Write the memory at test_address to INVERSE_CHECKER_BOARD
     *          - For all the power of 2 addresses including 0x0000
     *              o Read the memory content
     *              o Verify that the read value is either
     *                  (a) equal to CHECKER_BOARD in case a different address 
     *                      than test_addressed has been read or
     *                  (b) equal to INVERSE_CHECKER_BOARD in case the address
     *                      at test_address has been read
     *              o Errors found shall be indicated on LED31--16
     */
    
    /// STUDENTS: To be programmed
		for (int i = 0; i<NR_OF_ADDRESS_LINES; i++){
			current_address = START_SRAM2 & address_shifts[i];
			address_ptr = (uint32_t*)(uintptr_t)current_address;
			*address_ptr = CHECKERBOARD;
		}
		
		for (int i = 0; i<NR_OF_ADDRESS_LINES; i++){
			current_address = START_SRAM2 & address_shifts[i];
			checker_address_ptr = (uint32_t*)(uintptr_t)current_address;
			*checker_address_ptr = CHECKERBOARD_INV;
			for (int j = 0; j<NR_OF_ADDRESS_LINES; j++){
				current_address = START_SRAM2 & address_shifts[i];
				address_ptr = (uint32_t*)(uintptr_t)current_address;
				
				if(address_ptr != checker_address_ptr){
					
					if (*address_ptr == CHECKERBOARD){
					}
					else{
						address_errors |= (1 << i);
					}
				}
				else{
					
				if (*address_ptr == CHECKERBOARD_INV){
					}
					else{
						address_errors |= (1 << i);
					}
				}
			}
		}

		error_display = address_errors >> 1;
		if ((address_errors && 0x1) == 1){
			error_display |= 0xF000;
		}
		else{
		}
		CT_LED->HWORD.LED31_16 = walking_errors;


     /// END: To be programmed
    
    /* Device Test 
     * (1) Fill the whole memory with known increment pattern.
     *          Address     Data
     *          0x000       0x01
     *          0x001       0x02
     *          .....       ....
     *          0x0FE       0xFF
     *          0x0FF       0x00
     *          0x100       0x01
     *          .....       ....
     *
     * (2) First test: Read back each location and check pattern.
     *     In case of error, write address with wrong data to 7-segment and
     *     wait for press on button T0.
     *     Bitwise invert  the pattern in each location for the second test
     *
     * (3) Second test: Read back each location and check for new pattern.
     *     In case of error, write address with wrong data to 7-segment and
     *     wait for press on button T0.
     */
    /// STUDENTS: To be programmed




    /// END: To be programmed
    
    // Write 'End'
    CT_SEG7->RAW.BYTE.DS0 = 0xA1;
    CT_SEG7->RAW.BYTE.DS1 = 0xAB;
    CT_SEG7->RAW.BYTE.DS2 = 0x86;
    CT_SEG7->RAW.BYTE.DS3 = 0xFF;
    
    while(1){
    }

}
