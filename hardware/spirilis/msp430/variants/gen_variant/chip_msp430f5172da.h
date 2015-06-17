/* Chip-specific information - MSP430F5172DA (38-pin TSSOP) */

#ifndef CHIP_MSP430F5172DA_H
#define CHIP_MSP430F5172DA_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>


/* Clock system - Clockable Peripherals */
#define CPUFREQ_CHIP_SPECIFIC_PERIPH_MASK       (PERIPH_TIMERA0 |			\
						 PERIPH_TIMERD0 | PERIPH_TIMERD1	\
						 PERIPH_USCI_A0 | PERIPH_USCI_B0)

#define CPUFREQ_CHIP_SPECIFIC_PERIPH_COUNT      5

/* Clock system - SMCLK, ACLK output pins */
#define CLKOUT_SMCLK_PORT               PJIN
#define CLKOUT_SMCLK_PIN                BIT0

#define CLKOUT_ACLK_PORT                PJIN
#define CLKOUT_ACLK_PIN                 BIT2

/* Clock system - LFXT pins */
#define UCS_LFXT_PORT                   PJIN
#define UCS_LFXT_XIN                    BIT5
#define UCS_LFXT_XOUT                   BIT4

/* Clock system - HFXT (XT2) pins */
#define UCS_HFXT_PORT                   PJIN
#define UCS_HFXT_XIN                    BIT5
#define UCS_HFXT_XOUT                   BIT4

/* Clock system - VLO frequency */
#define CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY     9400UL


#ifdef ARDUINO_MAIN
/* GPIO port definitions */
const gpio_port_definition_t chip_ports[] = {
	/* F5xxx gpio_port_definition_t layout:
	 * portin, portout, portdir, portren, portsel, portmap, porties, portie, portifg
	 */
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // filler for index #0
	{&P1IN, &P1OUT, &P1DIR, &P1REN, &P1SEL, &P1MAP0, &P1IES, &P1IE, &P1IFG},
	{&P2IN, &P2OUT, &P2DIR, &P2REN, &P2SEL, &P2MAP0, &P2IES, &P2IE, &P2IFG},
	{&P3IN, &P3OUT, &P3DIR, &P3REN, &P3SEL, &P3MAP0, NULL, NULL, NULL},
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P4 doesn't exist on F5172
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P5 doesn't exist on F5172
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P6 doesn't exist on F5172
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P7 doesn't exist on F5172
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P8 doesn't exist on F5172
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P9 doesn't exist on F5172
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P10 doesn't exist on F5172
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P11 doesn't exist on F5172
	{&PJIN, &PJOUT, &PJDIR, &PJREN, &PJSEL , NULL, NULL, NULL, NULL}
};
#else
extern const gpio_port_definition_t chip_ports[];
#endif



#ifdef ARDUINO_MAIN
/* GPIO pin definitions - generated from awk script
 *
 * Fields: <P1/P2/etc>, <Port Pin#>, <ADC chan#>, <NOT_A_TIMER | TxNy enum>, <Timer PMAP_ID> */
const gpio_pin_definition_t chip_pins[] = {
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // filler for index #0
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 1 - AVcc
	{PJ, BIT4, -1, NOT_A_TIMER, 0},  // 2 - PJ.4/XOUT
	{PJ, BIT5, -1, NOT_A_TIMER, 0},  // 3 - PJ.5/XIN
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 4 - AVss
	{P1, BIT0, 0, NOT_A_TIMER, 0},  // 5 - P1.0/PM_UCA0CLK/PM_UCB0STE/A0/CB0
	{P1, BIT1, 1, NOT_A_TIMER, 0},  // 6 - P1.1/PM_UCA0TXD/PM_UCA0SIMO/A1/CB1
	{P1, BIT2, 2, NOT_A_TIMER, 0},  // 7 - P1.2/PM_UCA0RXD/PM_UCA0SOMI/A2/CB2
	{P1, BIT3, 3, NOT_A_TIMER, 0},  // 8 - P1.3/PM_UCB0CLK/PM_UCA0STE/A3/CB3
	{P1, BIT4, 4, NOT_A_TIMER, 0},  // 9 - P1.4/PM_UCB0SIMO/PM_UCB0SDA/A4/CB4
	{P1, BIT5, 5, NOT_A_TIMER, 0},  // 10 - P1.5/PM_UCB0SOMI/PM_UCB0SCL/A5/CB5
	{PJ, BIT0, -1, NOT_A_TIMER, 0},  // 11 - PJ.0/SMCLK/TDO/CB6
	{PJ, BIT1, -1, NOT_A_TIMER, 0},  // 12 - PJ.1/MCLK/TDI/TCLK/CB7
	{PJ, BIT2, -1, NOT_A_TIMER, 0},  // 13 - PJ.2/ADC10CLK/TMS/CB8
	{PJ, BIT3, -1, NOT_A_TIMER, 0},  // 14 - PJ.3/ACLK/TCK/CB9
	{P1, BIT6, -1, T0D0, PM_TD0CCR0A},  // 15 - P1.6/PM_TD0.0
	{P1, BIT7, -1, T0D1, PM_TD0CCR1A},  // 16 - P1.7/PM_TD0.1
	{P2, BIT0, -1, T0D2, PM_TD0CCR2A},  // 17 - P2.0/PM_TD0.2
	{P2, BIT1, -1, T1D0, PM_TD1CCR0A},  // 18 - P2.1/PM_TD1.0
	{P2, BIT2, -1, T1D1, PM_TD1CCR1A},  // 19 - P2.2/PM_TD1.1
	{P2, BIT3, -1, T1D2, PM_TD1CCR2A},  // 20 - P2.3/PM_TD1.2
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 21 - DVIO
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 22 - DVss
	{P2, BIT4, -1, T0D0, PM_TD0CCR0A},  // 23 - P2.4/PM_TEC0CLR/PM_TEC0FLT2/PM_TD0.0
	{P2, BIT5, -1, T0D1, PM_TD0CCR1A},  // 24 - P2.5/PM_TEC0FLT0/PM_TD0.1
	{P2, BIT6, -1, T0D2, PM_TD0CCR2A},  // 25 - P2.6/PM_TEC0FLT1/PM_TD0.2
	{P2, BIT7, -1, T1D0, PM_TD1CCR0A},  // 26 - P2.7/PM_TEC1CLR/PM_TEC1FLT1/PM_TD1.0
	{P3, BIT0, -1, T1D1, PM_TD1CCR1A},  // 27 - P3.0/PM_TEC1FLT2/PM_TD1.1
	{P3, BIT1, -1, T1D2, PM_TD1CCR2A},  // 28 - P3.1/PM_TEC1FLT0/PM_TD1.2
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 29 - VCore
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 30 - DVss
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 31 - DVcc
	{PJ, BIT6, -1, T0D1, 0},  // 32 - PJ.6/TD1CLK/TD0.1/CB15
	{P3, BIT2, -1, T0D0, PM_TD0CCR0A},  // 33 - P3.2/PM_TD0.0/PM_SMCLK/CB14
	{P3, BIT3, -1, NOT_A_TIMER, 0},  // 34 - P3.3/PM_TA0CLK/PM_CBOUT/CB13
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 35 - TEST/SBWTCK
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 36 - RESET/NMI/SBWTDIO
	{P3, BIT5, 8, T0A2, PM_TA0CCR2A},  // 37 - P3.5/PM_TA0.2/A8/VEREF+/CB12
	{P3, BIT6, 7, T0A1, PM_TA0CCR1A},  // 38 - P3.6/PM_TA0.1/A7/VEREF-/CB11
};
#else
extern const gpio_pin_definition_t chip_pins[];
#endif /* ARDUINO_MAIN */

