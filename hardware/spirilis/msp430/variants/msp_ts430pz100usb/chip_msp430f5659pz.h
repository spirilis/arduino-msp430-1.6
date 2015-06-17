/* Chip-specific pin information - MSP430F5659PZ (100-pin LQFP) */

#ifndef CHIP_MSP430F5659PZ_H
#define CHIP_MSP430F5659PZ_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>


/* Clock system - Clockable Peripherals */
#define CPUFREQ_CHIP_SPECIFIC_PERIPH_MASK	(PERIPH_TIMERA0 | PERIPH_TIMERA1 | PERIPH_TIMERA2 | \
						 PERIPH_TIMERB0 |				\
                                                 PERIPH_USCI_A0 | PERIPH_USCI_B0 |		\
                                                 PERIPH_USCI_A1 | PERIPH_USCI_B1 |		\
                                                 PERIPH_USCI_A2 | PERIPH_USCI_B2)

#define CPUFREQ_CHIP_SPECIFIC_PERIPH_COUNT	10

/* Clock system - SMCLK, ACLK output pins */
#define CLKOUT_SMCLK_PORT		P3IN
#define CLKOUT_SMCLK_PIN		BIT4

#define CLKOUT_ACLK_PORT		P1IN
#define CLKOUT_ACLK_PIN			BIT0

/* Clock system - LFXT pins */
#define UCS_LFXT_PORT			0
#define UCS_LFXT_XIN			BIT0
#define UCS_LFXT_XOUT			BIT0

