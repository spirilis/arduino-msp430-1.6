/* Chip-specific pin information - MSP430FR6989PZ (48-pin VQFN) */

#ifndef CHIP_MSP430FR6989PZ_H
#define CHIP_MSP430FR6989PZ_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>


/* Clock system - Clockable Peripherals */
#define CPUFREQ_CHIP_SPECIFIC_PERIPH_MASK	(PERIPH_TIMERA0 | PERIPH_TIMERA1 | PERIPH_TIMERB0 |		\
						 PERIPH_TIMERA2 | PERIPH_TIMERA3 |				\
						 PERIPH_USCI_A0 | PERIPH_USCI_B0 |				\
						 PERIPH_USCI_A1 | PERIPH_USCI_B1)

#define CPUFREQ_CHIP_SPECIFIC_PERIPH_COUNT      9

/* Clock system - SMCLK, ACLK output pins */
#define CLKOUT_SMCLK_PORT               P10IN
#define CLKOUT_SMCLK_PIN                BIT0

#define CLKOUT_ACLK_PORT                P5IN
#define CLKOUT_ACLK_PIN                 BIT2

/* Clock system - LFXT pins */
#define CS_LFXT_PORT                    PJIN
#define CS_LFXT_XIN                     BIT4
#define CS_LFXT_XOUT                    BIT5

/* Clock system - HFXT (XT2) pins */
#define CS_HFXT_PORT                    PJIN
#define CS_HFXT_XIN                     BIT6
#define CS_HFXT_XOUT                    BIT7

