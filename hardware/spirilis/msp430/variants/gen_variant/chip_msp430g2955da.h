/* Chip-specific pin information - MSP430G2955DA (38-pin TSSOP) */

#ifndef CHIP_MSP430G2955DA_H
#define CHIP_MSP430G2955DA_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>


/* Clock system - Clockable Peripherals */
#define CPUFREQ_CHIP_SPECIFIC_PERIPH_MASK	(PERIPH_TIMERA0 | PERIPH_TIMERA1 |      \
						 PERIPH_TIMERB0 |			\
						 PERIPH_USCI_A0 | PERIPH_USCI_B0)

#define CPUFREQ_CHIP_SPECIFIC_PERIPH_COUNT	5

/* Clock system - SMCLK, ACLK output pins */
#define CLKOUT_SMCLK_PORT		P1IN
#define CLKOUT_SMCLK_PIN		BIT4

#define CLKOUT_ACLK_PORT		P2IN
#define CLKOUT_ACLK_PIN			BIT0

/* Clock system - LFXT pins */
#define BCS2_LFXT_PORT			P2IN
#define BCS2_LFXT_XIN			BIT6
#define BCS2_LFXT_XOUT			BIT7

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
	{&P4IN, &P4OUT, &P4DIR, &P4REN, &P4SEL, &P4SEL2, NULL, NULL, NULL},  // P4 no interrupts
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P5 doesn't exist on G2xx4 or G2xx5
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P6 doesn't exist on G2xx4 or G2xx5
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P7 doesn't exist on G2xx4 or G2xx5
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P8 doesn't exist on G2xx4 or G2xx5
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P9 doesn't exist on G2xx4 or G2xx5
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P10 doesn't exist on G2xx4 or G2xx5
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P11 doesn't exist on G2xx4 or G2xx5
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // PJ doesn't exist on G2xx4 or G2xx5
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
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 1 - TEST/SBWTCK
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 2 - DVcc
	{P2, BIT5, -1, T1A0},  // 3 - P2.5/TA1.0/ROSC
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 4 - DVss
	{P2, BIT7, -1, NOT_A_TIMER},  // 5 - P2.7/XOUT
	{P2, BIT6, -1, NOT_A_TIMER},  // 6 - P2.6/XIN
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 7 - RESET/NMI/SBWTDIO
	{P2, BIT0, 0, NOT_A_TIMER},  // 8 - P2.0/TA1CLK/ACLK/A0
	{P2, BIT1, 1, NOT_A_TIMER},  // 9 - P2.1/TA0INCLK/SMCLK/A1
	{P2, BIT2, 2, T0A0},  // 10 - P2.2/TA0.0/A2
	{P3, BIT0, 5, NOT_A_TIMER},  // 11 - P3.0/UCB0STE/UCA0CLK/A5
	{P3, BIT1, -1, NOT_A_TIMER},  // 12 - P3.1/UCB0SIMO/UCB0SDA
	{P3, BIT2, -1, NOT_A_TIMER},  // 13 - P3.2/UCB0SOMI/UCB0SCL
	{P3, BIT3, -1, NOT_A_TIMER},  // 14 - P3.3/UCB0CLK/UCA0STE
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 15 - AVss
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 16 - AVcc
	{P4, BIT0, -1, T0B0},  // 17 - P4.0/TB0.0/CA0
	{P4, BIT1, -1, T0B1},  // 18 - P4.1/TB0.1/CA1
	{P4, BIT2, -1, T0B2},  // 19 - P4.2/TB0.2/CA2
	{P4, BIT3, 12, T0B0},  // 20 - P4.3/TB0.0/A12/CA3
	{P4, BIT4, 13, T0B1},  // 21 - P4.4/TB0.1/A13/CA4
	{P4, BIT5, 14, T0B2},  // 22 - P4.5/TB0.2/A14/CA5
	{P4, BIT6, 15, NOT_A_TIMER},  // 23 - P4.6/TB0OUTH/A15/CA6
	{P4, BIT7, -1, NOT_A_TIMER},  // 24 - P4.7/TB0CLK/CA7
	{P3, BIT4, -1, NOT_A_TIMER},  // 25 - P3.4/UCA0TXD/UCA0SIMO
	{P3, BIT5, -1, NOT_A_TIMER},  // 26 - P3.5/UCA0RXD/UCA0SOMI
	{P3, BIT6, 6, T1A1},  // 27 - P3.6/TA1.1/A6
	{P3, BIT7, 7, T1A2},  // 28 - P3.7/TA1.2/A7
	{P2, BIT3, 3, T0A1},  // 29 - P2.3/TA0.1/A3/VREF-/VeREF-
	{P2, BIT4, 4, T0A2},  // 30 - P2.4/TA0.2/A4/VREF+/VeREF+
	{P1, BIT0, -1, NOT_A_TIMER},  // 31 - P1.0/TA0CLK/ADC10CLK
	{P1, BIT1, -1, T0A0},  // 32 - P1.1/TA0.0
	{P1, BIT2, -1, T0A1},  // 33 - P1.2/TA0.1
	{P1, BIT3, -1, T0A2},  // 34 - P1.3/TA0.2
	{P1, BIT4, -1, NOT_A_TIMER},  // 35 - P1.4/SMCLK/TCK
	{P1, BIT5, -1, T0A0},  // 36 - P1.5/TA0.0/TMS
	{P1, BIT6, -1, T0A1},  // 37 - P1.6/TA0.1/TDI
	{P1, BIT7, -1, T0A2},  // 38 - P1.7/TA0.2/TDO/TDI
};
#else
extern const gpio_pin_definition_t chip_pins[];
#endif /* ARDUINO_MAIN */

