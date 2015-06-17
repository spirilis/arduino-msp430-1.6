/* Chip-specific pin information - MSP430FR5969RGZ (48-pin VQFN) */

#ifndef CHIP_MSP430FR5969RGZ_H
#define CHIP_MSP430FR5969RGZ_H

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

/* Clock system - HFXT (XT2) pins */
#define CS_HFXT_PORT			PJIN
#define CS_HFXT_XIN			BIT6
#define CS_HFXT_XOUT			BIT7

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
	{P1, BIT0, 0, T0A1},  // 1 - P1.0/TA0.1/DMAE0/RTCCLK/A0/C0/VREF-/VeREF-
	{P1, BIT1, 1, T0A2},  // 2 - P1.1/TA0.2/TA1CLK/COUT/A1/C1/VREF+/VeREF+
	{P1, BIT2, 2, T1A1},  // 3 - P1.2/TA1.1/TA0CLK/COUT/A2/C2
	{P3, BIT0, 12, NOT_A_TIMER},  // 4 - P3.0/A12/C12
	{P3, BIT1, 13, NOT_A_TIMER},  // 5 - P3.1/A13/C13
	{P3, BIT2, 14, NOT_A_TIMER},  // 6 - P3.2/A14/C14
	{P3, BIT3, 15, NOT_A_TIMER},  // 7 - P3.3/A15/C15
	{P4, BIT7, -1, NOT_A_TIMER},  // 8 - P4.7
	{P1, BIT3, 3, T1A2},  // 9 - P1.3/TA1.2/UCB0STE/A3/C3
	{P1, BIT4, 4, T0B1},  // 10 - P1.4/TB0.1/UCA0STE/A4/C4
	{P1, BIT5, 5, T0B2},  // 11 - P1.5/TB0.2/UCA0CLK/A5/C5
	{PJ, BIT0, -1, NOT_A_TIMER},  // 12 - PJ.0/TDO/TB0OUTH/SMCLK/SRSCG1/C6
	{PJ, BIT1, -1, NOT_A_TIMER},  // 13 - PJ.1/TDI/TCLK/MCLK/SRSCG0/C7
	{PJ, BIT2, -1, NOT_A_TIMER},  // 14 - PJ.2/TMS/ACLK/SROSCOFF/C8
	{PJ, BIT3, -1, NOT_A_TIMER},  // 15 - PJ.3/TCK/SRCPUOFF/C9
	{P4, BIT0, 8, NOT_A_TIMER},  // 16 - P4.0/A8
	{P4, BIT1, 9, NOT_A_TIMER},  // 17 - P4.1/A9
	{P4, BIT2, 10, NOT_A_TIMER},  // 18 - P4.2/A10
	{P4, BIT3, 11, NOT_A_TIMER},  // 19 - P4.3/A11
	{P2, BIT5, -1, T0B0},  // 20 - P2.5/TB0.0/UCA1TXD/UCA1SIMO
	{P2, BIT6, -1, T0B1},  // 21 - P2.6/TB0.1/UCA1RXD/UCA1SOMI
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 22 - TEST/SBWTCK
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 23 - RESET/NMI/SBWTDIO
	{P2, BIT0, -1, T0B6},  // 24 - P2.0/TB0.6/UCA0TXD/UCA0SIMO/TB0CLK/ACLK
	{P2, BIT1, -1, T0B0},  // 25 - P2.1/TB0.0/UCA0RXD/UCA0SOMI
	{P2, BIT2, -1, T0B2},  // 26 - P2.2/TB0.2/UCB0CLK
	{P3, BIT4, -1, T0B3},  // 27 - P3.4/TB0.3/SMCLK
	{P3, BIT5, -1, T0B4},  // 28 - P3.5/TB0.4/COUT
	{P3, BIT6, -1, T0B5},  // 29 - P3.6/TB0.5
	{P3, BIT7, -1, T0B6},  // 30 - P3.7/TB0.6
	{P1, BIT6, -1, T0A0},  // 31 - P1.6/TB0.3/UCB0SIMO/UCB0SDA/TA0.0
	{P1, BIT7, -1, T1A0},  // 32 - P1.7/TB0.4/UCB0SOMI/UCB0SCL/TA1.0
	{P4, BIT4, -1, T0B5},  // 33 - P4.4/TB0.5
	{P4, BIT5, -1, NOT_A_TIMER},  // 34 - P4.5
	{P4, BIT6, -1, NOT_A_TIMER},  // 35 - P4.6
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 36 - DVss
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 37 - DVcc
	{P2, BIT7, -1, NOT_A_TIMER},  // 38 - P2.7
	{P2, BIT3, 6, T0A0},  // 39 - P2.3/TA0.0/UCA1STE/A6/C10
	{P2, BIT4, 7, T1A0},  // 40 - P2.4/TA1.0/UCA1CLK/A7/C11
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 41 - AVss
	{PJ, BIT6, -1, NOT_A_TIMER},  // 42 - PJ.6/HFXIN
	{PJ, BIT7, -1, NOT_A_TIMER},  // 43 - PJ.7/HFXOUT
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 44 - AVss
	{PJ, BIT4, -1, NOT_A_TIMER},  // 45 - PJ.4/LFXIN
	{PJ, BIT5, -1, NOT_A_TIMER},  // 46 - PJ.5/LFXOUT
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 47 - AVss
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 48 - AVcc
};
#else
extern const gpio_pin_definition_t chip_pins[];
#endif /* ARDUINO_MAIN */

