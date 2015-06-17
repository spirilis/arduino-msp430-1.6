/* MSP430G2 LaunchPad layout w/ G2553
 *
 * G2553 LaunchPad is based on the MSP430G2553 DA (20-pin PDIP) chip.
 */

#ifndef PINS_BOOSTER_H
#define PINS_BOOSTER_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>
#include "chip_msp430g2553pw20.h"

#ifdef ARDUINO_MAIN
const uint8_t boosterpack_pins[] = {
	NOT_A_PIN,		// filler for index #0
				// J1 header
				// ---------
	NOT_A_PIN,		// 1  - Vcc
	2,			// 2  - P1.0
	3,			// 3  - P1.1
	4,			// 4  - P1.2
	5,			// 5  - P1.3
	6,			// 6  - P1.4
	7,			// 7  - P1.5
	8,			// 8  - P2.0
	9,			// 9  - P2.1
	10,			// 10 - P2.2
				// ---------
				// J2 header
	11,			// 11 - P2.3
	12,			// 12 - P2.4
	13,			// 13 - P2.5
	14,			// 14 - P1.6
	15,			// 15 - P1.7
	NOT_A_PIN,		// 16 - RESET
	NOT_A_PIN,		// 17 - TEST
	18,			// 18 - P2.7
	19,			// 19 - P2.6
	NOT_A_PIN,		// 20 - GND
};
#else
extern const uint8_t boosterpack_pins[];
#endif /* ARDUINO_MAIN */

// Common pins
static const uint16_t PUSH1 = 5 << 8;  // P1.3
static const uint16_t SW1 = 5 << 8;
static const uint16_t RED_LED = 2 << 8; // P1.0
static const uint16_t GREEN_LED = 14 << 8;  // P1.6
static const uint16_t TEMPSENSOR = 0x8000 | 10;

#endif /* PINS_BOOSTER_H */
