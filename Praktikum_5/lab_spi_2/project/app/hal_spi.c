/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Module      : SPI Library
 * --
 * -- $Id: hal_spi.c 4720 2019-03-04 10:11:31Z akdi $
 * ------------------------------------------------------------------
 */

#ifdef MOCKED_SPI_DISPLAY
#include "hal_spi.h"
#include "hal_mocked.h"
void hal_spi_init(void)
{
    hal_mocked_spi_init();
}
uint8_t hal_spi_read_write(uint8_t send_byte)
{
    return hal_mocked_spi_read_write(send_byte);
}
#else  // !MOCKED_SPI_DISPLAY
#include <reg_stm32f4xx.h>
#include "hal_spi.h"

#define BIT_TXE  (uint32_t)0x00000002
#define BIT_RXNE (uint32_t)0x00000001
#define BIT_BSY  (uint32_t)0x00000080

static void set_ss_pin_low(void);
static void set_ss_pin_high(void);
static void wait_10_us(void);

/*
 * according to description in header file
 */
void hal_spi_init(void)
{
    RCC->APB2ENR |= 0x00001000;     // enable SPI clock
    RCC->AHB1ENR |= 0x00000001;     // start clock on GPIO A
    GPIOA->OSPEEDR &= 0xFFFF00FF;     // clear P4 to P7
    GPIOA->OSPEEDR |= 0x0000FF00;     // set P4 to P7 to 100 MHz
    GPIOA->MODER &= 0xFFFF00FF;       // clear mode on P5 to P7
    // P5 to P7, P4 output mode
    GPIOA->MODER |= 0x0000A900;       // Set alternate function mode on
    // P5 to P7, P4 output mode
    GPIOA->AFRL &= 0x0000FFFF;      // clear alternate function
    GPIOA->AFRL |= 0x55550000;      // Set SPI1 alternate function

    SPI1->CR2 = 0x0000;               // set spi to default state
    SPI1->CR1 = 0x0000;               // set spi to default state

    /// STUDENTS: To be programmed
		SPI1->CR1 |= (0x1<<2); // Master select
		SPI1->CR1 |= (0x7<<3); // set Baudrate
		SPI1->CR1 |= (0x1<<9); // set SSM = 1
		SPI1->CR1 |= (0x1<<8); // set SSI = 1
		SPI1->CR1 |= (0x1<<6); //set peripheral enable SPE, muss am Ende sein



    /// END: To be programmed
    set_ss_pin_high();
}

/*
 * according to description in header file
 */
uint8_t hal_spi_read_write(uint8_t send_byte)
{
    /// STUDENTS: To be programmed
		set_ss_pin_low(); // select slave
		SPI1->DR = send_byte; // schreibt in Daten regsiter
		while(!(SPI1->SR & BIT_TXE)){} //waits unitl txe = 1 
		while(!(SPI1->SR & BIT_RXNE)){}//waits unitl rxne = 1 --> RX Buffer Not Empty
		while((SPI1->SR & BIT_BSY)){} //waits unitl bsy = 0 --> Transaktion fertig 
		wait_10_us(); //stellt sicher das die Slave Selct erst nach der lezten Flanke gesetzt wird
		set_ss_pin_high(); // deselect slave
		return (uint8_t) SPI1->DR; 

		/// END: To be programmed
}

/**
 * Set Slave-Select Pin (Port A, Pin 5) low
 *
 * No parameters
 *
 * No returns
 */
static void set_ss_pin_low(void)
{
    GPIOA->BSRR |= 0x0010 << 16;              // Set Port A, Pin 5 low
}

/**
 * Set Slave-Select Pin (Port A, Pin 5) high
 *
 * No parameters
 *
 * No returns
 */
static void set_ss_pin_high(void)
{
    GPIOA->BSRR |= 0x0010;             // Set Port A, Pin 5 high
}

/**
 * Wait for approximately 10us
 *
 * No parameters
 *
 * No returns
 */
static void wait_10_us(void)
{
    uint8_t counter = 0;
    while (counter < 160) {
        counter++;
    }
}
#endif // MOCKED_SPI_DISPLAY
