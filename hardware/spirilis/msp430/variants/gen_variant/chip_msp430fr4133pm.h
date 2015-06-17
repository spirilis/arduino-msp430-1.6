/* Chip-specific pin information - MSP430FR4133PM (64-pin LQFP) */

#ifndef CHIP_MSP430FR4133PM_H
#define CHIP_MSP430FR4133PM_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>


/* Clock system - Clockable Peripherals */
#define CPUFREQ_CHIP_SPECIFIC_PERIPH_MASK	(PERIPH_TIMERA0 | PERIPH_TIMERA1 |	\
						 PERIPH_USCI_A0 | PERIPH_USCI_B0)

#define CPUFREQ_CHIP_SPECIFIC_PERIPH_COUNT	4

/* Clock system - SMCLK, ACLK output pins */
#define CLKOUT_SMCLK_PORT		P8IN
#define CLKOUT_SMCLK_PIN		BIT0

#define CLKOUT_ACLK_PORT		P8IN
#define CLKOUT_ACLK_PIN			BIT1

/* Clock system - LFXT pins */
#define CS_LFXT_PORT			P4IN
#define CS_LFXT_XIN			BIT1
#define CS_LFXT_XOUT			BIT2

/* Clock system - VLO frequency */
#define CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY	9400UL
#define CPUFREQ_CHIP_SPECIFIC_MODOSC_NOMINAL_FREQUENCY	4800000UL


#ifdef ARDUINO_MAIN
/* GPIO port definitions */
const gpio_port_definition_t chip_ports[] = {
        /* FR5xxx gpio_port_definition_t layout:
         * portin, portout, portdir, portren, portsel, porties, portie, portifg
         */
        {NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL},  // filler for index #0
        {&P1IN, &P1OUT, &P1DIR, &P1REN, &P1SEL0, &P1IES, &P1IE, &P1IFG},
        {&P2IN, &P2OUT, &P2DIR, &P2REN, &P2SEL0, &P2IES, &P2IE, &P2IFG},
        {&P3IN, &P3OUT, &P3DIR, &P3REN, &P3SEL0, NULL, NULL, NULL},
        {&P4IN, &P4OUT, &P4DIR, &P4REN, &P4SEL0, NULL, NULL, NULL},
        {&P5IN, &P5OUT, &P5DIR, &P5REN, &P5SEL0, NULL, NULL, NULL},
        {&P6IN, &P6OUT, &P6DIR, &P6REN, &P6SEL0, NULL, NULL, NULL},
        {&P7IN, &P7OUT, &P7DIR, &P7REN, &P7SEL0, NULL, NULL, NULL},
        {&P8IN, &P8OUT, &P8DIR, &P8REN, &P8SEL0, NULL, NULL, NULL},
        {NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL},  // P9 doesn't exist on FR4133
        {NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL},  // P10 doesn't exist on FR4133
        {NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL},  // P11 doesn't exist on FR4133
        {NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL}   // PJ doesn't exist on FR4133
};
#else
extern const gpio_port_definition_t chip_ports[];
#endif


#ifdef ARDUINO_MAIN
/* GPIO pin definitions - generated from awk script
 *
 * Fields: <P1/P2/etc>, <Port Pin#>, <ADC chan#>, <NOT_A_TIMER | TxNy enum>
 */
