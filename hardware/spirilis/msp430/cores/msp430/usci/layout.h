/* Energia-on-Arduino 1.6+ HardwareSerial USCI/eUSCI implementation
 * USCI register layout
 */

#ifndef USCI_LAYOUT_H
#define USCI_LAYOUT_H

#include <Energia.h>

typedef struct {
	volatile void *UCAxCTL0;
	volatile void *UCAxCTL1;
	volatile void *UCAxMCTL;
	volatile void *UCAxSTAT;
	volatile void *UCAxBR0;
	volatile void *UCAxBR1;
	volatile void *UCAxABCTL;
	const volatile void *UCAxRXBUF;
	volatile void *UCAxTXBUF;
	volatile void *UCAxIE;
	uint8_t usci_ie_tx;
	uint8_t usci_ie_rx;
	volatile void *UCAxIFG;
	uint8_t usci_ifg_tx;
	uint8_t usci_ifg_rx;
	uint32_t periph_callback_mask;
} uscia_sfr_t;

typedef struct {
	volatile void *UCBxCTL0;
	volatile void *UCBxCTL1;
	volatile void *UCBxSTAT;
	volatile void *UCBxBR0;
	volatile void *UCBxBR1;
	volatile void *UCBxI2COA;
	volatile void *UCBxI2CSA;
	volatile void *UCBxI2CIE;
	const volatile void *UCBxRXBUF;
	volatile void *UCBxTXBUF;
	volatile void *UCBxIE;
	uint8_t usci_ie_tx;
	uint8_t usci_ie_rx;
	volatile void *UCBxIFG;
	uint8_t usci_ifg_tx;
	uint8_t usci_ifg_rx;
	uint32_t periph_callback_mask;
} uscib_sfr_t;


/* Identify flavor/variant of USCI */
#ifdef __MSP430_HAS_USCI__
#define CHIP_HAS_USCI
#endif

#ifdef __MSP430_HAS_USCI_A0__
#define CHIP_HAS_USCI
#endif

#ifdef __MSP430_HAS_EUSCI_A0__
#define CHIP_HAS_EUSCI
#endif

extern const uscia_sfr_t uscia_registers[];
extern const uscib_sfr_t uscib_registers[];


void usci_layout_dummy();  // Useless function used to ensure the compiler doesn't ignore layout.cpp

#endif /* USCI_LAYOUT_H */