/* Clock system - HFXT (XT2) pins */
#define UCS_HFXT_PORT			P7IN
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
	{&P2IN, &P2OUT, &P2DIR, &P2REN, &P2SEL, &P2MAP0, &P2IES, &P2IE, &P2IFG},
	{&P3IN, &P3OUT, &P3DIR, &P3REN, &P3SEL, NULL, NULL, NULL, NULL},
	{&P4IN, &P4OUT, &P4DIR, &P4REN, &P4SEL, NULL, NULL, NULL, NULL},
	{&P5IN, &P5OUT, &P5DIR, &P5REN, &P5SEL, NULL, NULL, NULL, NULL},
	{&P6IN, &P6OUT, &P6DIR, &P6REN, &P6SEL, NULL, NULL, NULL, NULL},
	{&P7IN, &P7OUT, &P7DIR, &P7REN, &P7SEL, NULL, NULL, NULL, NULL},
	{&P8IN, &P8OUT, &P8DIR, &P8REN, &P8SEL, NULL, NULL, NULL, NULL},
	{&P9IN, &P9OUT, &P9DIR, &P9REN, &P9SEL, NULL, NULL, NULL, NULL},  // P9 doesn't exist on F5659
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P10 doesn't exist on F5659
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL, NULL},  // P11 doesn't exist on F5659
	{&PJIN, &PJOUT, &PJDIR, &PJREN, NULL  , NULL, NULL, NULL, NULL}
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
	{P6, BIT4, 4, NOT_A_TIMER, 0},  // 1 - P6.4/CB4/A4
	{P6, BIT5, 5, NOT_A_TIMER, 0},  // 2 - P6.5/CB5/A5
	{P6, BIT6, 6, NOT_A_TIMER, 0},  // 3 - P6.6/CB6/A6/DAC0
	{P6, BIT7, 7, NOT_A_TIMER, 0},  // 4 - P6.7/CB7/A7/DAC1
	{P7, BIT4, 12, NOT_A_TIMER, 0},  // 5 - P7.4/CB8/A12
	{P7, BIT5, 13, NOT_A_TIMER, 0},  // 6 - P7.5/CB9/A13
	{P7, BIT6, 14, NOT_A_TIMER, 0},  // 7 - P7.6/CB10/A14/DAC0
	{P7, BIT7, 15, NOT_A_TIMER, 0},  // 8 - P7.7/CB11/A15/DAC1
	{P5, BIT0, -1, NOT_A_TIMER, 0},  // 9 - P5.0/VREF+/VeREF+
	{P5, BIT1, -1, NOT_A_TIMER, 0},  // 10 - P5.1/VREF-/VeREF-
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 11 - AVcc1
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 12 - AVss1
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 13 - XIN
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 14 - XOUT
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 15 - AVss2
	{P5, BIT6, -1, NOT_A_TIMER, 0},  // 16 - P5.6/ADC12CLK/DMAE0
	{P2, BIT0, -1, NOT_A_TIMER, 0},  // 17 - P2.0/P2MAP0
	{P2, BIT1, -1, NOT_A_TIMER, 0},  // 18 - P2.1/P2MAP1
	{P2, BIT2, -1, NOT_A_TIMER, 0},  // 19 - P2.2/P2MAP2
	{P2, BIT3, -1, NOT_A_TIMER, 0},  // 20 - P2.3/P2MAP3
	{P2, BIT4, -1, NOT_A_TIMER, 0},  // 21 - P2.4/P2MAP4
	{P2, BIT5, -1, NOT_A_TIMER, 0},  // 22 - P2.5/P2MAP5
	{P2, BIT6, -1, NOT_A_TIMER, 0},  // 23 - P2.6/P2MAP6
	{P2, BIT7, -1, NOT_A_TIMER, 0},  // 24 - P2.7/P2MAP7
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 25 - DVcc1
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 26 - DVss1
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 27 - VCore
	{P5, BIT2, -1, NOT_A_TIMER, 0},  // 28 - P5.2
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 29 - DVss
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 30 - DNC
	{P5, BIT3, -1, NOT_A_TIMER, 0},  // 31 - P5.3
	{P5, BIT4, -1, NOT_A_TIMER, 0},  // 32 - P5.4
	{P5, BIT5, -1, NOT_A_TIMER, 0},  // 33 - P5.5
	{P1, BIT0, -1, NOT_A_TIMER, 0},  // 34 - P1.0/TA0CLK/ACLK
	{P1, BIT1, -1, T0A0, 0},  // 35 - P1.1/TA0.0
	{P1, BIT2, -1, T0A1, 0},  // 36 - P1.2/TA0.1
	{P1, BIT3, -1, T0A2, 0},  // 37 - P1.3/TA0.2
	{P1, BIT4, -1, T0A3, 0},  // 38 - P1.4/TA0.3
	{P1, BIT5, -1, T0A4, 0},  // 39 - P1.5/TA0.4
	{P1, BIT6, -1, T0A1, 0},  // 40 - P1.6/TA0.1
	{P1, BIT7, -1, T0A2, 0},  // 41 - P1.7/TA0.2
	{P3, BIT0, -1, NOT_A_TIMER, 0},  // 42 - P3.0/TA1CLK/CBOUT
	{P3, BIT1, -1, T1A0, 0},  // 43 - P3.1/TA1.0
	{P3, BIT2, -1, T1A1, 0},  // 44 - P3.2/TA1.1
	{P3, BIT3, -1, T1A2, 0},  // 45 - P3.3/TA1.2
	{P3, BIT4, -1, NOT_A_TIMER, 0},  // 46 - P3.4/TA2CLK/SMCLK
	{P3, BIT5, -1, T2A0, 0},  // 47 - P3.5/TA2.0
	{P3, BIT6, -1, T2A1, 0},  // 48 - P3.6/TA2.1
	{P3, BIT7, -1, T2A2, 0},  // 49 - P3.7/TA2.2
	{P4, BIT0, -1, T0B0, 0},  // 50 - P4.0/TB0.0
	{P4, BIT1, -1, T0B1, 0},  // 51 - P4.1/TB0.1
	{P4, BIT2, -1, T0B2, 0},  // 52 - P4.2/TB0.2
	{P4, BIT3, -1, T0B3, 0},  // 53 - P4.3/TB0.3
	{P4, BIT4, -1, T0B4, 0},  // 54 - P4.4/TB0.4
	{P4, BIT5, -1, T0B5, 0},  // 55 - P4.5/TB0.5
	{P4, BIT6, -1, T0B6, 0},  // 56 - P4.6/TB0.6
	{P4, BIT7, -1, NOT_A_TIMER, 0},  // 57 - P4.7/TB0OUTH/SVMOUT
	{P8, BIT0, -1, NOT_A_TIMER, 0},  // 58 - P8.0/TB0CLK
	{P8, BIT1, -1, NOT_A_TIMER, 0},  // 59 - P8.1/UCB1STE/UCA1CLK
	{P8, BIT2, -1, NOT_A_TIMER, 0},  // 60 - P8.2/UCA1TXD/UCA1SIMO
	{P8, BIT3, -1, NOT_A_TIMER, 0},  // 61 - P8.3/UCA1RXD/UCA1SOMI
	{P8, BIT4, -1, NOT_A_TIMER, 0},  // 62 - P8.4/UCB1CLK/UCA1STE
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 63 - DVss2
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 64 - DVcc2
	{P8, BIT5, -1, NOT_A_TIMER, 0},  // 65 - P8.5/UCB1SIMO/UCB1SDA
	{P8, BIT6, -1, NOT_A_TIMER, 0},  // 66 - P8.6/UCB1SOMI/UCB1SCL
	{P8, BIT7, -1, NOT_A_TIMER, 0},  // 67 - P8.7
	{P9, BIT0, -1, NOT_A_TIMER, 0},  // 68 - P9.0
	{P9, BIT1, -1, NOT_A_TIMER, 0},  // 69 - P9.1/UCB2STE/UCA2CLK
	{P9, BIT2, -1, NOT_A_TIMER, 0},  // 70 - P9.2/UCA2TXD/UCA2SIMO
	{P9, BIT3, -1, NOT_A_TIMER, 0},  // 71 - P9.3/UCA2RXD/UCA2SOMI
	{P9, BIT4, -1, NOT_A_TIMER, 0},  // 72 - P9.4/UCB2CLK/UCA2STE
	{P9, BIT5, -1, NOT_A_TIMER, 0},  // 73 - P9.5/UCB2SIMO/UCB2SDA
	{P9, BIT6, -1, NOT_A_TIMER, 0},  // 74 - P9.6/UCB2SOMI/UCB2SCL
	{P9, BIT7, -1, NOT_A_TIMER, 0},  // 75 - P9.7
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 76 - VSSU
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 77 - PU.0/DP
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 78 - PUR
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 79 - PU.1/DM
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 80 - VBUS
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 81 - VUSB
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 82 - V18
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 83 - AVss3
	{P7, BIT2, -1, NOT_A_TIMER, 0},  // 84 - P7.2/XT2IN
	{P7, BIT3, -1, NOT_A_TIMER, 0},  // 85 - P7.3/XT2OUT
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 86 - VBAK
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 87 - VBAT
	{P5, BIT7, -1, NOT_A_TIMER, 0},  // 88 - P5.7/RTCCLK
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 89 - DVcc3
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 90 - DVss3
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 91 - TEST/SBWTCK
	{PJ, BIT0, -1, NOT_A_TIMER, 0},  // 92 - PJ.0/TDO
	{PJ, BIT1, -1, NOT_A_TIMER, 0},  // 93 - PJ.1`/TDI/TCLK
	{PJ, BIT2, -1, NOT_A_TIMER, 0},  // 94 - PJ.2/TMS
	{PJ, BIT3, -1, NOT_A_TIMER, 0},  // 95 - PJ.3/TCK
	{NOT_A_PORT, 0, -1, NOT_A_TIMER, 0},  // 96 - RESET/NMI/SBWTDIO
	{P6, BIT0, 0, NOT_A_TIMER, 0},  // 97 - P6.0/CB0/A0
	{P6, BIT1, 1, NOT_A_TIMER, 0},  // 98 - P6.1/CB1/A1
	{P6, BIT2, 2, NOT_A_TIMER, 0},  // 99 - P6.2/CB2/A2
	{P6, BIT3, 3, NOT_A_TIMER, 0},  // 100 - P6.3/CB3/A3
};
#else
extern const gpio_pin_definition_t chip_pins[];
#endif /* ARDUINO_MAIN */

