/* MSP430FR5969 Wolverine LaunchPad BoosterPack pin layout
 *
 * FR5969 LaunchPad is based on the MSP430FR5969 RGZ (48-pin VQFN) chip
 */

#ifndef PINS_BOOSTER_H
#define PINS_BOOSTER_H

#include <msp430.h>
#include <stdint.h>
#include <variant_schema.h>
#include "chip_msp430fr5969rgz.h"

#ifdef ARDUINO_MAIN
const uint8_t boosterpack_pins[] = {
	NOT_A_PIN,		// filler for index #0
				// J1 header
				// ---------
	NOT_A_PIN,		// 1  - Vcc
	18,			// 2  - P4.2
	21,			// 3  - P2.6
	20,			// 4  - P2.5
	19,			// 5  - P4.3
	40,			// 6  - P2.4
	26,			// 7  - P2.2
	27,			// 8  - P3.4
	28,			// 9  - P3.5
	29,			// 10 - P3.6
				// ---------
				// J2 header
	9,			// 11 - P1.3
	10,			// 12 - P1.4
	11,			// 13 - P1.5
	32,			// 14 - P1.7
	31,			// 15 - P1.6
	NOT_A_PIN,		// 16 - RESET
	NOT_A_PIN,		// 17 - NC
	4,			// 18 - P3.0
	3,			// 19 - P1.2
	NOT_A_PIN,		// 20 - GND
				// ---------
};
#else
extern const uint8_t boosterpack_pins[];
#endif /* ARDUINO_MAIN */

// Common pins
static const uint16_t PUSH1 = 34 << 8;		// P4.5
static const uint16_t SW1 = 34 << 8;
static const uint16_t PUSH2 = 2 << 8;		// P1.1
static const uint16_t SW2 = 2 << 8;
static const uint16_t RED_LED = 35 << 8;	// P4.6
static const uint16_t LED1 = 35 << 8;
static const uint16_t GREEN_LED = 1 << 8;	// P1.0
static const uint16_t LED2 = 1 << 8;
static const uint16_t CTSD = 16 << 8;		// P4.0 (CTS from PC Backchannel UART)
static const uint16_t RTSD = 17 << 8;		// P4.1 (RTS to PC Backchannel UART)
static const uint16_t TEMPSENSOR = 0x8000 | 30;

#endif /* PINS_BOOSTER_H */
