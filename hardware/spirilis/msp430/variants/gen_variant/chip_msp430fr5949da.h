/* Chip-specific pin information - MSP430FR5949DA (38-pin TSSOP) */

#ifndef CHIP_MSP430FR5949DA_H
#define CHIP_MSP430FR5949DA_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>


/* Clock system - Clockable Peripherals */
#define CPUFREQ_CHIP_SPECIFIC_PERIPH_MASK	(PERIPH_TIMERA0 | PERIPH_TIMERA1 |	\
						 PERIPH_TIMERA2 | PERIPH_TIMERA3 |	\
						 PERIPH_TIMERB0 |			\
						 PERIPH_USCI_A0 | PERIPH_USCI_B0 |	\
						 PERIPH_USCI_A1)

#define CPUFREQ_CHIP_SPECIFIC_PERIPH_COUNT	8

/* Clock system - SMCLK, ACLK output pins */
#define CLKOUT_SMCLK_PORT		P3IN
#define CLKOUT_SMCLK_PIN		BIT4

#define CLKOUT_ACLK_PORT		P2IN
#define CLKOUT_ACLK_PIN			BIT0

/* Clock system - LFXT pins */
#define CS_LFXT_PORT			PJIN
#define CS_LFXT_XIN			BIT4
#define CS_LFXT_XOUT			BIT5

/* Clock system - HFXT (XT2) pins -- FR5949 doesn't have HFXT support */
//#define CS_HFXT_PORT			PJIN
//#define CS_HFXT_XIN			BIT6
//#define CS_HFXT_XOUT			BIT7

/* Clock system - VLO frequency */
#define CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY	9400UL
#define CPUFREQ_CHIP_SPECIFIC_MODOSC_NOMINAL_FREQUENCY	4800000UL



