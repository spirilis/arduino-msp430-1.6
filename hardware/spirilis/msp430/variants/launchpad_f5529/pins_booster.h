/* MSP430F5529 LaunchPad BoosterPack pin layout
 *
 * F5529 LaunchPad is based on the MSP430F5529 PN (80-pin LQFP) chip
 */

#ifndef PINS_BOOSTER_H
#define PINS_BOOSTER_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>
#include "chip_msp430f5529pn.h"

#ifdef ARDUINO_MAIN
const uint8_t boosterpack_pins[] = {
	NOT_A_PIN,		// filler for index #0
				// J1 header
				// ---------
	NOT_A_PIN,		// 1  - Vcc
	2,			// 2  - P6.5
	41,			// 3  - P3.4
	40,			// 4  - P3.3
	27,			// 5  - P1.6
	3,			// 6  - P6.6
	39,			// 7  - P3.2
	36,			// 8  - P2.7
	47,			// 9  - P4.2
	46,			// 10 - P4.1
				// ---------
				// J2 header
	16,			// 11 - P8.1
	32,			// 12 - P2.3
	35,			// 13 - P2.6
	38,			// 14 - P3.1
	37,			// 15 - P3.0
	NOT_A_PIN,		// 16 - RESET
	57,			// 17 - P7.4
	31,			// 18 - P2.2
	29,			// 19 - P2.0
	NOT_A_PIN,		// 20 - GND
				// ---------
				// J3 header
	NOT_A_PIN,		// 21 - 5V
	NOT_A_PIN,		// 22 - GND
	77,			// 23 - P6.0
	78,			// 24 - P6.1
	79,			// 25 - P6.2
	80,			// 26 - P6.3
	1,			// 27 - P6.4
	5,			// 28 - P7.0
	43,			// 29 - P3.6
	42,			// 30 - P3.5
				// ---------
				// J4 header
	17,			// 31 - P8.2
	44,			// 32 - P3.7
	45,			// 33 - P4.0
	48,			// 34 - P4.3
	23,			// 35 - P1.2
	24,			// 36 - P1.3
	25,			// 37 - P1.4
	26,			// 38 - P1.5
	33,			// 39 - P2.4
	34,			// 40 - P2.5
};
#else
extern const uint8_t boosterpack_pins[];
#endif /* ARDUINO_MAIN */

// Common pins
static const uint16_t PUSH1 = 30 << 8;  // P2.1
static const uint16_t SW1 = 30 << 8;
static const uint16_t PUSH2 = 22 << 8;  // P1.1
static const uint16_t SW2 = 22 << 8;
static const uint16_t RED_LED = 21 << 8; // P1.0
static const uint16_t GREEN_LED = 54 << 8;  // P4.7
static const uint16_t TEMPSENSOR = 0x8000 | 10;

#endif /* PINS_BOOSTER_H */