#define CHIP_PIN_COUNT 38

// ADC pins
static const uint16_t A4 = 30 << 8;
static const uint16_t A5 = 11 << 8;
static const uint16_t A6 = 27 << 8;
static const uint16_t A7 = 28 << 8;
static const uint16_t A12 = 20 << 8;
static const uint16_t A13 = 21 << 8;
static const uint16_t A0 = 8 << 8;
static const uint16_t A14 = 22 << 8;
static const uint16_t A1 = 9 << 8;
static const uint16_t A15 = 23 << 8;
static const uint16_t A2 = 10 << 8;
static const uint16_t A3 = 29 << 8;

// Px_y definitions
static const uint16_t P1_7 = 38 << 8;
static const uint16_t P4_5 = 22 << 8;
static const uint16_t P2_2 = 10 << 8;
static const uint16_t P2_3 = 29 << 8;
static const uint16_t P4_6 = 23 << 8;
static const uint16_t P2_4 = 30 << 8;
static const uint16_t P4_7 = 24 << 8;
static const uint16_t P3_0 = 11 << 8;
static const uint16_t P2_5 = 3 << 8;
static const uint16_t P3_1 = 12 << 8;
static const uint16_t P2_6 = 6 << 8;
static const uint16_t P3_2 = 13 << 8;
static const uint16_t P2_7 = 5 << 8;
static const uint16_t P1_0 = 31 << 8;
static const uint16_t P3_3 = 14 << 8;
static const uint16_t P1_1 = 32 << 8;
static const uint16_t P3_4 = 25 << 8;
static const uint16_t P1_2 = 33 << 8;
static const uint16_t P3_5 = 26 << 8;
static const uint16_t P4_0 = 17 << 8;
static const uint16_t P1_3 = 34 << 8;
static const uint16_t P3_6 = 27 << 8;
static const uint16_t P4_1 = 18 << 8;
static const uint16_t P1_4 = 35 << 8;
static const uint16_t P3_7 = 28 << 8;
static const uint16_t P4_2 = 19 << 8;
static const uint16_t P1_5 = 36 << 8;
static const uint16_t P4_3 = 20 << 8;
static const uint16_t P2_0 = 8 << 8;
static const uint16_t P1_6 = 37 << 8;
static const uint16_t P4_4 = 21 << 8;
static const uint16_t P2_1 = 9 << 8;


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
	{15 << 8 | PORT_SELECTION0, 14 << 8 | PORT_SELECTION0, 6 << 8 | PORT_SELECTION0, 7 << 8 | PORT_SELECTION0}
};
#else
extern const usci_pin_definition_t uscia_pins[];
extern const usci_pin_definition_t uscib_pins[];
#endif /* ARDUINO_MAIN */



#endif /* CHIP_MSP430G2955DA_H */
