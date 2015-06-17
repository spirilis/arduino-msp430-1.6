/* Chip-specific pin information - MSP430F5529PN (80-pin LQFP) */

#ifndef CHIP_MSP430F5529PN_H
#define CHIP_MSP430F5529PN_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>


/* Clock system - Clockable Peripherals */
#define CPUFREQ_CHIP_SPECIFIC_PERIPH_MASK	(PERIPH_TIMERA0 |                               \
						 PERIPH_TIMERA1 |                               \
						 PERIPH_TIMERA2 |                               \
						 PERIPH_TIMERB0 |                               \
						 PERIPH_USCI_A0 | PERIPH_USCI_B0 |              \
						 PERIPH_USCI_A1 | PERIPH_USCI_B1)

#define CPUFREQ_CHIP_SPECIFIC_PERIPH_COUNT	8

/* Clock system - SMCLK, ACLK output pins */
#define CLKOUT_SMCLK_PORT		P2IN
#define CLKOUT_SMCLK_PIN		BIT2

#define CLKOUT_ACLK_PORT		P1IN
#define CLKOUT_ACLK_PIN			BIT0

/* Clock system - LFXT pins */
#define UCS_LFXT_PORT			P5IN
#define UCS_LFXT_XIN			BIT4
#define UCS_LFXT_XOUT			BIT5

/* Clock system - HFXT (XT2) pins */
#define UCS_HFXT_PORT			P5IN
#define UCS_HFXT_XIN			BIT2
#define UCS_HFXT_XOUT			BIT3

/* Clock system - VLO frequency */
#define CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY	9400UL


#ifdef ARDUINO_MAIN
/* GPIO port definitions */
const gpio_port_definition_t chip_ports[] = {
	/* F5xxx gpio_port_definition_t layout:
	 * portin, portout, portdir, portren, portsel, portmap, porties, portie, portifg
	 */
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // filler for index #0
	{&P1IN, &P1OUT, &P1DIR, &P1REN, &P1SEL, NULL, &P1IES, &P1IE, &P1IFG},
	{&P2IN, &P2OUT, &P2DIR, &P2REN, &P2SEL, NULL, &P2IES, &P2IE, &P2IFG},
	{&P3IN, &P3OUT, &P3DIR, &P3REN, &P3SEL, NULL, NULL, NULL, NULL},
	{&P4IN, &P4OUT, &P4DIR, &P4REN, &P4SEL, &P4MAP0, NULL, NULL, NULL},
	{&P5IN, &P5OUT, &P5DIR, &P5REN, &P5SEL, NULL, NULL, NULL, NULL},
	{&P6IN, &P6OUT, &P6DIR, &P6REN, &P6SEL, NULL, NULL, NULL, NULL},
	{&P7IN, &P7OUT, &P7DIR, &P7REN, &P7SEL, NULL, NULL, NULL, NULL},
	{&P8IN, &P8OUT, &P8DIR, &P8REN, &P8SEL, NULL, NULL, NULL, NULL},
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P9 doesn't exist on F5529
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P10 doesn't exist on F5529
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P11 doesn't exist on F5529
	{&PJIN, &PJOUT, &PJDIR, &PJREN, NULL  , NULL, NULL, NULL, NULL}
};
#else
extern const gpio_port_definition_t chip_ports[];
#endif


#ifdef ARDUINO_MAIN
/* GPIO pin definitions - generated from awk script
 *
 * Fields: <P1/P2/etc>, <Port Pin#>, <ADC chan#>, <NOT_A_TIMER | TxNy enum>, <Timer PMAP_ID>
 */