/* Clock system - VLO frequency */
#define CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY     9400UL
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
	{&P5IN, &P5OUT, &P5DIR, &P5REN, &P5SEL0, &P5SEL1, NULL, NULL, NULL},
	{&P6IN, &P6OUT, &P6DIR, &P6REN, &P6SEL0, &P6SEL1, NULL, NULL, NULL},
	{&P7IN, &P7OUT, &P7DIR, &P7REN, &P7SEL0, &P7SEL1, NULL, NULL, NULL},
	{&P8IN, &P8OUT, &P8DIR, &P8REN, &P8SEL0, &P8SEL1, NULL, NULL, NULL},
	{&P9IN, &P9OUT, &P9DIR, &P9REN, &P9SEL0, &P9SEL1, NULL, NULL, NULL},
	{&P10IN, &P10OUT, &P10DIR, &P10REN, &P10SEL0, &P10SEL1, NULL, NULL, NULL},
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
	{P4, BIT3, -1, NOT_A_TIMER},  // 1 - P4.3/UCA0SOMI/UCA0RXD/UCB1STE
	{P1, BIT4, -1, T1A0},  // 2 - P1.4/UCB0CLK/UCA0STE/TA1.0/S1
	{P1, BIT5, -1, T0A0},  // 3 - P1.5/UCB0STE/UCA0CLK/TA0.0/S0
	{P1, BIT6, -1, T0A1},  // 4 - P1.6/UCB0SIMO/UCB0SDA/TA0.1
	{P1, BIT7, -1, T0A2},  // 5 - P1.7/UCB0SOMI/UCB0SCL/TA0.2
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 6 - R33/LCDCAP
	{P6, BIT0, -1, NOT_A_TIMER},  // 7 - P6.0/R23
	{P6, BIT1, -1, NOT_A_TIMER},  // 8 - P6.1/R13/LCDREF
	{P6, BIT2, -1, NOT_A_TIMER},  // 9 - P6.2/COUT/R03
	{P6, BIT3, -1, NOT_A_TIMER},  // 10 - P6.3/COM0
	{P6, BIT4, -1, T0B0},  // 11 - P6.4/TB0.0/COM1
	{P6, BIT5, -1, T0B1},  // 12 - P6.5/TB0.1/COM2
	{P6, BIT6, -1, T0B2},  // 13 - P6.6/TB0.2/COM3
	{P2, BIT4, -1, T0B3},  // 14 - P2.4/TB0.3/COM4/S43
	{P2, BIT5, -1, T0B4},  // 15 - P2.5/TB0.4/COM5/S42
	{P2, BIT6, -1, T0B5},  // 16 - P2.6/TB0.5/ESIC1OUT/COM6/S41
	{P2, BIT7, -1, T0B6},  // 17 - P2.7/TB0.6/ESIC2OUT/COM7/S40
	{P10, BIT2, -1, T1A0},  // 18 - P10.2/TA1.0/SMCLK/S39
	{P5, BIT0, -1, T1A1},  // 19 - P5.0/TA1.1/MCLK/S38
	{P5, BIT1, -1, T1A2},  // 20 - P5.1/TA1.2/S37
	{P5, BIT2, -1, T1A0},  // 21 - P5.2/TA1.0/TA1CLK/ACLK/S36
	{P5, BIT3, -1, NOT_A_TIMER},  // 22 - P5.3/UCB1STE/S35
	{P3, BIT0, -1, NOT_A_TIMER},  // 23 - P3.0/UCB1CLK/S34
	{P3, BIT1, -1, NOT_A_TIMER},  // 24 - P3.1/UCB1SIMO/UCB1SDA/S33
	{P3, BIT2, -1, NOT_A_TIMER},  // 25 - P3.2/UCB1SOMI/UCB1SCL/S32
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 26 - DVss1
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 27 - DVcc1
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 28 - TEST/SBWTCK
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 29 - RESET/NMI/SBWTDIO
	{PJ, BIT0, -1, NOT_A_TIMER},  // 30 - PJ.0/TDO/TB0OUTH/SMCLK/SRSCG1
	{PJ, BIT1, -1, NOT_A_TIMER},  // 31 - PJ.1/TDI/TCLK/MCLK/SRSCG0
	{PJ, BIT2, -1, NOT_A_TIMER},  // 32 - PJ.2/TMS/ACLK/SROSCOFF
	{PJ, BIT3, -1, NOT_A_TIMER},  // 33 - PJ.3/TCK/COUT/SRCPUOFF
	{P6, BIT7, -1, NOT_A_TIMER},  // 34 - P6.7/TA0CLK/S31
	{P7, BIT5, -1, T0A2},  // 35 - P7.5/TA0.2/S30
	{P7, BIT6, -1, T0A1},  // 36 - P7.6/TA0.1/S29
	{P10, BIT1, -1, T0A0},  // 37 - P10.1/TA0.0/S28
	{P7, BIT7, -1, T1A2},  // 38 - P7.7/TA1.2/TB0OUTH/S27
	{P3, BIT3, -1, T1A1},  // 39 - P3.3/TA1.1/TB0CLK/S26
	{P3, BIT4, -1, T0B0},  // 40 - P3.4/UCA1SIMO/UCA1TXD/TB0.0/S25
	{P3, BIT5, -1, T0B1},  // 41 - P3.5/UCA1SOMI/UCA1RXD/TB0.1/S24
	{P3, BIT6, -1, T0B2},  // 42 - P3.6/UCA1CLK/TB0.2/S23
	{P3, BIT7, -1, T0B3},  // 43 - P3.7/UCA1STE/TB0.3/S22
	{P8, BIT0, -1, NOT_A_TIMER},  // 44 - P8.0/RTCCLK/S21
	{P8, BIT1, -1, NOT_A_TIMER},  // 45 - P8.1/DMAE0/S20
	{P8, BIT2, -1, NOT_A_TIMER},  // 46 - P8.2/S19
	{P8, BIT3, -1, NOT_A_TIMER},  // 47 - P8.3/MCLK/S18
	{P2, BIT3, -1, NOT_A_TIMER},  // 48 - P2.3/UCA0STE/TB0OUTH
	{P2, BIT2, -1, T0B4},  // 49 - P2.2/UCA0CLK/TB0.4/RTCCLK
	{P2, BIT1, -1, T0B5},  // 50 - P2.1/UCA0SOMI/UCA0RXD/TB0.5/DMAE0
	{P2, BIT0, -1, T0B6},  // 51 - P2.0/UCA0SIMO/UCA0TXD/TB0.6/TB0CLK
	{P7, BIT0, -1, NOT_A_TIMER},  // 52 - P7.0/TA0CLK/S17
	{P7, BIT1, -1, T0A0},  // 53 - P7.1/TA0.0/S16
	{P7, BIT2, -1, T0A1},  // 54 - P7.2/TA0.1/S15
	{P7, BIT3, -1, T0A2},  // 55 - P7.3/TA0.2/S14
	{P7, BIT4, -1, NOT_A_TIMER},  // 56 - P7.4/SMCLK/S13
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 57 - DVss2
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 58 - DVcc2
	{P8, BIT4, 7, NOT_A_TIMER},  // 59 - P8.4/A7/C7
	{P8, BIT5, 6, NOT_A_TIMER},  // 60 - P8.5/A6/C6
	{P8, BIT6, 5, NOT_A_TIMER},  // 61 - P8.6/A5/C5
	{P8, BIT7, 4, NOT_A_TIMER},  // 62 - P8.7/A4/C4
	{P1, BIT3, 3, T1A2},  // 63 - P1.3/TA1.2/ESITEST4/A3/C3
	{P1, BIT2, 2, T1A1},  // 64 - P1.2/TA1.1/TA0CLK/COUT/A2/C2
	{P1, BIT1, 1, T0A2},  // 65 - P1.1/TA0.2/TA1CLK/COUT/A1/C1/VREF+/VeREF+
	{P1, BIT0, 0, T0A1},  // 66 - P1.0/TA0.1/DMAE0/RTCCLK/A0/C0/VREF-/VeREF-
	{P9, BIT0, 8, NOT_A_TIMER},  // 67 - P9.0/ESICH0/ESITEST0/A8/C8
	{P9, BIT1, 9, NOT_A_TIMER},  // 68 - P9.1/ESICH1/ESITEST1/A9/C9
	{P9, BIT2, 10, NOT_A_TIMER},  // 69 - P9.2/ESICH2/ESITEST2/A10/C10
	{P9, BIT3, 11, NOT_A_TIMER},  // 70 - P9.3/ESICH3/ESITEST3/A11/C11
	{P9, BIT4, 12, NOT_A_TIMER},  // 71 - P9.4/ESICI0/A12/C12
	{P9, BIT5, 13, NOT_A_TIMER},  // 72 - P9.5/ESICI1/A13/C13
	{P9, BIT6, 14, NOT_A_TIMER},  // 73 - P9.6/ESICI2/A14/C14
	{P9, BIT7, 15, NOT_A_TIMER},  // 74 - P9.7/ESICI3/A15/C15
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 75 - ESIDVcc
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 76 - ESIDVss
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 77 - ESICI
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 78 - ESICOM
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 79 - AVcc1
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 80 - AVss3
	{PJ, BIT7, -1, NOT_A_TIMER},  // 81 - PJ.7/HFXOUT
	{PJ, BIT6, -1, NOT_A_TIMER},  // 82 - PJ.6/HFXIN
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 83 - AVss1
	{PJ, BIT4, -1, NOT_A_TIMER},  // 84 - PJ.4/LFXIN
	{PJ, BIT5, -1, NOT_A_TIMER},  // 85 - PJ.5/LFXOUT
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 86 - AVss2
	{P5, BIT4, -1, NOT_A_TIMER},  // 87 - P5.4/UCA1SIMO/UCA1TXD/S12
	{P5, BIT5, -1, NOT_A_TIMER},  // 88 - P5.5/UCA1SOMI/UCA1RXD/S11
	{P5, BIT6, -1, NOT_A_TIMER},  // 89 - P5.6/UCA1CLK/S10
	{P5, BIT7, -1, NOT_A_TIMER},  // 90 - P5.7/UCA1STE/TB0CLK/S9
	{P4, BIT4, -1, NOT_A_TIMER},  // 91 - P4.4/UCB1STE/TA1CLK/S8
	{P4, BIT5, -1, T1A0},  // 92 - P4.5/UCB1CLK/TA1.0/S7
	{P4, BIT6, -1, T1A1},  // 93 - P4.6/UCB1SIMO/UCB1SDA/TA1.1/S6
	{P4, BIT7, -1, T1A2},  // 94 - P4.7/UCB1SOMI/UCB1SCL/TA1.2/S5
	{P10, BIT0, -1, NOT_A_TIMER},  // 95 - P10.0/SMCLK/S4
	{P4, BIT0, -1, NOT_A_TIMER},  // 96 - P4.0/UCB1SIMO/UCB1SDA/MCLK/S3
	{P4, BIT1, -1, NOT_A_TIMER},  // 97 - P4.1/UCB1SOMI/UCB1SCL/ACLK/S2
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 98 - DVss3
	{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // 99 - DVcc3
	{P4, BIT2, -1, NOT_A_TIMER},  // 100 - P4.2/UCA0SIMO/UCA0TXD/UCB1CLK
};
#else
extern const gpio_pin_definition_t chip_pins[];
#endif /* ARDUINO_MAIN */

