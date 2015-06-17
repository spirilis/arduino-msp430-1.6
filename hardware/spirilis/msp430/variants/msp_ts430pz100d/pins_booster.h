/* MSP-TS430PZ100D Target Board with MSP430FR6989 installed
 *
 * This is a 100-pin LQFP chip with FRAM, no USB.
 */

#ifndef PINS_BOOSTER_H
#define PINS_BOOSTER_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>
#include "chip_msp430fr6989pz.h"

#ifdef ARDUINO_MAIN
// All I/O access to this should happen via physical pin references.
const uint8_t boosterpack_pins[] = {
	NOT_A_PIN,		// filler for index #0
};
#else
extern const uint8_t boosterpack_pins[];
#endif /* ARDUINO_MAIN */

// Common pins
static const uint16_t D1 = 66 << 8;  // P1.0
static const uint16_t D2 = 65 << 8;  // P1.1
static const uint16_t D3 = 64 << 8;  // P1.2
static const uint16_t SW2 = 63 << 8;  // P1.3 (SW2)
static const uint16_t PUSH2 = 63 << 8;

#endif /* PINS_BOOSTER_H */
