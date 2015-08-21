/* vim: set sw=8 ts=8 si et: */
/*************************************************************************
 Title:   C include file for spi
 Target:    atmega8
 Copyright: GPL
***************************************************************************/
#ifndef SPI_H
#define SPI_H

#if defined(__AVR_ATmega8__)||(__AVR_ATmega168__)||(__AVR_ATmega328P__)||(__AVR_ATmega328__)
#define SPIPORT         PORTB
#define SPIDRR          DDRB
#define SPIPIN          PINB
#define SS              PB2
#define MOSI            PB3
#define MISO            PB4
#define SCK             PB5
#else
#error "No special device"
#endif

extern void spi_init(void);
extern unsigned char spi_set_sck_duration(unsigned char dur);
extern unsigned char spi_get_sck_duration(void);
extern unsigned char spi_mastertransmit(unsigned char data);
extern unsigned char spi_mastertransmit_16(unsigned int data);
extern unsigned char spi_mastertransmit_32(unsigned long data);
extern void spi_disable(void);
extern void spi_reset_pulse(void);
extern void spi_sck_pulse(void);

#endif /* SPI_H */