const gpio_pin_definition_t chip_pins[] = {
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // filler for index #0
	{P6, BIT4, 4, NOT_A_TIMER, 0},  // 1 - P6.4/CB4/A4
	{P6, BIT5, 5, NOT_A_TIMER, 0},  // 2 - P6.5/CB5/A5
	{P6, BIT6, 6, NOT_A_TIMER, 0},  // 3 - P6.6/CB6/A6
	{P6, BIT7, 7, NOT_A_TIMER, 0},  // 4 - P6.7/CB7/A7
	{P7, BIT0, 12, NOT_A_TIMER, 0},  // 5 - P7.0/CB8/A12
	{P7, BIT1, 13, NOT_A_TIMER, 0},  // 6 - P7.1/CB9/A13
	{P7, BIT2, 14, NOT_A_TIMER, 0},  // 7 - P7.2/CB10/A14
	{P7, BIT3, 15, NOT_A_TIMER, 0},  // 8 - P7.3/CB11/A15
	{P5, BIT0, 8, NOT_A_TIMER, 0},  // 9 - P5.0/A8/VREF+/VeREF+
	{P5, BIT1, 9, NOT_A_TIMER, 0},  // 10 - P5.1/A9/VREF-/VeREF-
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 11 - AVCC1
	{P5, BIT4, -1, NOT_A_TIMER, 0},  // 12 - P5.4/XIN
	{P5, BIT5, -1, NOT_A_TIMER, 0},  // 13 - P5.5/XOUT
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 14 - AVSS1
	{P8, BIT0, -1, NOT_A_TIMER, 0},  // 15 - P8.0
	{P8, BIT1, -1, NOT_A_TIMER, 0},  // 16 - P8.1
	{P8, BIT2, -1, NOT_A_TIMER, 0},  // 17 - P8.2
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 18 - DVCC1
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 19 - DVSS1
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 20 - VCORE
	{P1, BIT0, -1, NOT_A_TIMER, 0},  // 21 - P1.0/TA0CLK/ACLK
	{P1, BIT1, -1, T0A0, 0},  // 22 - P1.1/TA0.0
	{P1, BIT2, -1, T0A1, 0},  // 23 - P1.2/TA0.1
	{P1, BIT3, -1, T0A2, 0},  // 24 - P1.3/TA0.2
	{P1, BIT4, -1, T0A3, 0},  // 25 - P1.4/TA0.3
	{P1, BIT5, -1, T0A4, 0},  // 26 - P1.5/TA0.4
	{P1, BIT6, -1, NOT_A_TIMER, 0},  // 27 - P1.6/TA1CLK/CBOUT
	{P1, BIT7, -1, T1A0, 0},  // 28 - P1.7/TA1.0
	{P2, BIT0, -1, T1A1, 0},  // 29 - P2.0/TA1.1
	{P2, BIT1, -1, T1A2, 0},  // 30 - P2.1/TA1.2
	{P2, BIT2, -1, NOT_A_TIMER, 0},  // 31 - P2.2/TA2CLK/SMCLK
	{P2, BIT3, -1, T2A0, 0},  // 32 - P2.3/TA2.0
	{P2, BIT4, -1, T2A1, 0},  // 33 - P2.4/TA2.1
	{P2, BIT5, -1, T2A2, 0},  // 34 - P2.5/TA2.2
	{P2, BIT6, -1, NOT_A_TIMER, 0},  // 35 - P2.6/RTCCLK/DMAE0
	{P2, BIT7, -1, NOT_A_TIMER, 0},  // 36 - P2.7/UCB0STE/UCA0CLK
	{P3, BIT0, -1, NOT_A_TIMER, 0},  // 37 - P3.0/UCB0SIMO/UCB0SDA
	{P3, BIT1, -1, NOT_A_TIMER, 0},  // 38 - P3.1/UCB0SOMI/UCB0SCL
	{P3, BIT2, -1, NOT_A_TIMER, 0},  // 39 - P3.2/UCB0CLK/UCA0STE
	{P3, BIT3, -1, NOT_A_TIMER, 0},  // 40 - P3.3/UCA0TXD/UCA0SIMO
	{P3, BIT4, -1, NOT_A_TIMER, 0},  // 41 - P3.4/UCA0RXD/UCA0SOMI
	{P3, BIT5, -1, T0B5, 0},  // 42 - P3.5/TB0.5
	{P3, BIT6, -1, T0B6, 0},  // 43 - P3.6/TB0.6
	{P3, BIT7, -1, NOT_A_TIMER, 0},  // 44 - P3.7/TB0OUTH/SVMOUT
	{P4, BIT0, -1, NOT_A_TIMER, 0},  // 45 - P4.0/PM_UCB1STE/PM_UCA1CLK
	{P4, BIT1, -1, NOT_A_TIMER, 0},  // 46 - P4.1/PM_UCB1SIMO/PM_UCB1SDA
	{P4, BIT2, -1, NOT_A_TIMER, 0},  // 47 - P4.2/PM_UCB1SOMI/PM_UCB1SCL
	{P4, BIT3, -1, NOT_A_TIMER, 0},  // 48 - P4.3/PM_UCB1CLK/PM_UCA1STE
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 49 - DVSS2
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 50 - DVCC2
	{P4, BIT4, -1, NOT_A_TIMER, 0},  // 51 - P4.4/PM_UCA1TXD/PM_UCA1SIMO
	{P4, BIT5, -1, NOT_A_TIMER, 0},  // 52 - P4.5/PM_UCA1RXD/PM_UCA1SOMI
	{P4, BIT6, -1, T0B5, PM_TB0CCR5A},  // 53 - P4.6/PM_NONE
	{P4, BIT7, -1, T0B6, PM_TB0CCR6A},  // 54 - P4.7/PM_NONE
	{P5, BIT6, -1, T0B0, 0},  // 55 - P5.6/TB0.0
	{P5, BIT7, -1, T0B1, 0},  // 56 - P5.7/TB0.1
	{P7, BIT4, -1, T0B2, 0},  // 57 - P7.4/TB0.2
	{P7, BIT5, -1, T0B3, 0},  // 58 - P7.5/TB0.3
	{P7, BIT6, -1, T0B4, 0},  // 59 - P7.6/TB0.4
	{P7, BIT7, -1, NOT_A_TIMER, 0},  // 60 - P7.7/TB0CLK/MCLK
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 61 - VSSU
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 62 - PU.0/DP
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 63 - PUR
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 64 - PU.1/DM
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 65 - VBUS
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 66 - VUSB
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 67 - V18
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 68 - AVSS2
	{P5, BIT2, -1, NOT_A_TIMER, 0},  // 69 - P5.2/XT2IN
	{P5, BIT3, -1, NOT_A_TIMER, 0},  // 70 - P5.3/XT2OUT
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 71 - TEST/SBWTCK
	{PJ, BIT0, -1, NOT_A_TIMER, 0},  // 72 - PJ.0/TDO
	{PJ, BIT1, -1, NOT_A_TIMER, 0},  // 73 - PJ.1/TDI/TCLK
	{PJ, BIT2, -1, NOT_A_TIMER, 0},  // 74 - PJ.2/TMS
	{PJ, BIT3, -1, NOT_A_TIMER, 0},  // 75 - PJ.3/TCK
	{NOT_A_PORT, 0, 0, NOT_A_TIMER, 0},  // 76 - RESET/NMI/SBWTDIO
	{P6, BIT0, 0, NOT_A_TIMER, 0},  // 77 - P6.0/CB0/A0
	{P6, BIT1, 1, NOT_A_TIMER, 0},  // 78 - P6.1/CB1/A1
	{P6, BIT2, 2, NOT_A_TIMER, 0},  // 79 - P6.2/CB2/A2
	{P6, BIT3, 3, NOT_A_TIMER, 0},  // 80 - P6.3/CB3/A3
};
#else
extern const gpio_pin_definition_t chip_pins[];
#endif /* ARDUINO_MAIN */