#ifdef ARDUINO_MAIN
/* GPIO port definitions */
const gpio_port_definition_t chip_ports[] = {
        /* FR5xxx gpio_port_definition_t layout:
         * portin, portout, portdir, portren, portsel, portsel2, porties, portie, portifg
         */
        {NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // filler for index #0
        {&P1IN, &P1OUT, &P1DIR, &P1REN, &P1SEL0, &P1SEL1, &P1IES, &P1IE, &P1IFG},
        {&P2IN, &P2OUT, &P2DIR, &P2REN, &P2SEL0, &P2SEL1, &P2IES, &P2IE, &P2IFG},
        {&P3IN, &P3OUT, &P3DIR, &P3REN, &P3SEL0, &P3SEL1, &P3IES, &P3IE, &P3IFG},
        {&P4IN, &P4OUT, &P4DIR, &P4REN, &P4SEL0, &P4SEL1, &P4IES, &P4IE, &P4IFG},
        {NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P5 doesn't exist on FR5969
        {NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P6 doesn't exist on FR5969
        {NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P7 doesn't exist on FR5969
        {NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P8 doesn't exist on FR5969
        {NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P9 doesn't exist on FR5969
        {NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P10 doesn't exist on FR5969
        {NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P11 doesn't exist on FR5969
        {&PJIN, &PJOUT, &PJDIR, &PJREN, &PJSEL0, &PJSEL1, NULL, NULL, NULL}
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
	{PJ, BIT4, -1, NOT_A_TIMER},  // 1 - PJ.4/LFXIN
	{PJ, BIT5, -1, NOT_A_TIMER},  // 2 - PJ.5/LFXOUT
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 3 - AVss
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 4 - AVcc
	{P1, BIT0, 0, T0A1},  // 5 - P1.0/TA0.1/DMAE0/RTCCLK/A0/C0/VREF-/VeREF-
	{P1, BIT1, 1, T0A2},  // 6 - P1.1/TA0.2/TA1CLK/COUT/A1/C1/VREF+/VeREF+
	{P1, BIT2, 2, T1A1},  // 7 - P1.2/TA1.1/TA0CLK/COUT/A2/C2
	{P3, BIT0, 12, NOT_A_TIMER},  // 8 - P3.0/A12/C12
	{P3, BIT1, 13, NOT_A_TIMER},  // 9 - P3.1/A13/C13
	{P3, BIT2, 14, NOT_A_TIMER},  // 10 - P3.2/A14/C14
	{P3, BIT3, 15, NOT_A_TIMER},  // 11 - P3.3/A15/C15
	{P1, BIT3, 3, T1A2},  // 12 - P1.3/TA1.2/UCB0STE/A3/C3
	{P1, BIT4, 4, T0B1},  // 13 - P1.4/TB0.1/UCA0STE/A4/C4
	{P1, BIT5, 5, T0B2},  // 14 - P1.5/TB0.2/UCA0CLK/A5/C5
	{PJ, BIT0, -1, NOT_A_TIMER},  // 15 - PJ.0/TDO/TB0OUTH/SMCLK/SRSCG1/C6
	{PJ, BIT1, -1, NOT_A_TIMER},  // 16 - PJ.1/TDI/TCLK/MCLK/SRSCG0/C7
	{PJ, BIT2, -1, NOT_A_TIMER},  // 17 - PJ.2/TMS/ACLK/SROSCOFF/C8
	{PJ, BIT3, -1, NOT_A_TIMER},  // 18 - PJ.3/TCK/SRCPUOFF/C9
	{P2, BIT5, -1, T0B0},  // 19 - P2.5/TB0.0/UCA1TXD/UCA1SIMO
	{P2, BIT6, -1, T0B1},  // 20 - P2.6/TB0.1/UCA1RXD/UCA1SOMI
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 21 - TEST/SBWTCK
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 22 - RESET/NMI/SBWTDIO
	{P2, BIT0, -1, T0B6},  // 23 - P2.0/TB0.6/UCA0TXD/UCA0SIMO/TB0CLK/ACLK
	{P2, BIT1, -1, T0B0},  // 24 - P2.1/TB0.0/UCA0RXD/UCA0SOMI
	{P2, BIT2, -1, T0B2},  // 25 - P2.2/TB0.2/UCB0CLK
	{P3, BIT4, -1, T0B3},  // 26 - P3.4/TB0.3/SMCLK
	{P3, BIT5, -1, T0B4},  // 27 - P3.5/TB0.4/COUT
	{P3, BIT6, -1, T0B5},  // 28 - P3.6/TB0.5
	{P3, BIT7, -1, T0B6},  // 29 - P3.7/TB0.6
	{P1, BIT6, -1, T0A0},  // 30 - P1.6/TB0.3/UCB0SIMO/UCB0SDA/TA0.0
	{P1, BIT7, -1, T1A0},  // 31 - P1.7/TB0.4/UCB0SOMI/UCB0SCL/TA1.0
	{P4, BIT4, -1, T0B5},  // 32 - P4.4/TB0.5
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 33 - DVss
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 34 - DVcc
	{P2, BIT7, -1, NOT_A_TIMER},  // 35 - P2.7
	{P2, BIT3, 6, T0A0},  // 36 - P2.3/TA0.0/UCA1STE/A6/C10
	{P2, BIT4, 7, T1A0},  // 37 - P2.4/TA1.0/UCA1CLK/A7/C11
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 38 - AVss
};
#else
extern const gpio_pin_definition_t chip_pins[];
#endif /* ARDUINO_MAIN */

#define CHIP_PIN_COUNT 38

// ADC pins
static const uint16_t A4 = 13 << 8;
static const uint16_t A5 = 14 << 8;
static const uint16_t A6 = 36 << 8;
static const uint16_t A7 = 37 << 8;
static const uint16_t A12 = 8 << 8;
static const uint16_t A13 = 9 << 8;
static const uint16_t A0 = 5 << 8;
static const uint16_t A14 = 10 << 8;
static const uint16_t A1 = 6 << 8;
static const uint16_t A15 = 11 << 8;
static const uint16_t A2 = 7 << 8;
static const uint16_t A3 = 12 << 8;

// Px_y definitions
static const uint16_t P1_7 = 31 << 8;
static const uint16_t P2_2 = 25 << 8;
static const uint16_t P2_3 = 36 << 8;
static const uint16_t PJ_0 = 15 << 8;
static const uint16_t P2_4 = 37 << 8;
static const uint16_t PJ_1 = 16 << 8;
static const uint16_t P2_5 = 19 << 8;
static const uint16_t PJ_2 = 17 << 8;
static const uint16_t P3_0 = 8 << 8;
static const uint16_t P2_6 = 20 << 8;
static const uint16_t PJ_3 = 18 << 8;
static const uint16_t P3_1 = 9 << 8;
static const uint16_t P2_7 = 35 << 8;
static const uint16_t P3_2 = 10 << 8;
static const uint16_t PJ_4 = 1 << 8;
static const uint16_t P3_3 = 11 << 8;
static const uint16_t P1_0 = 5 << 8;
static const uint16_t PJ_5 = 2 << 8;
static const uint16_t P3_4 = 26 << 8;
static const uint16_t P1_1 = 6 << 8;
static const uint16_t P3_5 = 27 << 8;
static const uint16_t P1_2 = 7 << 8;
static const uint16_t P3_6 = 28 << 8;
static const uint16_t P1_3 = 12 << 8;
static const uint16_t P3_7 = 29 << 8;
static const uint16_t P1_4 = 13 << 8;
static const uint16_t P2_0 = 23 << 8;
static const uint16_t P1_5 = 14 << 8;
static const uint16_t P4_4 = 32 << 8;
static const uint16_t P1_6 = 30 << 8;
static const uint16_t P2_1 = 24 << 8;


/* USCI configuration */
#define USCI_A_COUNT 2
#define USCI_B_COUNT 1

#ifdef ARDUINO_MAIN
const usci_pin_definition_t uscia_pins[] = {
	// TXD << 8 | PORT_SELECTIONx, RXD << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx
	{23 << 8 | PORT_SELECTION1, 24 << 8 | PORT_SELECTION1, 13 << 8 | PORT_SELECTION1, 14 << 8 | PORT_SELECTION1},
	{19 << 8 | PORT_SELECTION1, 20 << 8 | PORT_SELECTION1, 36 << 8 | PORT_SELECTION1, 37 << 8 | PORT_SELECTION1}
};

const usci_pin_definition_t uscib_pins[] = {
	// MOSI/SDA << 8 | PORT_SELECTIONx, MISO/SCL << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx
	{30 << 8 | PORT_SELECTION1, 31 << 8 | PORT_SELECTION1, 12 << 8 | PORT_SELECTION1, 25 << 8 | PORT_SELECTION1}
};
#else
extern const usci_pin_definition_t uscia_pins[];
extern const usci_pin_definition_t uscib_pins[];
#endif /* ARDUINO_MAIN */



#endif /* CHIP_MSP430FR5949DA_H */