const gpio_pin_definition_t chip_pins[] = {
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // filler for index #0
	{P4, BIT7, -1, NOT_A_TIMER},  // 1 - P4.7/R13
	{P4, BIT6, -1, NOT_A_TIMER},  // 2 - P4.6/R23
	{P4, BIT5, -1, NOT_A_TIMER},  // 3 - P4.5/R33
	{P4, BIT4, -1, NOT_A_TIMER},  // 4 - P4.4/LCDCAP1
	{P4, BIT3, -1, NOT_A_TIMER},  // 5 - P4.3/LCDCAP0
	{P4, BIT2, -1, NOT_A_TIMER},  // 6 - P4.2/XOUT
	{P4, BIT1, -1, NOT_A_TIMER},  // 7 - P4.1/XIN
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 8 - DVss
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 9 - DVcc
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 10 - RESET/NMI/SBWTDIO
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 11 - TEST/SBWTCK
	{P4, BIT0, -1, T1A1},  // 12 - P4.0/TA1.1
	{P8, BIT3, -1, T1A2},  // 13 - P8.3/TA1.2
	{P8, BIT2, -1, NOT_A_TIMER},  // 14 - P8.2/TA1CLK
	{P8, BIT1, 9, NOT_A_TIMER},  // 15 - P8.1/ACLK/A9
	{P8, BIT0, 8, NOT_A_TIMER},  // 16 - P8.0/SMCLK/A8
	{P1, BIT7, 7, T0A1},  // 17 - P1.7/TA0.1/TDO/A7
	{P1, BIT6, 6, T0A2},  // 18 - P1.6/TA0.2/TDI/TCLK/A6
	{P1, BIT5, 5, NOT_A_TIMER},  // 19 - P1.5/TA0CLK/TMS/A5
	{P1, BIT4, 4, NOT_A_TIMER},  // 20 - P1.4/MCLK/TCK/A4/VREF+
	{P1, BIT3, 3, NOT_A_TIMER},  // 21 - P1.3/UCA0STE/A3
	{P1, BIT2, 2, NOT_A_TIMER},  // 22 - P1.2/UCA0CLK/A2
	{P1, BIT1, 1, NOT_A_TIMER},  // 23 - P1.1/UCA0RXD/UCA0SOMI/A1/VeREF+
	{P1, BIT0, 0, NOT_A_TIMER},  // 24 - P1.0/UCA0TXD/UCA0SIMO/A0/VeREF-
	{P5, BIT7, -1, NOT_A_TIMER},  // 25 - P5.7/L39
	{P5, BIT6, -1, NOT_A_TIMER},  // 26 - P5.6/L38
	{P5, BIT5, -1, NOT_A_TIMER},  // 27 - P5.5/L37
	{P5, BIT4, -1, NOT_A_TIMER},  // 28 - P5.4/L36
	{P5, BIT3, -1, NOT_A_TIMER},  // 29 - P5.3/UCB0SOMI/UCB0SCL/L35
	{P5, BIT2, -1, NOT_A_TIMER},  // 30 - P5.2/UCB0SIMO/UCB0SDA/L34
	{P5, BIT1, -1, NOT_A_TIMER},  // 31 - P5.1/UCB0CLK/L33
	{P5, BIT0, -1, NOT_A_TIMER},  // 32 - P5.0/UCB0STE/L32
	{P2, BIT7, -1, NOT_A_TIMER},  // 33 - P2.7/L31
	{P2, BIT6, -1, NOT_A_TIMER},  // 34 - P2.6/L30
	{P2, BIT5, -1, NOT_A_TIMER},  // 35 - P2.5/L29
	{P2, BIT4, -1, NOT_A_TIMER},  // 36 - P2.4/L28
	{P2, BIT3, -1, NOT_A_TIMER},  // 37 - P2.3/L27
	{P2, BIT2, -1, NOT_A_TIMER},  // 38 - P2.2/L26
	{P2, BIT1, -1, NOT_A_TIMER},  // 39 - P2.1/L25
	{P2, BIT0, -1, NOT_A_TIMER},  // 40 - P2.0/L24
	{P6, BIT7, -1, NOT_A_TIMER},  // 41 - P6.7/L23
	{P6, BIT6, -1, NOT_A_TIMER},  // 42 - P6.6/L22
	{P6, BIT5, -1, NOT_A_TIMER},  // 43 - P6.5/L21
	{P6, BIT4, -1, NOT_A_TIMER},  // 44 - P6.4/L20
	{P6, BIT3, -1, NOT_A_TIMER},  // 45 - P6.3/L19
	{P6, BIT2, -1, NOT_A_TIMER},  // 46 - P6.2/L18
	{P6, BIT1, -1, NOT_A_TIMER},  // 47 - P6.1/L17
	{P6, BIT0, -1, NOT_A_TIMER},  // 48 - P6.0/L16
	{P3, BIT7, -1, NOT_A_TIMER},  // 49 - P3.7/L15
	{P3, BIT6, -1, NOT_A_TIMER},  // 50 - P3.6/L14
	{P3, BIT5, -1, NOT_A_TIMER},  // 51 - P3.5/L13
	{P3, BIT4, -1, NOT_A_TIMER},  // 52 - P3.4/L12
	{P3, BIT3, -1, NOT_A_TIMER},  // 53 - P3.3/L11
	{P3, BIT2, -1, NOT_A_TIMER},  // 54 - P3.2/L10
	{P3, BIT1, -1, NOT_A_TIMER},  // 55 - P3.1/L9
	{P3, BIT0, -1, NOT_A_TIMER},  // 56 - P3.0/L8
	{P7, BIT7, -1, NOT_A_TIMER},  // 57 - P7.7/L7
	{P7, BIT6, -1, NOT_A_TIMER},  // 58 - P7.6/L6
	{P7, BIT5, -1, NOT_A_TIMER},  // 59 - P7.5/L5
	{P7, BIT4, -1, NOT_A_TIMER},  // 60 - P7.4/L4
	{P7, BIT3, -1, NOT_A_TIMER},  // 61 - P7.3/L3
	{P7, BIT2, -1, NOT_A_TIMER},  // 62 - P7.2/L2
	{P7, BIT1, -1, NOT_A_TIMER},  // 63 - P7.1/L1
	{P7, BIT0, -1, NOT_A_TIMER},  // 64 - P7.0/L0
};
#else
extern const gpio_pin_definition_t chip_pins[];
#endif /* ARDUINO_MAIN */

