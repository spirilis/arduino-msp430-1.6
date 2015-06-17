/* Chip-specific pin information - MSP430G2553PW28 (28-pin TSSOP) */

#ifndef CHIP_MSP430G2553PW28_H
#define CHIP_MSP430G2553PW28_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>


/* Clock system - Clockable Peripherals */
#define CPUFREQ_CHIP_SPECIFIC_PERIPH_MASK       (PERIPH_TIMERA0 | PERIPH_TIMERA1 |      \
                                                 PERIPH_USCI_A0 | PERIPH_USCI_B0)

#define CPUFREQ_CHIP_SPECIFIC_PERIPH_COUNT      4

/* Clock system - SMCLK, ACLK output pins */
#define CLKOUT_SMCLK_PORT               P1IN
#define CLKOUT_SMCLK_PIN                BIT4

#define CLKOUT_ACLK_PORT                P1IN
#define CLKOUT_ACLK_PIN                 BIT0

/* Clock system - LFXT pins */
#define BCS2_LFXT_PORT                    P2IN
#define BCS2_LFXT_XIN                     BIT6
#define BCS2_LFXT_XOUT                    BIT7

/* Clock system - VLO frequency */
#define CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY     12000UL



#ifdef ARDUINO_MAIN
/* GPIO port definitions */
const gpio_port_definition_t chip_ports[] = {
	/* G2xxx gpio_port_definition_t layout:
	 * portin, portout, portdir, portren, portsel, portsel2, porties, portie, portifg
	 */
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // filler for index #0
	{&P1IN, &P1OUT, &P1DIR, &P1REN, &P1SEL, &P1SEL2, &P1IES, &P1IE, &P1IFG},
	{&P2IN, &P2OUT, &P2DIR, &P2REN, &P2SEL, &P2SEL2, &P2IES, &P2IE, &P2IFG},
	{&P3IN, &P3OUT, &P3DIR, &P3REN, &P3SEL, &P3SEL2, NULL, NULL, NULL},  // P3 no interrupts
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P4 doesn't exist on G2xx3
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P5 doesn't exist on G2xx3
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P6 doesn't exist on G2xx3
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P7 doesn't exist on G2xx3
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P8 doesn't exist on G2xx3
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P9 doesn't exist on G2xx3
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P10 doesn't exist on G2xx3
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P11 doesn't exist on G2xx3
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // PJ doesn't exist on G2xx3
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
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 1 - DVcc
	{P1, BIT0, 0, NOT_A_TIMER},  // 2 - P1.0/TA0CLK/ACLK/A0/CA0
	{P1, BIT1, 1, T0A0},  // 3 - P1.1/TA0.0/UCA0RXD/UCA0SOMI/A1/CA1
	{P1, BIT2, 2, T0A1},  // 4 - P1.2/TA0.1/UCA0TXD/UCA0SIMO/A2/CA2
	{P1, BIT3, 3, NOT_A_TIMER},  // 5 - P1.3/ADC10CLK/CAOUT/VREF-/VeREF-/A3/CA3
	{P1, BIT4, 4, NOT_A_TIMER},  // 6 - P1.4/SMCLK/UCB0STE/UCA0CLK/VREF+/VeREF+/A4/CA4/TCK
	{P1, BIT5, 5, T0A0},  // 7 - P1.5/TA0.0/UCB0CLK/UCA0STE/A5/CA5/TMS
	{P3, BIT1, -1, T1A0},  // 8 - P3.1/TA1.0
	{P3, BIT0, -1, T0A2},  // 9 - P3.0/TA0.2
	{P2, BIT0, -1, T1A0},  // 10 - P2.0/TA1.0
	{P2, BIT1, -1, T1A1},  // 11 - P2.1/TA1.1
	{P2, BIT2, -1, T1A1},  // 12 - P2.2/TA1.1
	{P3, BIT2, -1, T1A1},  // 13 - P3.2/TA1.1
	{P3, BIT3, -1, T1A2},  // 14 - P3.3/TA1.2
	{P3, BIT4, -1, T0A0},  // 15 - P3.4/TA0.0
	{P2, BIT3, -1, T1A0},  // 16 - P2.3/TA1.0
	{P2, BIT4, -1, T1A2},  // 17 - P2.4/TA1.2
	{P2, BIT5, -1, T1A2},  // 18 - P2.5/TA1.2
	{P3, BIT5, -1, T0A1},  // 19 - P3.5/TA0.1
	{P3, BIT6, -1, T0A2},  // 20 - P3.6/TA0.2
	{P3, BIT7, -1, NOT_A_TIMER},  // 21 - P3.7/TA1CLK/CAOUT
	{P1, BIT6, 6, T0A1},  // 22 - P1.6/TA0.1/UCB0SOMI/UCB0SCL/A6/CA6/TDI/TCLK
	{P1, BIT7, 7, NOT_A_TIMER},  // 23 - P1.7/CAOUT/UCB0SIMO/UCB0SDA/A7/CA7/TDO/TDI
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 24 - RESET/NMI/SBWTDIO
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 25 - TEST/SBWTCK
	{P2, BIT7, -1, NOT_A_TIMER},  // 26 - P2.7/XOUT
	{P2, BIT6, -1, T0A1},  // 27 - P2.6/XIN/TA0.1
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 28 - DVss
};
#else
extern const gpio_pin_definition_t chip_pins[];
#endif /* ARDUINO_MAIN */