#define CHIP_PIN_COUNT 100

// ADC pins
static const uint16_t A4 = 1 << 8;
static const uint16_t A5 = 2 << 8;
static const uint16_t A6 = 3 << 8;
static const uint16_t A7 = 4 << 8;
static const uint16_t A12 = 5 << 8;
static const uint16_t A0 = 97 << 8;
static const uint16_t A13 = 6 << 8;
static const uint16_t A1 = 98 << 8;
static const uint16_t A14 = 7 << 8;
static const uint16_t A2 = 99 << 8;
static const uint16_t A15 = 8 << 8;
static const uint16_t A3 = 100 << 8;

// Px_y definitions
static const uint16_t P4_0 = 50 << 8;
static const uint16_t P4_1 = 51 << 8;
static const uint16_t P4_2 = 52 << 8;
static const uint16_t P4_3 = 53 << 8;
static const uint16_t P4_4 = 54 << 8;
static const uint16_t P4_5 = 55 << 8;
static const uint16_t P4_6 = 56 << 8;
static const uint16_t P4_7 = 57 << 8;
static const uint16_t P9_0 = 68 << 8;
static const uint16_t P9_1 = 69 << 8;
static const uint16_t P9_2 = 70 << 8;
static const uint16_t P9_3 = 71 << 8;
static const uint16_t P9_4 = 72 << 8;
static const uint16_t P9_5 = 73 << 8;
static const uint16_t P9_6 = 74 << 8;
static const uint16_t P9_7 = 75 << 8;
static const uint16_t P5_0 = 9 << 8;
static const uint16_t P5_1 = 10 << 8;
static const uint16_t P5_2 = 28 << 8;
static const uint16_t P5_3 = 31 << 8;
static const uint16_t P5_4 = 32 << 8;
static const uint16_t P5_5 = 33 << 8;
static const uint16_t P5_6 = 16 << 8;
static const uint16_t P5_7 = 88 << 8;
static const uint16_t P1_0 = 34 << 8;
static const uint16_t P1_1 = 35 << 8;
static const uint16_t P1_2 = 36 << 8;
static const uint16_t P1_3 = 37 << 8;
static const uint16_t P1_4 = 38 << 8;
static const uint16_t P1_5 = 39 << 8;
static const uint16_t P1_6 = 40 << 8;
static const uint16_t P1_7 = 41 << 8;
static const uint16_t P6_0 = 97 << 8;
static const uint16_t P6_1 = 98 << 8;
static const uint16_t P6_2 = 99 << 8;
static const uint16_t P6_3 = 100 << 8;
static const uint16_t P6_4 = 1 << 8;
static const uint16_t P6_5 = 2 << 8;
static const uint16_t P6_6 = 3 << 8;
static const uint16_t P6_7 = 4 << 8;
static const uint16_t P2_0 = 17 << 8;
static const uint16_t P2_1 = 18 << 8;
static const uint16_t P2_2 = 19 << 8;
static const uint16_t P2_3 = 20 << 8;
static const uint16_t P2_4 = 21 << 8;
static const uint16_t P2_5 = 22 << 8;
static const uint16_t P2_6 = 23 << 8;
static const uint16_t P2_7 = 24 << 8;
static const uint16_t P7_2 = 84 << 8;
static const uint16_t P7_3 = 85 << 8;
static const uint16_t P7_4 = 5 << 8;
static const uint16_t P7_5 = 6 << 8;
static const uint16_t P7_6 = 7 << 8;
static const uint16_t P7_7 = 8 << 8;
static const uint16_t P3_0 = 42 << 8;
static const uint16_t P3_1 = 43 << 8;
static const uint16_t P3_2 = 44 << 8;
static const uint16_t P3_3 = 45 << 8;
static const uint16_t P3_4 = 46 << 8;
static const uint16_t P3_5 = 47 << 8;
static const uint16_t P3_6 = 48 << 8;
static const uint16_t P3_7 = 49 << 8;
static const uint16_t PJ_0 = 92 << 8;
static const uint16_t PJ_1 = 93 << 8;
static const uint16_t PJ_2 = 94 << 8;
static const uint16_t PJ_3 = 95 << 8;
static const uint16_t P8_0 = 58 << 8;
static const uint16_t P8_1 = 59 << 8;
static const uint16_t P8_2 = 60 << 8;
static const uint16_t P8_3 = 61 << 8;
static const uint16_t P8_4 = 62 << 8;
static const uint16_t P8_5 = 65 << 8;
static const uint16_t P8_6 = 66 << 8;
static const uint16_t P8_7 = 67 << 8;


