/* Chip-specific information - MSP430F5438A "PZ" package (100-pin LQFP) */

#ifndef CHIP_MSP430F5438APZ_H
#define CHIP_MSP430F5438APZ_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>


/* Clock system - Clockable Peripherals */
#define CPUFREQ_CHIP_SPECIFIC_PERIPH_MASK	(PERIPH_TIMERA0 | PERIPH_TIMERA1	\
						 PERIPH_TIMERB0	|			\
						 PERIPH_USCI_A0 | PERIPH_USCI_B0	\
						 PERIPH_USCI_A1 | PERIPH_USCI_B1	\
						 PERIPH_USCI_A2 | PERIPH_USCI_B2	\
						 PERIPH_USCI_A3 | PERIPH_USCI_B3)

#define CPUFREQ_CHIP_SPECIFIC_PERIPH_COUNT	11

/* Clock system - SMCLK, ACLK output pins */
#define CLKOUT_SMCLK_PORT		P4IN
#define CLKOUT_SMCLK_PIN		BIT7

#define CLKOUT_ACLK_PORT		P1IN
#define CLKOUT_ACLK_PIN			BIT0

/* Clock system - LFXT pins */
#define UCS_LFXT_PORT			P7IN
#define UCS_LFXT_XIN			BIT0
#define UCS_LFXT_XOUT			BIT1

/* Clock system - HFXT (XT2) pins */
#define UCS_HFXT_PORT			P5IN
#define UCS_HFXT_XIN			BIT2
#define UCS_HFXT_XOUT			BIT3

/* Clock system - VLO frequency */
#define CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY     9400UL