#define CHIP_PIN_COUNT 80

// ADC pins
static const uint16_t A4 = 1 << 8;
static const uint16_t A5 = 2 << 8;
static const uint16_t A6 = 3 << 8;
static const uint16_t A7 = 4 << 8;
static const uint16_t A8 = 9 << 8;
static const uint16_t A9 = 10 << 8;
static const uint16_t A12 = 5 << 8;
static const uint16_t A0 = 77 << 8;
static const uint16_t A13 = 6 << 8;
static const uint16_t A1 = 78 << 8;
static const uint16_t A14 = 7 << 8;
static const uint16_t A2 = 79 << 8;
static const uint16_t A15 = 8 << 8;
static const uint16_t A3 = 80 << 8;

// Px_y definitions
static const uint16_t P4_0 = 45 << 8;
static const uint16_t P4_1 = 46 << 8;
static const uint16_t P4_2 = 47 << 8;
static const uint16_t P4_3 = 48 << 8;
static const uint16_t P4_4 = 51 << 8;
static const uint16_t P4_5 = 52 << 8;
static const uint16_t P4_6 = 53 << 8;
static const uint16_t P4_7 = 54 << 8;
static const uint16_t P5_0 = 9 << 8;
static const uint16_t P5_1 = 10 << 8;
static const uint16_t P5_2 = 69 << 8;
static const uint16_t P5_3 = 70 << 8;
static const uint16_t P5_4 = 12 << 8;
static const uint16_t P5_5 = 13 << 8;
static const uint16_t P5_6 = 55 << 8;
static const uint16_t P5_7 = 56 << 8;
static const uint16_t P1_0 = 21 << 8;
static const uint16_t P1_1 = 22 << 8;
static const uint16_t P1_2 = 23 << 8;
static const uint16_t P1_3 = 24 << 8;
static const uint16_t P1_4 = 25 << 8;
static const uint16_t P1_5 = 26 << 8;
static const uint16_t P1_6 = 27 << 8;
static const uint16_t P1_7 = 28 << 8;
static const uint16_t P6_0 = 77 << 8;
static const uint16_t P6_1 = 78 << 8;
static const uint16_t P6_2 = 79 << 8;
static const uint16_t P6_3 = 80 << 8;
static const uint16_t P6_4 = 1 << 8;
static const uint16_t P6_5 = 2 << 8;
static const uint16_t P6_6 = 3 << 8;
static const uint16_t P6_7 = 4 << 8;
static const uint16_t P2_0 = 29 << 8;
static const uint16_t P2_1 = 30 << 8;
static const uint16_t P2_2 = 31 << 8;
static const uint16_t P2_3 = 32 << 8;
static const uint16_t P2_4 = 33 << 8;
static const uint16_t P2_5 = 34 << 8;
static const uint16_t P2_6 = 35 << 8;
static const uint16_t P2_7 = 36 << 8;
static const uint16_t P7_0 = 5 << 8;
static const uint16_t P7_1 = 6 << 8;
static const uint16_t P7_2 = 7 << 8;
static const uint16_t P7_3 = 8 << 8;
static const uint16_t P7_4 = 57 << 8;
static const uint16_t P7_5 = 58 << 8;
static const uint16_t P7_6 = 59 << 8;
static const uint16_t P7_7 = 60 << 8;
static const uint16_t P3_0 = 37 << 8;
static const uint16_t P3_1 = 38 << 8;
static const uint16_t P3_2 = 39 << 8;
static const uint16_t P3_3 = 40 << 8;
static const uint16_t P3_4 = 41 << 8;
static const uint16_t P3_5 = 42 << 8;
static const uint16_t P3_6 = 43 << 8;
static const uint16_t P3_7 = 44 << 8;
static const uint16_t PJ_0 = 72 << 8;
static const uint16_t PJ_1 = 73 << 8;
static const uint16_t PJ_2 = 74 << 8;
static const uint16_t PJ_3 = 75 << 8;
static const uint16_t P8_0 = 15 << 8;
static const uint16_t P8_1 = 16 << 8;
static const uint16_t P8_2 = 17 << 8;

