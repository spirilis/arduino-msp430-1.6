/* variant_schema.h - Structs and other data structures relevant to per-chip variant headers */

#ifndef VARIANT_SCHEMA_H
#define VARIANT_SCHEMA_H

#include <periph_mask.h>

#define ARRAY_IN_FLASH __attribute__((section(".text")))

/* Enum of all Timer + CCR types */
enum timer_database_t {
	NOT_A_TIMER = 0,
	T0A0, T0A1, T0A2, T0A3, T0A4, T0A5, T0A6, T0A7,
	T1A0, T1A1, T1A2, T1A3, T1A4, T1A5, T1A6, T1A7,
	T2A0, T2A1, T2A2, T2A3, T2A4, T2A5, T2A6, T2A7,
	T3A0, T3A1, T3A2, T3A3, T3A4, T3A5, T3A6, T3A7,
	T0B0, T0B1, T0B2, T0B3, T0B4, T0B5, T0B6, T0B7,
	T1B0, T1B1, T1B2, T1B3, T1B4, T1B5, T1B6, T1B7,
	T2B0, T2B1, T2B2, T2B3, T2B4, T2B5, T2B6, T2B7,
	T0D0, T0D1, T0D2, T0D3, T0D4, T0D5, T0D6, T0D7,
	T1D0, T1D1, T1D2, T1D3, T1D4, T1D5, T1D6, T1D7,
};

enum gpio_port_t {
	NOT_A_PORT = 0,
	P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, PJ
};

// boosterpack_pins[] definition indicating that this isn't a real pin
#define NOT_A_PIN 0xFF

// Whether PORT_SELECTION1 (PxSEL2 / PxSEL1 depending on chip family) exists.
#define PORT_SELECTION0 0x10
#define PORT_SELECTION1 0x20

#if defined(P1SEL2) || defined(P1SEL2_) || defined(P1SEL1) || defined(P1SEL1_)
#define CHIP_HAS_PORT_SELECTION1
#endif



/* Pins in this variant of Arduino/Energia can have 3 different flavors:
 *
 * Pin value < 255  - Value is an index into boosterpack_pins[], which is then used to look up the actual
 *                    port information in the chip_pins[] array.
 *
 * Pin value >= 256 - MSB (upper 8 bits) is an index into the chip_pins[] array; LSB is ignored.  Index value is
 *                    right-shifted 8 bits before using.
 *
 * Pin value < 255 but with BITF (0x8000) set - Pin value refers to internal ADC channel.  Only applies to
 *                    analogRead().  Using with pinMode or digitalWrite/digitalRead is treated like NOT_A_PIN.
 *
 * Pin value == 0xFF (255) means that it's not a valid pin.
 *
 * This allows scripts to easily auto-generate Px_y constants as well as ADC channel constants based on the
 * chip's pinout (producing them during scripted generation of the chip_pins[] array), and removes the burden
 * on the creator of an individual board or launchpad from having to generate all the Px_y and ADC constants.
 *
 * A common "chip_<chipname>.h" can be copied & used for different variants.
 */

/* Pin definition - includes port, ADC channel, Timer and Port Mapper information */
typedef struct {
	enum gpio_port_t port;
	const uint8_t portbit;
	const int8_t adc_channel;
	const enum timer_database_t timer;
#ifdef __MSP430_HAS_PORT_MAPPING__
	const uint8_t pmapid;
#endif
} gpio_pin_definition_t;


/* Port definition - tells us about its Interrupt capability, PxSEL2
 *
 * Defined with:
 * #ifdef ARDUINO_MAIN
 * const gpio_port_definition_t chip_ports[] = { .... }
 * #else
 * extern const gpio_port_definition_t chip_ports[];
 * #endif
 */
typedef struct {
	const volatile void *portin;
	volatile void *portout;
	volatile void *portdir;
	volatile void *portren;
	volatile void *portsel;
#if defined(P1SEL2) || defined(P1SEL2_)
	volatile void *portsel2;
#endif
#if defined(P1SEL1) || defined(P1SEL1_)
	volatile void *portsel2;
#endif
#ifdef __MSP430_HAS_PORT_MAPPING__
	volatile void *portmap;
#endif
	volatile void *porties;
	volatile void *portie;
	volatile void *portifg;
} gpio_port_definition_t;

/* Each variant needs to have a few #define's for the GPIO port register offsets:
 * GPIO_SFR_OFFSET_PxDIR
 * GPIO_SFR_OFFSET_PxOUT
 * GPIO_SFR_OFFSET_PxIN
 * GPIO_SFR_OFFSET_PxREN
 * GPIO_SFR_OFFSET_PxIE
 * GPIO_SFR_OFFSET_PxIES
 * GPIO_SFR_OFFSET_PxSEL
 *
 * These are referenced off the "portbase", which usually refers to PxIN already.
 */



/* USCI/eUSCI is expressed in the per-chip (same file housing chip_pins[]) header
 * with the following required definitions:
 *
 * #define USCI_A_COUNT <N>
 * #define USCI_B_COUNT <N>
 *
 * #ifdef ARDUINO_MAIN
 * const usci_pin_definition_t uscia_pins[] = { .... };
 * const usci_pin_definition_t uscib_pins[] = { .... };
 * #else
 * extern const usci_pin_definition_t uscia_pins[];
 * extern const usci_pin_definition_t uscib_pins[];
 * #endif
 *
 * The pin #'s are resolved directly into chip_pins[], no reference to boosterpack_pins[]
 *   allowed for these.  These should never have to change across different board implementations
 *   of the same chip.
 */

/* USCI/eUSCI port config definitions */
typedef struct {
	const uint16_t pin_txd;	// TXD (SPI SIMO / I2C SDA (USCI_B only)) << 8 | PORT_SELECTION[0 or 1 or both OR'd together]
	const uint16_t pin_rxd;	// RXD (SPI SOMI / I2C SCL (USCI_B only)) << 8 | PORT_SELECTION[0 or 1 or both OR'd together]
	const uint16_t pin_ste; // STE pin << 8 | PORT_SELECTION[0 or 1 or both OR'd together]
	const uint16_t pin_clk; // CLK pin << 8 | PORT_SELECTION[0 or 1 or both OR'd together]
#ifdef __MSP430_HAS_PORT_MAPPING__
	const uint16_t pmapid_txd_rxd;  // MSB = PM_UCAxTXD (SIMO/SDA), LSB = PM_UCAxRXD (SOMI/SCL)
	const uint16_t pmapid_ste_clk;  // MSB = PM_UCAxSTE, LSB = PM_UCAxCLK
#endif
} usci_pin_definition_t;



#endif /* VARIANT_SCHEMA_H */