#define CHIP_PIN_COUNT 38

// ADC pins
static const uint16_t A4 = 9 << 8;
static const uint16_t A5 = 10 << 8;
static const uint16_t A7 = 38 << 8;
static const uint16_t A8 = 37 << 8;
static const uint16_t A0 = 5 << 8;
static const uint16_t A1 = 6 << 8;
static const uint16_t A2 = 7 << 8;
static const uint16_t A3 = 8 << 8;

// Px_y definitions
static const uint16_t P2_2 = 19 << 8;
static const uint16_t P1_7 = 16 << 8;
static const uint16_t P2_3 = 20 << 8;
static const uint16_t PJ_0 = 11 << 8;
static const uint16_t P2_4 = 23 << 8;
static const uint16_t PJ_1 = 12 << 8;
static const uint16_t P3_0 = 27 << 8;
static const uint16_t P2_5 = 24 << 8;
static const uint16_t PJ_2 = 13 << 8;
static const uint16_t P3_1 = 28 << 8;
static const uint16_t P2_6 = 25 << 8;
static const uint16_t PJ_3 = 14 << 8;
static const uint16_t P3_2 = 33 << 8;
static const uint16_t P2_7 = 26 << 8;
static const uint16_t PJ_4 = 2 << 8;
static const uint16_t P3_3 = 34 << 8;
static const uint16_t P1_0 = 5 << 8;
static const uint16_t PJ_5 = 3 << 8;
static const uint16_t PJ_6 = 32 << 8;
static const uint16_t P1_1 = 6 << 8;
static const uint16_t P3_5 = 37 << 8;
static const uint16_t P1_2 = 7 << 8;
static const uint16_t P3_6 = 38 << 8;
static const uint16_t P1_3 = 8 << 8;
static const uint16_t P1_4 = 9 << 8;
static const uint16_t P2_0 = 17 << 8;
static const uint16_t P1_5 = 10 << 8;
static const uint16_t P2_1 = 18 << 8;
static const uint16_t P1_6 = 15 << 8;

/* USCI configuration */
#define USCI_A_COUNT 1
#define USCI_B_COUNT 1

#ifdef ARDUINO_MAIN
const usci_pin_definition_t uscia_pins[] = {
	// TXD << 8 | PORT_SELECTIONx, RXD << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx, PMAP_TXD << 8 | PMAP_RXD, PMAP_SPISTE << 8 | PMAP_SPICLK
	{6 << 8 | PORT_SELECTION0, 7 << 8 | PORT_SELECTION0, 8 << 8 | PORT_SELECTION0, 5 << 8 | PORT_SELECTION0, PM_UCA0TXD << 8 | PM_UCA0RXD, PM_UCA0STE << 8 | PM_UCA0CLK}
};

const usci_pin_definition_t uscib_pins[] = {
	// MOSI/SDA << 8 | PORT_SELECTIONx, MISO/SCL << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx, PMAP_MOSI << 8 | PMAP_MISO, PMAP_SPISTE << 8 | PMAP_SPICLK
	{9 << 8 | PORT_SELECTION0, 10 << 8 | PORT_SELECTION0, 5 << 8 | PORT_SELECTION0, 8 << 8 | PORT_SELECTION0, PM_UCB0SOMI << 8 | PM_UCB0SIMO, PM_UCB0STE << 8 | PM_UCB0CLK}
};
#else
extern const usci_pin_definition_t uscia_pins[];
extern const usci_pin_definition_t uscib_pins[];
#endif /* ARDUINO_MAIN */



#endif /* CHIP_MSP430F5172DA_H */