/* USCI configuration */
#define USCI_A_COUNT 2
#define USCI_B_COUNT 2

#ifdef ARDUINO_MAIN
const usci_pin_definition_t uscia_pins[] = {
	// TXD << 8 | PORT_SELECTIONx, RXD << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx, PMAP_TXD << 8 | PMAP_RXD, PMAP_SPISTE << 8 | PMAP_SPICLK
	{40 << 8 | PORT_SELECTION0, 41 << 8 | PORT_SELECTION0, 39 << 8 | PORT_SELECTION0, 36 << 8 | PORT_SELECTION0, 0, 0},
	{51 << 8 | PORT_SELECTION0, 52 << 8 | PORT_SELECTION0, 48 << 8 | PORT_SELECTION0, 45 << 8 | PORT_SELECTION0, PM_UCA1TXD << 8 | PM_UCA1RXD, PM_UCA1STE << 8 | PM_UCA1CLK},
};

const usci_pin_definition_t uscib_pins[] = {
	// MOSI/SDA << 8 | PORT_SELECTIONx, MISO/SCL << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx, PMAP_MOSI << 8 | PMAP_MISO, PMAP_SPISTE << 8 | PMAP_SPICLK
	{37 << 8 | PORT_SELECTION0, 38 << 8 | PORT_SELECTION0, 36 << 8 | PORT_SELECTION0, 39 << 8 | PORT_SELECTION0, 0, 0},
	{46 << 8 | PORT_SELECTION0, 47 << 8 | PORT_SELECTION0, 45 << 8 | PORT_SELECTION0, 48 << 8 | PORT_SELECTION0, PM_UCB1SIMO << 8 | PM_UCB1SOMI, PM_UCB1STE << 8 | PM_UCB1CLK},
};
#else
extern const usci_pin_definition_t uscia_pins[];
extern const usci_pin_definition_t uscib_pins[];
#endif /* ARDUINO_MAIN */



#endif /* CHIP_MSP430F5529PN_H */
