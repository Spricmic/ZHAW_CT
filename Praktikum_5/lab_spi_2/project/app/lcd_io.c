/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : CT2 lab - SPI Display
 * -- Description : Contains the implementations of the functions
 * --               to write and read frames from and to the TFT-LCD
 * --               display EAeDIPTFT43-A.
 * --
 * -- $Id: lcd_io.c 4720 2019-03-04 10:11:31Z akdi $
 * ------------------------------------------------------------------
 */
#include "lcd_io.h"
#include "hal_spi.h"
#include "hal_sbuf.h"

#define ACK_CHAR         (uint8_t)0x06
#define DC1_CHAR         (uint8_t)0x11
#define DC2_CHAR         (uint8_t)0x12
#define ESC_CHAR         (uint8_t)0x1B
#define ONE_CHAR         (uint8_t)0x01
#define S_CHAR         	 (uint8_t)0x53
#define SEND_BCC_CHAR    (uint8_t)0x66
#define NOTHING_RECEIVED (uint8_t)0
enum { SUCCESS = 0, ERRORCODE = 1 };

/* ------------------------------------------------------------------
 * -- Function prototypes
 * ------------------------------------------------------------------
 */
static void send_read_display_buffer_request(void);

/* ------------------------------------------------------------------
 * -- Function implementations
 * ------------------------------------------------------------------
 */

/*
 * according to description in header file
 */
void init_display_interface(void)
{
    hal_spi_init();
    hal_sbuf_init();
}

/*
 * according to description in header file
 */
uint8_t read_display_buffer(uint8_t *readBuffer)
{
    /// STUDENTS: To be programmed
		int i = 0;
		uint8_t len = 0;
			
		if (hal_sbuf_get_state()!=0){
			send_read_display_buffer_request();
			if (hal_spi_read_write(0x00) != ACK_CHAR){
				return 0;
			}
			hal_spi_read_write(0x00);
			len = hal_spi_read_write(0x00);
	
			for (i = 0; i < len;i++){
				readBuffer[i] = hal_spi_read_write(0x00);
			}
			hal_spi_read_write(0x00);
		}
		return len;
		/// END: To be programmed
}

/*
 * according to description in header file
 */
uint8_t write_cmd_to_display(const uint8_t *cmdBuffer, uint8_t length)
{
    /// STUDENTS: To be programmed
		int i = 0;
		uint8_t bcc = 0;
		uint8_t rec_byte = 0;
		hal_spi_read_write(DC1_CHAR); //DC1
		hal_spi_read_write(length + 1); //Len
		hal_spi_read_write(ESC_CHAR); //ESC

		for (i = 0; i < length;i++){
			hal_spi_read_write(cmdBuffer[i]);
			bcc += cmdBuffer[i];
		}
		
		hal_spi_read_write((length + 1 + DC1_CHAR + ESC_CHAR + bcc) % 256);
		rec_byte = hal_spi_read_write(0x00);
		return rec_byte == ACK_CHAR ? SUCCESS : ERRORCODE;
		/// END: To be programmed
}

/*
 * Assemble and send a packet to trigger the reading of the display buffer
 * Uses the sequence "<DC2>, 0x01, 0x53, checksum" according to datasheet
 */
static void send_read_display_buffer_request()
{
    /// STUDENTS: To be programmed
		hal_spi_read_write(DC2_CHAR);
		hal_spi_read_write(0x01);
		hal_spi_read_write(0x53);
		hal_spi_read_write((DC2_CHAR + 0x01 + 0x53) % 256);
		/// END: To be programmed
}
