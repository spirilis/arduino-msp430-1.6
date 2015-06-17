/* MSP430FR6989 Wolverine LCD LaunchPad BoosterPack pin layout
 *
 * FR6989 LaunchPad is based on the MSP430FR6989 PZ (10-pin LQFP) chip
 */

#ifndef PINS_BOOSTER_H
#define PINS_BOOSTER_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>
#include "chip_msp430fr6989pz.h"

#ifdef ARDUINO_MAIN
const uint8_t boosterpack_pins[] = {
	NOT_A_PIN,		// filler for index #0
				// J1 header
				// ---------
	NOT_A_PIN,		// 1  - Vcc
	69,			// 2  - P9.2
	1,			// 3  - P4.3
	100,			// 4  - P4.2
	25,			// 5  - P3.2
	70,			// 6  - P9.3
	2,			// 7  - P1.4
	51,			// 8  - P2.0
	97,			// 9  - P4.1
	96,			// 10 - P4.0
				// ---------
				// J2 header
	94,			// 11 - P4.7
	14,			// 12 - P2.4
	15,			// 13 - P2.5
	5,			// 14 - P1.7
	4,			// 15 - P1.6
	NOT_A_PIN,		// 16 - RESET
	71,			// 17 - P9.4
	3,			// 18 - P1.5
	50,			// 19 - P2.1
	NOT_A_PIN,		// 20 - GND
				// ---------
				// J3 header
	NOT_A_PIN,		// 21 - 5V
	NOT_A_PIN,		// 22 - GND
	59,			// 23 - P8.4
	60,			// 24 - P8.5
	61,			// 25 - P8.6
	62,			// 26 - P8.7
	67,			// 27 - P9.0
	68,			// 28 - P9.1
	72,			// 29 - P9.5
	73,			// 30 - P9.6
				// ---------
				// J4 header
	48,			// 31 - P2.3
	24,			// 32 - P3.1
	23,			// 33 - P3.0
	63,			// 34 - P1.3
	49,			// 35 - P2.2
	43,			// 36 - P3.7
	42,			// 37 - P3.6
	39,			// 38 - P3.3
	16,			// 39 - P2.6
	17,			// 40 - P2.7
};
#else
extern const uint8_t boosterpack_pins[];
#endif /* ARDUINO_MAIN */

// Common pins
static const uint16_t PUSH1 = 65 << 8;  // P1.1
static const uint16_t SW1 = 65 << 8;
static const uint16_t PUSH2 = 64 << 8;  // P1.2
static const uint16_t SW2 = 64 << 8;
static const uint16_t LED1 = 66 << 8;   // P1.0
static const uint16_t RED_LED = 66 << 8;
static const uint16_t LED2 = 74 << 8;   // P9.7
static const uint16_t GREEN_LED = 74 << 8;
static const uint16_t CTSD = 24 << 8;   // P3.1
static const uint16_t RTSD = 23 << 8;   // P3.0
static const uint16_t TEMPSENSOR = 0x8000 | 30;

#endif /* PINS_BOOSTER_H */