/* USCI configuration */
#define USCI_A_COUNT 3
#define USCI_B_COUNT 3

#ifdef ARDUINO_MAIN
const usci_pin_definition_t uscia_pins[] = {
	// TXD << 8 | PORT_SELECTIONx, RXD << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx, PMAP_TXD << 8 | PMAP_RXD, PMAP_SPISTE << 8 | PMAP_SPICLK
	{17 << 8 | PORT_SELECTION0, 18 << 8 | PORT_SELECTION0, 19 << 8 | PORT_SELECTION0, 20 << 8 | PORT_SELECTION0, PM_UCA0TXD << 8 | PM_UCA0RXD, PM_UCA0STE << 8 | PM_UCA0CLK},
	{60 << 8 | PORT_SELECTION0, 61 << 8 | PORT_SELECTION0, 62 << 8 | PORT_SELECTION0, 59 << 8 | PORT_SELECTION0, 0, 0},
	{70 << 8 | PORT_SELECTION0, 71 << 8 | PORT_SELECTION0, 72 << 8 | PORT_SELECTION0, 69 << 8 | PORT_SELECTION0, 0, 0}
};

const usci_pin_definition_t uscib_pins[] = {
	// MOSI/SDA << 8 | PORT_SELECTIONx, MISO/SCL << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx, PMAP_MOSI << 8 | PMAP_MISO, PMAP_SPISTE << 8 | PMAP_SPICLK
	{21 << 8 | PORT_SELECTION0, 22 << 8 | PORT_SELECTION0, 23 << 8 | PORT_SELECTION0, 24 << 8 | PORT_SELECTION0, PM_UCB0SIMO << 8 | PM_UCB0SOMI, PM_UCB0STE << 8 | PM_UCB0CLK},
	{65 << 8 | PORT_SELECTION0, 66 << 8 | PORT_SELECTION0, 59 << 8 | PORT_SELECTION0, 62 << 8 | PORT_SELECTION0, 0, 0},
	{73 << 8 | PORT_SELECTION0, 74 << 8 | PORT_SELECTION0, 69 << 8 | PORT_SELECTION0, 72 << 8 | PORT_SELECTION0, 0, 0}
};
#else
extern const usci_pin_definition_t uscia_pins[];
extern const usci_pin_definition_t uscib_pins[];
#endif /* ARDUINO_MAIN */



#endif /* CHIP_MSP430F5659PN_H */