#define CHIP_PIN_COUNT 48

// ADC pins
static const uint16_t A4 = 10 << 8;
static const uint16_t A5 = 11 << 8;
static const uint16_t A6 = 39 << 8;
static const uint16_t A7 = 40 << 8;
static const uint16_t A8 = 16 << 8;
static const uint16_t A9 = 17 << 8;
static const uint16_t A10 = 18 << 8;
static const uint16_t A11 = 19 << 8;
static const uint16_t A12 = 4 << 8;
static const uint16_t A13 = 5 << 8;
static const uint16_t A0 = 1 << 8;
static const uint16_t A14 = 6 << 8;
static const uint16_t A1 = 2 << 8;
static const uint16_t A15 = 7 << 8;
static const uint16_t A2 = 3 << 8;
static const uint16_t A3 = 9 << 8;

// Px_y definitions
static const uint16_t P4_0 = 16 << 8;
static const uint16_t P4_1 = 17 << 8;
static const uint16_t P4_2 = 18 << 8;
static const uint16_t P4_3 = 19 << 8;
static const uint16_t P4_4 = 33 << 8;
static const uint16_t P4_5 = 34 << 8;
static const uint16_t P4_6 = 35 << 8;
static const uint16_t P4_7 = 8 << 8;
static const uint16_t P1_0 = 1 << 8;
static const uint16_t P1_1 = 2 << 8;
static const uint16_t P1_2 = 3 << 8;
static const uint16_t P1_3 = 9 << 8;
static const uint16_t P1_4 = 10 << 8;
static const uint16_t P1_5 = 11 << 8;
static const uint16_t P1_6 = 31 << 8;
static const uint16_t P1_7 = 32 << 8;
static const uint16_t P2_0 = 24 << 8;
static const uint16_t P2_1 = 25 << 8;
static const uint16_t P2_2 = 26 << 8;
static const uint16_t P2_3 = 39 << 8;
static const uint16_t P2_4 = 40 << 8;
static const uint16_t P2_5 = 20 << 8;
static const uint16_t P2_6 = 21 << 8;
static const uint16_t P2_7 = 38 << 8;
static const uint16_t P3_0 = 4 << 8;
static const uint16_t P3_1 = 5 << 8;
static const uint16_t P3_2 = 6 << 8;
static const uint16_t P3_3 = 7 << 8;
static const uint16_t P3_4 = 27 << 8;
static const uint16_t P3_5 = 28 << 8;
static const uint16_t P3_6 = 29 << 8;
static const uint16_t P3_7 = 30 << 8;
static const uint16_t PJ_0 = 12 << 8;
static const uint16_t PJ_1 = 13 << 8;
static const uint16_t PJ_2 = 14 << 8;
static const uint16_t PJ_3 = 15 << 8;
static const uint16_t PJ_4 = 45 << 8;
static const uint16_t PJ_5 = 46 << 8;
static const uint16_t PJ_6 = 42 << 8;
static const uint16_t PJ_7 = 43 << 8;

/* USCI configuration */
#define USCI_A_COUNT 2
#define USCI_B_COUNT 1

#ifdef ARDUINO_MAIN
const usci_pin_definition_t uscia_pins[] = {
	// TXD << 8 | PORT_SELECTIONx, RXD << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx
	{24 << 8 | PORT_SELECTION1, 25 << 8 | PORT_SELECTION1, 10 << 8 | PORT_SELECTION1, 11 << 8 | PORT_SELECTION1},
	{20 << 8 | PORT_SELECTION1, 21 << 8 | PORT_SELECTION1, 39 << 8 | PORT_SELECTION1, 40 << 8 | PORT_SELECTION1}
};

const usci_pin_definition_t uscib_pins[] = {
	// MOSI/SDA << 8 | PORT_SELECTIONx, MISO/SCL << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx
	{31 << 8 | PORT_SELECTION1, 32 << 8 | PORT_SELECTION1, 9 << 8 | PORT_SELECTION1, 26 << 8 | PORT_SELECTION1}
};
#else
extern const usci_pin_definition_t uscia_pins[];
extern const usci_pin_definition_t uscib_pins[];
#endif /* ARDUINO_MAIN */



#endif /* CHIP_MSP430FR5969RGZ_H */