#define CHIP_PIN_COUNT 28

// ADC pins
static const uint16_t A4 = 6 << 8;
static const uint16_t A5 = 7 << 8;
static const uint16_t A6 = 22 << 8;
static const uint16_t A7 = 23 << 8;
static const uint16_t A0 = 2 << 8;
static const uint16_t A1 = 3 << 8;
static const uint16_t A2 = 4 << 8;
static const uint16_t A3 = 5 << 8;

// Px_y definitions
static const uint16_t P1_7 = 23 << 8;
static const uint16_t P2_2 = 12 << 8;
static const uint16_t P2_3 = 16 << 8;
static const uint16_t P2_4 = 17 << 8;
static const uint16_t P2_5 = 18 << 8;
static const uint16_t P3_0 = 9 << 8;
static const uint16_t P2_6 = 27 << 8;
static const uint16_t P3_1 = 8 << 8;
static const uint16_t P2_7 = 26 << 8;
static const uint16_t P3_2 = 13 << 8;
static const uint16_t P3_3 = 14 << 8;
static const uint16_t P1_0 = 2 << 8;
static const uint16_t P3_4 = 15 << 8;
static const uint16_t P1_1 = 3 << 8;
static const uint16_t P3_5 = 19 << 8;
static const uint16_t P1_2 = 4 << 8;
static const uint16_t P3_6 = 20 << 8;
static const uint16_t P1_3 = 5 << 8;
static const uint16_t P3_7 = 21 << 8;
static const uint16_t P1_4 = 6 << 8;
static const uint16_t P2_0 = 10 << 8;
static const uint16_t P1_5 = 7 << 8;
static const uint16_t P1_6 = 22 << 8;
static const uint16_t P2_1 = 11 << 8;


/* USCI configuration */
#define USCI_A_COUNT 1
#define USCI_B_COUNT 1

#ifdef ARDUINO_MAIN
const usci_pin_definition_t uscia_pins[] = {
	// TXD << 8 | PORT_SELECTIONx, RXD << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx
	{4 << 8 | PORT_SELECTION0, 3 << 8 | PORT_SELECTION0, 7 << 8 | PORT_SELECTION0, 6 << 8 | PORT_SELECTION0}
};

const usci_pin_definition_t uscib_pins[] = {
	// MOSI/SDA << 8 | PORT_SELECTIONx, MISO/SCL << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx
	{23 << 8 | PORT_SELECTION0, 22 << 8 | PORT_SELECTION0, 6 << 8 | PORT_SELECTION0, 7 << 8 | PORT_SELECTION0}
};
#else
extern const usci_pin_definition_t uscia_pins[];
extern const usci_pin_definition_t uscib_pins[];
#endif /* ARDUINO_MAIN */



#endif /* CHIP_MSP430G2553PW28_H */