#ifdef ARDUINO_MAIN
/* GPIO port definitions */
const gpio_port_definition_t chip_ports[] = {
	/* F5xxx gpio_port_definition_t layout:
	 * portin, portout, portdir, portren, portsel, porties, portie, portifg
	 */
	{NULL,  NULL,   NULL,   NULL,   NULL,   NULL, NULL, NULL},  // filler for index #0
	{&P1IN, &P1OUT, &P1DIR, &P1REN, &P1SEL, &P1IES, &P1IE, &P1IFG},
	{&P2IN, &P2OUT, &P2DIR, &P2REN, &P2SEL, &P2IES, &P2IE, &P2IFG},
	{&P3IN, &P3OUT, &P3DIR, &P3REN, &P3SEL, NULL, NULL, NULL},
	{&P4IN, &P4OUT, &P4DIR, &P4REN, &P4SEL, NULL, NULL, NULL},
	{&P5IN, &P5OUT, &P5DIR, &P5REN, &P5SEL, NULL, NULL, NULL},
	{&P6IN, &P6OUT, &P6DIR, &P6REN, &P6SEL, NULL, NULL, NULL},
	{&P7IN, &P7OUT, &P7DIR, &P7REN, &P7SEL, NULL, NULL, NULL},
	{&P8IN, &P8OUT, &P8DIR, &P8REN, &P8SEL, NULL, NULL, NULL},
	{&P9IN, &P9OUT, &P9DIR, &P9REN, &P9SEL, NULL, NULL, NULL},
	{&P10IN, &P10OUT, &P10DIR, &P10REN, &P10SEL, NULL, NULL, NULL},
	{&P11IN, &P11OUT, &P11DIR, &P11REN, &P11SEL, NULL, NULL, NULL},
	{&PJIN, &PJOUT, &PJDIR, &PJREN, NULL , NULL, NULL, NULL, NULL}
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
	{P6, BIT4, 4, NOT_A_TIMER},  // 1 - P6.4/A4
	{P6, BIT5, 5, NOT_A_TIMER},  // 2 - P6.5/A5
	{P6, BIT6, 6, NOT_A_TIMER},  // 3 - P6.6/A6
	{P6, BIT7, 7, NOT_A_TIMER},  // 4 - P6.7/A7
	{P7, BIT4, 12, NOT_A_TIMER},  // 5 - P7.4/A12
	{P7, BIT5, 13, NOT_A_TIMER},  // 6 - P7.5/A13
	{P7, BIT6, 14, NOT_A_TIMER},  // 7 - P7.6/A14
	{P7, BIT7, 15, NOT_A_TIMER},  // 8 - P7.7/A15
	{P5, BIT0, 8, NOT_A_TIMER},  // 9 - P5.0/A8/VREF+/VeREF+
	{P5, BIT1, 9, NOT_A_TIMER},  // 10 - P5.1/A9/VREF-/VeREF-
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 11 - AVcc
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 12 - AVss
	{P7, BIT0, -1, NOT_A_TIMER},  // 13 - P7.0/XIN
	{P7, BIT1, -1, NOT_A_TIMER},  // 14 - P7.1/XOUT
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 15 - DVss1
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 16 - DVcc1
	{P1, BIT0, -1, NOT_A_TIMER},  // 17 - P1.0/TA0CLK/ACLK
	{P1, BIT1, -1, T0A0},  // 18 - P1.1/TA0.0
	{P1, BIT2, -1, T0A1},  // 19 - P1.2/TA0.1
	{P1, BIT3, -1, T0A2},  // 20 - P1.3/TA0.2
	{P1, BIT4, -1, T0A3},  // 21 - P1.4/TA0.3
	{P1, BIT5, -1, T0A4},  // 22 - P1.5/TA0.4
	{P1, BIT6, -1, NOT_A_TIMER},  // 23 - P1.6/SMCLK
	{P1, BIT7, -1, NOT_A_TIMER},  // 24 - P1.7
	{P2, BIT0, -1, NOT_A_TIMER},  // 25 - P2.0/TA1CLK/MCLK
	{P2, BIT1, -1, T1A0},  // 26 - P2.1/TA1.0
	{P2, BIT2, -1, T1A1},  // 27 - P2.2/TA1.1
	{P2, BIT3, -1, T1A2},  // 28 - P2.3/TA1.2
	{P2, BIT4, -1, NOT_A_TIMER},  // 29 - P2.4/RTCCLK
	{P2, BIT5, -1, NOT_A_TIMER},  // 30 - P2.5
	{P2, BIT6, -1, NOT_A_TIMER},  // 31 - P2.6/ACLK
	{P2, BIT7, -1, NOT_A_TIMER},  // 32 - P2.7/ADC12CLK/DMAE0
	{P3, BIT0, -1, NOT_A_TIMER},  // 33 - P3.0/UCB0STE/UCA0CLK
	{P3, BIT1, -1, NOT_A_TIMER},  // 34 - P3.1/UCB0SIMO/UCB0SDA
	{P3, BIT2, -1, NOT_A_TIMER},  // 35 - P3.2/UCB0SOMI/UCB0SCL
	{P3, BIT3, -1, NOT_A_TIMER},  // 36 - P3.3/UCB0CLK/UCA0STE
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 37 - DVss3
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 38 - DVcc3
	{P3, BIT4, -1, NOT_A_TIMER},  // 39 - P3.4/UCA0TXD/UCA0SIMO
	{P3, BIT5, -1, NOT_A_TIMER},  // 40 - P3.5/UCA0RXD/UCA0SOMI
	{P3, BIT6, -1, NOT_A_TIMER},  // 41 - P3.6/UCB1STE/UCA1CLK
	{P3, BIT7, -1, NOT_A_TIMER},  // 42 - P3.7/UCB1SIMO/UCB1SDA
	{P4, BIT0, -1, T0B0},  // 43 - P4.0/TB0.0
	{P4, BIT1, -1, T0B1},  // 44 - P4.1/TB0.1
	{P4, BIT2, -1, T0B2},  // 45 - P4.2/TB0.2
	{P4, BIT3, -1, T0B3},  // 46 - P4.3/TB0.3
	{P4, BIT4, -1, T0B4},  // 47 - P4.4/TB0.4
	{P4, BIT5, -1, T0B5},  // 48 - P4.5/TB0.5
	{P4, BIT6, -1, T0B6},  // 49 - P4.6/TB0.6
	{P4, BIT7, -1, NOT_A_TIMER},  // 50 - P4.7/TB0CLK/SMCLK
	{P5, BIT4, -1, NOT_A_TIMER},  // 51 - P5.4/UCB1SOMI/UCB1SCL
	{P5, BIT5, -1, NOT_A_TIMER},  // 52 - P5.5/UCB1CLK/UCA1STE
	{P5, BIT6, -1, NOT_A_TIMER},  // 53 - P5.6/UCA1TXD/UCA1SIMO
	{P5, BIT7, -1, NOT_A_TIMER},  // 54 - P5.7/UCA1RXD/UCA1SOMI
	{P7, BIT2, -1, NOT_A_TIMER},  // 55 - P7.2/TB0OUTH/SVMOUT
	{P7, BIT3, -1, T1A2},  // 56 - P7.3/TA1.2
	{P8, BIT0, -1, T0A0},  // 57 - P8.0/TA0.0
	{P8, BIT1, -1, T0A1},  // 58 - P8.1/TA0.1
	{P8, BIT2, -1, T0A2},  // 59 - P8.2/TA0.2
	{P8, BIT3, -1, T0A3},  // 60 - P8.3/TA0.3
	{P8, BIT4, -1, T0A4},  // 61 - P8.4/TA0.4
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 62 - VCORE
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 63 - DVss2
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 64 - DVcc2
	{P8, BIT5, -1, T1A0},  // 65 - P8.5/TA1.0
	{P8, BIT6, -1, T1A1},  // 66 - P8.6/TA1.1
	{P8, BIT7, -1, NOT_A_TIMER},  // 67 - P8.7
	{P9, BIT0, -1, NOT_A_TIMER},  // 68 - P9.0/UCB2STE/UCA2CLK
	{P9, BIT1, -1, NOT_A_TIMER},  // 69 - P9.1/UCB2SIMO/UCB2SDA
	{P9, BIT2, -1, NOT_A_TIMER},  // 70 - P9.2/UCB2SOMI/UCB2SCL
	{P9, BIT3, -1, NOT_A_TIMER},  // 71 - P9.3/UCB2CLK/UCA2STE
	{P9, BIT4, -1, NOT_A_TIMER},  // 72 - P9.4/UCA2TXD/UCA2SIMO
	{P9, BIT5, -1, NOT_A_TIMER},  // 73 - P9.5/UCA2RXD/UCA2SOMI
	{P9, BIT6, -1, NOT_A_TIMER},  // 74 - P9.6
	{P9, BIT7, -1, NOT_A_TIMER},  // 75 - P9.7
	{P10, BIT0, -1, NOT_A_TIMER},  // 76 - P10.0/UCB3STE/UCA3CLK
	{P10, BIT1, -1, NOT_A_TIMER},  // 77 - P10.1/UCB3SIMO/UCB3SDA
	{P10, BIT2, -1, NOT_A_TIMER},  // 78 - P10.2/UCB3SOMI/UCB3SCL
	{P10, BIT3, -1, NOT_A_TIMER},  // 79 - P10.3/UCB3CLK/UCA3STE
	{P10, BIT4, -1, NOT_A_TIMER},  // 80 - P10.4/UCA3TXD/UCA3SIMO
	{P10, BIT5, -1, NOT_A_TIMER},  // 81 - P10.5/UCA3RXD/UCA3SOMI
	{P10, BIT6, -1, NOT_A_TIMER},  // 82 - P10.6
	{P10, BIT7, -1, NOT_A_TIMER},  // 83 - P10.7
	{P11, BIT0, -1, NOT_A_TIMER},  // 84 - P11.0/ACLK
	{P11, BIT1, -1, NOT_A_TIMER},  // 85 - P11.1/MCLK
	{P11, BIT2, -1, NOT_A_TIMER},  // 86 - P11.2/SMCLK
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 87 - DVcc4
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 88 - DVss4
	{P5, BIT2, -1, NOT_A_TIMER},  // 89 - P5.2/XT2IN
	{P5, BIT3, -1, NOT_A_TIMER},  // 90 - P5.3/XT2OUT
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 91 - TEST/SBWTCK
	{PJ, BIT0, -1, NOT_A_TIMER},  // 92 - PJ.0/TDO
	{PJ, BIT1, -1, NOT_A_TIMER},  // 93 - PJ.1/TDI/TCLK
	{PJ, BIT2, -1, NOT_A_TIMER},  // 94 - PJ.2/TMS
	{PJ, BIT3, -1, NOT_A_TIMER},  // 95 - PJ.3/TCK
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 96 - RESET/NMI/SBWTDIO
	{P6, BIT0, 0, NOT_A_TIMER},  // 97 - P6.0/A0
	{P6, BIT1, 1, NOT_A_TIMER},  // 98 - P6.1/A1
	{P6, BIT2, 2, NOT_A_TIMER},  // 99 - P6.2/A2
	{P6, BIT3, 3, NOT_A_TIMER},  // 100 - P6.3/A3
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
static const uint16_t A8 = 9 << 8;
static const uint16_t A9 = 10 << 8;
static const uint16_t A12 = 5 << 8;
static const uint16_t A0 = 97 << 8;
static const uint16_t A13 = 6 << 8;
static const uint16_t A1 = 98 << 8;
static const uint16_t A14 = 7 << 8;
static const uint16_t A2 = 99 << 8;
static const uint16_t A15 = 8 << 8;
static const uint16_t A3 = 100 << 8;

// Px_y definitions
static const uint16_t P4_0 = 43 << 8;
static const uint16_t P4_1 = 44 << 8;
static const uint16_t P4_2 = 45 << 8;
static const uint16_t P4_3 = 46 << 8;
static const uint16_t P4_4 = 47 << 8;
static const uint16_t P4_5 = 48 << 8;
static const uint16_t P4_6 = 49 << 8;
static const uint16_t P4_7 = 50 << 8;
static const uint16_t P11_0 = 84 << 8;
static const uint16_t P11_1 = 85 << 8;
static const uint16_t P9_0 = 68 << 8;
static const uint16_t P11_2 = 86 << 8;
static const uint16_t P9_1 = 69 << 8;
static const uint16_t P9_2 = 70 << 8;
static const uint16_t P9_3 = 71 << 8;
static const uint16_t P9_4 = 72 << 8;
static const uint16_t P9_5 = 73 << 8;
static const uint16_t P9_6 = 74 << 8;
static const uint16_t P9_7 = 75 << 8;
static const uint16_t P5_0 = 9 << 8;
static const uint16_t P5_1 = 10 << 8;
static const uint16_t P5_2 = 89 << 8;
static const uint16_t P5_3 = 90 << 8;
static const uint16_t P5_4 = 51 << 8;
static const uint16_t P5_5 = 52 << 8;
static const uint16_t P5_6 = 53 << 8;
static const uint16_t P5_7 = 54 << 8;
static const uint16_t P1_0 = 17 << 8;
static const uint16_t P1_1 = 18 << 8;
static const uint16_t P1_2 = 19 << 8;
static const uint16_t P1_3 = 20 << 8;
static const uint16_t P1_4 = 21 << 8;
static const uint16_t P1_5 = 22 << 8;
static const uint16_t P1_6 = 23 << 8;
static const uint16_t P1_7 = 24 << 8;
static const uint16_t P6_0 = 97 << 8;
static const uint16_t P6_1 = 98 << 8;
static const uint16_t P6_2 = 99 << 8;
static const uint16_t P6_3 = 100 << 8;
static const uint16_t P6_4 = 1 << 8;
static const uint16_t P6_5 = 2 << 8;
static const uint16_t P6_6 = 3 << 8;
static const uint16_t P6_7 = 4 << 8;
static const uint16_t P2_0 = 25 << 8;
static const uint16_t P2_1 = 26 << 8;
static const uint16_t P2_2 = 27 << 8;
static const uint16_t P2_3 = 28 << 8;
static const uint16_t P2_4 = 29 << 8;
static const uint16_t P2_5 = 30 << 8;
static const uint16_t P2_6 = 31 << 8;
static const uint16_t P2_7 = 32 << 8;
static const uint16_t P7_0 = 13 << 8;
static const uint16_t P7_1 = 14 << 8;
static const uint16_t P7_2 = 55 << 8;
static const uint16_t P7_3 = 56 << 8;
static const uint16_t P7_4 = 5 << 8;
static const uint16_t P7_5 = 6 << 8;
static const uint16_t P7_6 = 7 << 8;
static const uint16_t P7_7 = 8 << 8;
static const uint16_t P3_0 = 33 << 8;
static const uint16_t P3_1 = 34 << 8;
static const uint16_t P3_2 = 35 << 8;
static const uint16_t P3_3 = 36 << 8;
static const uint16_t P3_4 = 39 << 8;
static const uint16_t P3_5 = 40 << 8;
static const uint16_t P3_6 = 41 << 8;
static const uint16_t P3_7 = 42 << 8;
static const uint16_t PJ_0 = 92 << 8;
static const uint16_t PJ_1 = 93 << 8;
static const uint16_t PJ_2 = 94 << 8;
static const uint16_t PJ_3 = 95 << 8;
static const uint16_t P10_0 = 76 << 8;
static const uint16_t P10_1 = 77 << 8;
static const uint16_t P8_0 = 57 << 8;
static const uint16_t P10_2 = 78 << 8;
static const uint16_t P8_1 = 58 << 8;
static const uint16_t P10_3 = 79 << 8;
static const uint16_t P8_2 = 59 << 8;
static const uint16_t P10_4 = 80 << 8;
static const uint16_t P8_3 = 60 << 8;
static const uint16_t P10_5 = 81 << 8;
static const uint16_t P8_4 = 61 << 8;
static const uint16_t P10_6 = 82 << 8;
static const uint16_t P8_5 = 65 << 8;
static const uint16_t P10_7 = 83 << 8;
static const uint16_t P8_6 = 66 << 8;
static const uint16_t P8_7 = 67 << 8;


/* USCI configuration */
#define USCI_A_COUNT 4
#define USCI_B_COUNT 4

#ifdef ARDUINO_MAIN
const usci_pin_definition_t uscia_pins[] = {
	// TXD << 8 | PORT_SELECTIONx, RXD << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx
	{39 << 8 | PORT_SELECTION0, 40 << 8 | PORT_SELECTION0, 36 << 8 | PORT_SELECTION0, 33 << 8 | PORT_SELECTION0},
	{53 << 8 | PORT_SELECTION0, 54 << 8 | PORT_SELECTION0, 52 << 8 | PORT_SELECTION0, 41 << 8 | PORT_SELECTION0},
	{72 << 8 | PORT_SELECTION0, 73 << 8 | PORT_SELECTION0, 71 << 8 | PORT_SELECTION0, 68 << 8 | PORT_SELECTION0},
	{80 << 8 | PORT_SELECTION0, 81 << 8 | PORT_SELECTION0, 79 << 8 | PORT_SELECTION0, 76 << 8 | PORT_SELECTION0}
};

const usci_pin_definition_t uscib_pins[] = {
	// MOSI/SDA << 8 | PORT_SELECTIONx, MISO/SCL << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx
	{34 << 8 | PORT_SELECTION0, 35 << 8 | PORT_SELECTION0, 33 << 8 | PORT_SELECTION0, 36 << 8 | PORT_SELECTION0},
	{42 << 8 | PORT_SELECTION0, 51 << 8 | PORT_SELECTION0, 41 << 8 | PORT_SELECTION0, 52 << 8 | PORT_SELECTION0},
	{69 << 8 | PORT_SELECTION0, 70 << 8 | PORT_SELECTION0, 68 << 8 | PORT_SELECTION0, 71 << 8 | PORT_SELECTION0},
	{77 << 8 | PORT_SELECTION0, 78 << 8 | PORT_SELECTION0, 76 << 8 | PORT_SELECTION0, 79 << 8 | PORT_SELECTION0}
};
#else
extern const usci_pin_definition_t uscia_pins[];
extern const usci_pin_definition_t uscib_pins[];
#endif /* ARDUINO_MAIN */



#endif /* CHIP_MSP430F5438APZ_H */