#define CHIP_PIN_COUNT 100

// ADC pins
static const uint16_t A4 = 62 << 8;
static const uint16_t A5 = 61 << 8;
static const uint16_t A6 = 60 << 8;
static const uint16_t A7 = 59 << 8;
static const uint16_t A8 = 67 << 8;
static const uint16_t A9 = 68 << 8;
static const uint16_t A10 = 69 << 8;
static const uint16_t A11 = 70 << 8;
static const uint16_t A12 = 71 << 8;
static const uint16_t A13 = 72 << 8;
static const uint16_t A0 = 66 << 8;
static const uint16_t A14 = 73 << 8;
static const uint16_t A1 = 65 << 8;
static const uint16_t A15 = 74 << 8;
static const uint16_t A2 = 64 << 8;
static const uint16_t A3 = 63 << 8;

// Px_y definitions
static const uint16_t P4_0 = 96 << 8;
static const uint16_t P4_1 = 97 << 8;
static const uint16_t P4_2 = 100 << 8;
static const uint16_t P4_3 = 1 << 8;
static const uint16_t P4_4 = 91 << 8;
static const uint16_t P4_5 = 92 << 8;
static const uint16_t P4_6 = 93 << 8;
static const uint16_t P4_7 = 94 << 8;
static const uint16_t P9_0 = 67 << 8;
static const uint16_t P9_1 = 68 << 8;
static const uint16_t P9_2 = 69 << 8;
static const uint16_t P9_3 = 70 << 8;
static const uint16_t P9_4 = 71 << 8;
static const uint16_t P9_5 = 72 << 8;
static const uint16_t P9_6 = 73 << 8;
static const uint16_t P9_7 = 74 << 8;
static const uint16_t P5_0 = 19 << 8;
static const uint16_t P5_1 = 20 << 8;
static const uint16_t P5_2 = 21 << 8;
static const uint16_t P5_3 = 22 << 8;
static const uint16_t P5_4 = 87 << 8;
static const uint16_t P5_5 = 88 << 8;
static const uint16_t P5_6 = 89 << 8;
static const uint16_t P5_7 = 90 << 8;
static const uint16_t P1_0 = 66 << 8;
static const uint16_t P1_1 = 65 << 8;
static const uint16_t P1_2 = 64 << 8;
static const uint16_t P1_3 = 63 << 8;
static const uint16_t P1_4 = 2 << 8;
static const uint16_t P1_5 = 3 << 8;
static const uint16_t P1_6 = 4 << 8;
static const uint16_t P1_7 = 5 << 8;
static const uint16_t P6_0 = 7 << 8;
static const uint16_t P6_1 = 8 << 8;
static const uint16_t P6_2 = 9 << 8;
static const uint16_t P6_3 = 10 << 8;
static const uint16_t P6_4 = 11 << 8;
static const uint16_t P6_5 = 12 << 8;
static const uint16_t P6_6 = 13 << 8;
static const uint16_t P6_7 = 34 << 8;
static const uint16_t P2_0 = 51 << 8;
static const uint16_t P2_1 = 50 << 8;
static const uint16_t P2_2 = 49 << 8;
static const uint16_t P2_3 = 48 << 8;
static const uint16_t P2_4 = 14 << 8;
static const uint16_t P2_5 = 15 << 8;
static const uint16_t P2_6 = 16 << 8;
static const uint16_t P2_7 = 17 << 8;
static const uint16_t P7_0 = 52 << 8;
static const uint16_t P7_1 = 53 << 8;
static const uint16_t P7_2 = 54 << 8;
static const uint16_t P7_3 = 55 << 8;
static const uint16_t P7_4 = 56 << 8;
static const uint16_t P7_5 = 35 << 8;
static const uint16_t P7_6 = 36 << 8;
static const uint16_t P7_7 = 38 << 8;
static const uint16_t P3_0 = 23 << 8;
static const uint16_t P3_1 = 24 << 8;
static const uint16_t P3_2 = 25 << 8;
static const uint16_t P3_3 = 39 << 8;
static const uint16_t P3_4 = 40 << 8;
static const uint16_t P3_5 = 41 << 8;
static const uint16_t P3_6 = 42 << 8;
static const uint16_t P3_7 = 43 << 8;
static const uint16_t PJ_0 = 30 << 8;
static const uint16_t PJ_1 = 31 << 8;
static const uint16_t PJ_2 = 32 << 8;
static const uint16_t P10_0 = 95 << 8;
static const uint16_t PJ_3 = 33 << 8;
static const uint16_t PJ_4 = 84 << 8;
static const uint16_t P8_0 = 44 << 8;
static const uint16_t P10_1 = 37 << 8;
static const uint16_t PJ_5 = 85 << 8;
static const uint16_t P8_1 = 45 << 8;
static const uint16_t P10_2 = 18 << 8;
static const uint16_t PJ_6 = 82 << 8;
static const uint16_t P8_2 = 46 << 8;
static const uint16_t PJ_7 = 81 << 8;
static const uint16_t P8_3 = 47 << 8;
static const uint16_t P8_4 = 59 << 8;
static const uint16_t P8_5 = 60 << 8;
static const uint16_t P8_6 = 61 << 8;
static const uint16_t P8_7 = 62 << 8;