#define CHIP_PIN_COUNT 64

// ADC pins
static const uint16_t A4 = 20 << 8;
static const uint16_t A5 = 19 << 8;
static const uint16_t A6 = 18 << 8;
static const uint16_t A7 = 17 << 8;
static const uint16_t A8 = 16 << 8;
static const uint16_t A9 = 15 << 8;
static const uint16_t A0 = 24 << 8;
static const uint16_t A1 = 23 << 8;
static const uint16_t A2 = 22 << 8;
static const uint16_t A3 = 21 << 8;

// Px_y definitions
static const uint16_t P4_0 = 12 << 8;
static const uint16_t P4_1 = 7 << 8;
static const uint16_t P4_2 = 6 << 8;
static const uint16_t P4_3 = 5 << 8;
static const uint16_t P4_4 = 4 << 8;
static const uint16_t P4_5 = 3 << 8;
static const uint16_t P4_6 = 2 << 8;
static const uint16_t P4_7 = 1 << 8;
static const uint16_t P5_0 = 32 << 8;
static const uint16_t P5_1 = 31 << 8;
static const uint16_t P5_2 = 30 << 8;
static const uint16_t P5_3 = 29 << 8;
static const uint16_t P5_4 = 28 << 8;
static const uint16_t P5_5 = 27 << 8;
static const uint16_t P5_6 = 26 << 8;
static const uint16_t P5_7 = 25 << 8;
static const uint16_t P1_0 = 24 << 8;
static const uint16_t P1_1 = 23 << 8;
static const uint16_t P1_2 = 22 << 8;
static const uint16_t P1_3 = 21 << 8;
static const uint16_t P1_4 = 20 << 8;
static const uint16_t P1_5 = 19 << 8;
static const uint16_t P1_6 = 18 << 8;
static const uint16_t P1_7 = 17 << 8;
static const uint16_t P6_0 = 48 << 8;
static const uint16_t P6_1 = 47 << 8;
static const uint16_t P6_2 = 46 << 8;
static const uint16_t P6_3 = 45 << 8;
static const uint16_t P6_4 = 44 << 8;
static const uint16_t P6_5 = 43 << 8;
static const uint16_t P6_6 = 42 << 8;
static const uint16_t P6_7 = 41 << 8;
static const uint16_t P2_0 = 40 << 8;
static const uint16_t P2_1 = 39 << 8;
static const uint16_t P2_2 = 38 << 8;
static const uint16_t P2_3 = 37 << 8;
static const uint16_t P2_4 = 36 << 8;
static const uint16_t P2_5 = 35 << 8;
static const uint16_t P2_6 = 34 << 8;
static const uint16_t P2_7 = 33 << 8;
static const uint16_t P7_0 = 64 << 8;
static const uint16_t P7_1 = 63 << 8;
static const uint16_t P7_2 = 62 << 8;
static const uint16_t P7_3 = 61 << 8;
static const uint16_t P7_4 = 60 << 8;
static const uint16_t P7_5 = 59 << 8;
static const uint16_t P7_6 = 58 << 8;
static const uint16_t P7_7 = 57 << 8;
static const uint16_t P3_0 = 56 << 8;
static const uint16_t P3_1 = 55 << 8;
static const uint16_t P3_2 = 54 << 8;
static const uint16_t P3_3 = 53 << 8;
static const uint16_t P3_4 = 52 << 8;
static const uint16_t P3_5 = 51 << 8;
static const uint16_t P3_6 = 50 << 8;
static const uint16_t P3_7 = 49 << 8;
static const uint16_t P8_0 = 16 << 8;
static const uint16_t P8_1 = 15 << 8;
static const uint16_t P8_2 = 14 << 8;
static const uint16_t P8_3 = 13 << 8;

/* USCI configuration */
#define USCI_A_COUNT 1
#define USCI_B_COUNT 1

#ifdef ARDUINO_MAIN
const usci_pin_definition_t uscia_pins[] = {
	// TXD << 8 | PORT_SELECTIONx, RXD << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx
	{24 << 8 | PORT_SELECTION0, 23 << 8 | PORT_SELECTION0, 21 << 8 | PORT_SELECTION0, 22 << 8 | PORT_SELECTION0}
};

const usci_pin_definition_t uscib_pins[] = {
	// MOSI/SDA << 8 | PORT_SELECTIONx, MISO/SCL << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx
	{30 << 8 | PORT_SELECTION0, 29 << 8 | PORT_SELECTION0, 32 << 8 | PORT_SELECTION0, 31 << 8 | PORT_SELECTION0}
};
#else
extern const usci_pin_definition_t uscia_pins[];
extern const usci_pin_definition_t uscib_pins[];
#endif /* ARDUINO_MAIN */



#endif /* CHIP_MSP430FR4133PM_H */
