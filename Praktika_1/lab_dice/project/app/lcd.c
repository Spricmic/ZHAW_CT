/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module lcd
 * --               Performs all the interactions with the lcd
 * --
 * -- $Id: lcd.c 5144 2020-09-01 06:17:21Z ruan $
 * ------------------------------------------------------------------
 */

/* standard includes */
#include <stdio.h>
#include <string.h>

/* user includes */
#include "lcd.h"
#include "reg_ctboard.h"

/* macros */
#define LCD_ADDR_LINE1      0u
#define LCD_ADDR_LINE2      20u

#define NR_OF_CHAR_PER_LINE 20u

#define LCD_CLEAR           "                    "

#define SPACE_PER_VALUE 3u

/// STUDENTS: To be programmed

/*declarations*/

static uint8_t text_length;
static uint8_t i;
static char text[NR_OF_CHAR_PER_LINE];
static char buffer[NR_OF_CHAR_PER_LINE];
static char clear = ' ';


void lcd_write_value(uint8_t slot_nr, uint8_t value){
    snprintf(buffer, SPACE_PER_VALUE, "%d", value);
    
		text_length = strlen(buffer);
		for (i = 0; i < text_length; i++){
				CT_LCD->ASCII[((slot_nr-1)*SPACE_PER_VALUE) + i] = buffer[i];
		}
}

void lcd_write_total(uint8_t total_value){
		snprintf(text, NR_OF_CHAR_PER_LINE, "total throws %d", total_value);
	
    text_length = strlen(text);
    for (i = 0; i < text_length; i++){
        CT_LCD->ASCII[LCD_ADDR_LINE2 + i] = text[i];
    }
}

void hal_ct_lcd_clear(void){
    CT_LCD-> BG.GREEN = 65535;

    for (i = 0; i < (2*NR_OF_CHAR_PER_LINE-1); i++){
        CT_LCD->ASCII[i] = clear;
    }
}

/// END: To be programmed