/* USCI configuration */
#define USCI_A_COUNT 2
#define USCI_B_COUNT 2

#ifdef ARDUINO_MAIN
const usci_pin_definition_t uscia_pins[] = {
	// TXD << 8 | PORT_SELECTIONx, RXD << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx
	{51 << 8 | PORT_SELECTION0, 50 << 8 | PORT_SELECTION0, 48 << 8 | PORT_SELECTION0, 49 << 8 | PORT_SELECTION0},
	{40 << 8 | PORT_SELECTION0, 41 << 8 | PORT_SELECTION0, 43 << 8 | PORT_SELECTION0, 42 << 8 | PORT_SELECTION0}
};

const usci_pin_definition_t uscib_pins[] = {
	// MOSI/SDA << 8 | PORT_SELECTIONx, MISO/SCL << 8 | PORT_SELECTIONx, SPI STE << 8 | PORT_SELECTIONx, SPI CLK << 8 | PORT_SELECTIONx
	{4 << 8 | PORT_SELECTION0, 5 << 8 | PORT_SELECTION0, 3 << 8 | PORT_SELECTION0, 2 << 8 | PORT_SELECTION0},
	{24 << 8 | PORT_SELECTION0, 25 << 8 | PORT_SELECTION0, 22 << 8 | PORT_SELECTION1, 23 << 8 | PORT_SELECTION0}
};
#else
extern const usci_pin_definition_t uscia_pins[];
extern const usci_pin_definition_t uscib_pins[];
#endif /* ARDUINO_MAIN */



#endif /* CHIP_MSP430FR6989PZ_H */
