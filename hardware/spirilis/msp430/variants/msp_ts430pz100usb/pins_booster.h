/* MSP-TS430PZ100USB Target Board with MSP430F5659 installed
 *
 * This is a 100-pin LQFP chip with USB support.
 */

#ifndef PINS_BOOSTER_H
#define PINS_BOOSTER_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>
#include "chip_msp430f5659pz.h"

#ifdef ARDUINO_MAIN
// All I/O access to this should happen via physical pin references.
const uint8_t boosterpack_pins[] = {
	NOT_A_PIN,		// filler for index #0
};
#else
extern const uint8_t boosterpack_pins[];
#endif /* ARDUINO_MAIN */

// Common pins
static const uint16_t D1 = 34 << 8;
static const uint16_t LED1 = 58 << 8;
static const uint16_t LED2 = 59 << 8;
static const uint16_t LED3 = 60 << 8;
static const uint16_t TEMPSENSOR = 0x8000 | 10;

#endif /* PINS_BOOSTER_H */
