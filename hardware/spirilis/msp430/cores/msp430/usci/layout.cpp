/* Energia-on-Arduino 1.6+ HardwareSerial USCI/eUSCI implementation
 * USCI register layout
 */

#include <Energia.h>
#include <usci/layout.h>


void usci_layout_dummy() {;}  // Simple function to ensure this .cpp gets compiled & linked in

/* Array of available USCI_A, USCI_B registers */

#ifdef __MSP430_HAS_USCI__
const uscia_sfr_t uscia_registers[] = {
	{&UCA0CTL0, &UCA0CTL1, &UCA0MCTL, &UCA0STAT, &UCA0BR0, &UCA0BR1, &UCA0ABCTL, &UCA0RXBUF, &UCA0TXBUF, &IE2, UCA0TXIE, UCA0RXIE, &IFG2, UCA0TXIFG, UCA0RXIFG, PERIPH_USCI_A0},
	#if USCI_A_COUNT > 1
	// Chips with __MSP430_HAS_USCI__ and two USCI_A's will also set __MSP430_HAS_USCI_AB0__ and __MSP430_HAS_USCI_AB1__
	{&UCA1CTL0, &UCA1CTL1, &UCA1MCTL, &UCA1STAT, &UCA1BR0, &UCA1BR1, &UCA1ABCTL, &UCA1RXBUF, &UCA1TXBUF, &UC1IE, UCA1TXIE, UCA1RXIE, &UC1IFG, UCA1TXIFG, UCA1RXIFG, PERIPH_USCI_A1},
	#endif
};

const uscib_sfr_t uscib_registers[] = {
	{&UCB0CTL0, &UCB0CTL1, &UCB0STAT, &UCB0BR0, &UCB0BR1, &UCB0I2COA, &UCB0I2CSA, &UCB0I2CIE, &UCB0RXBUF, &UCB0TXBUF, &IE2, UCB0TXIE, UCB0RXIE, &IFG2, UCB0TXIFG, UCB0RXIFG, PERIPH_USCI_B0},
	#if USCI_B_COUNT > 1
	// Chips with __MSP430_HAS_USCI__ and two USCI_A's will also set __MSP430_HAS_USCI_AB0__ and __MSP430_HAS_USCI_AB1__
	{&UCB1CTL0, &UCB1CTL1, &UCB1STAT, &UCB1BR0, &UCB1BR1, &UCB1I2COA, &UCB1I2CSA, &UCB1I2CIE, &UCB1RXBUF, &UCB1TXBUF, &UC1IE, UCB1TXIE, UCB1RXIE, &UC1IFG, UCB1TXIFG, UCB1RXIFG, PERIPH_USCI_B1},
	#endif
};
#endif

/*
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

 */

#ifdef __MSP430_HAS_USCI_A0__
const uscia_sfr_t uscia_registers[] = {
	{&UCA0CTL0, &UCA0CTL1, &UCA0MCTL, &UCA0STAT, &UCA0BR0, &UCA0BR1, &UCA0ABCTL, &UCA0RXBUF, &UCA0TXBUF, &UCA0IE, UCTXIE, UCRXIE, &UCA0IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_A0},
	#if USCI_A_COUNT > 1
	{&UCA1CTL0, &UCA1CTL1, &UCA1MCTL, &UCA1STAT, &UCA1BR0, &UCA1BR1, &UCA1ABCTL, &UCA1RXBUF, &UCA1TXBUF, &UCA1IE, UCTXIE, UCRXIE, &UCA1IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_A1},
	#endif
	#if USCI_A_COUNT > 2
	{&UCA2CTL0, &UCA2CTL1, &UCA2MCTL, &UCA2STAT, &UCA2BR0, &UCA2BR1, &UCA2ABCTL, &UCA2RXBUF, &UCA2TXBUF, &UCA2IE, UCTXIE, UCRXIE, &UCA2IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_A2},
	#endif
	#if USCI_A_COUNT > 3
	{&UCA3CTL0, &UCA3CTL1, &UCA3MCTL, &UCA3STAT, &UCA3BR0, &UCA3BR1, &UCA3ABCTL, &UCA3RXBUF, &UCA3TXBUF, &UCA3IE, UCTXIE, UCRXIE, &UCA3IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_A3},
	#endif
};
#endif

#ifdef __MSP430_HAS_EUSCI_A0__
const uscia_sfr_t uscia_registers[] = {
	{&UCA0CTL0, &UCA0CTL1, &UCA0MCTLW, &UCA0STATW, &UCA0BRW, NULL, &UCA0ABCTL, &UCA0RXBUF, &UCA0TXBUF, &UCA0IE, UCTXIE, UCRXIE, &UCA0IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_A0},
	#if USCI_A_COUNT > 1
	{&UCA1CTL0, &UCA1CTL1, &UCA1MCTLW, &UCA1STATW, &UCA1BRW, NULL, &UCA1ABCTL, &UCA1RXBUF, &UCA1TXBUF, &UCA1IE, UCTXIE, UCRXIE, &UCA1IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_A1},
	#endif
	#if USCI_A_COUNT > 2
	{&UCA2CTL0, &UCA2CTL1, &UCA2MCTLW, &UCA2STATW, &UCA2BRW, NULL, &UCA2ABCTL, &UCA2RXBUF, &UCA2TXBUF, &UCA2IE, UCTXIE, UCRXIE, &UCA2IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_A2},
	#endif
	#if USCI_A_COUNT > 3
	{&UCA3CTL0, &UCA3CTL1, &UCA3MCTLW, &UCA3STATW, &UCA3BRW, NULL, &UCA3ABCTL, &UCA3RXBUF, &UCA3TXBUF, &UCA3IE, UCTXIE, UCRXIE, &UCA3IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_A3},
	#endif
};
#endif


/* Array of available USCI_B registers */

#ifdef __MSP430_HAS_USCI_B0__
const uscib_sfr_t uscib_registers[] = {
	{&UCB0CTL0, &UCB0CTL1, &UCB0STAT, &UCB0BR0, &UCB0BR1, &UCB0I2COA, &UCB0I2CSA, NULL, &UCB0RXBUF, &UCB0TXBUF, &UCB0IE, UCTXIE, UCRXIE, &UCB0IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_B0},
	#if USCI_B_COUNT > 1
	{&UCB1CTL0, &UCB1CTL1, &UCB1STAT, &UCB1BR0, &UCB1BR1, &UCB1I2COA, &UCB1I2CSA, NULL, &UCB1RXBUF, &UCB1TXBUF, &UCB1IE, UCTXIE, UCRXIE, &UCB1IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_B1},
	#endif
	#if USCI_B_COUNT > 2
	{&UCB2CTL0, &UCB2CTL1, &UCB2STAT, &UCB2BR0, &UCB2BR1, &UCB2I2COA, &UCB2I2CSA, NULL, &UCB2RXBUF, &UCB2TXBUF, &UCB2IE, UCTXIE, UCRXIE, &UCB2IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_B2},
	#endif
	#if USCI_B_COUNT > 3
	{&UCB3CTL0, &UCB3CTL1, &UCB3STAT, &UCB3BR0, &UCB3BR1, &UCB3I2COA, &UCB3I2CSA, NULL, &UCB3RXBUF, &UCB3TXBUF, &UCB3IE, UCTXIE, UCRXIE, &UCB3IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_B3},
	#endif
};
#endif

#ifdef __MSP430_HAS_EUSCI_B0__
const uscib_sfr_t uscib_registers[] = {
	{&UCB0CTL0, &UCB0CTL1, &UCB0STATW, &UCB0BRW, NULL, &UCB0I2COA0, &UCB0I2CSA, NULL, &UCB0RXBUF, &UCB0TXBUF, &UCB0IE, UCTXIE, UCRXIE, &UCB0IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_B0},
	#if USCI_B_COUNT > 1
	{&UCB1CTL0, &UCB1CTL1, &UCB1STATW, &UCB1BRW, NULL, &UCB1I2COA0, &UCB1I2CSA, NULL, &UCB1RXBUF, &UCB1TXBUF, &UCB1IE, UCTXIE, UCRXIE, &UCB1IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_B1},
	#endif
	#if USCI_B_COUNT > 2
	{&UCB2CTL0, &UCB2CTL1, &UCB2STATW, &UCB2BRW, NULL, &UCB2I2COA0, &UCB2I2CSA, NULL, &UCB2RXBUF, &UCB2TXBUF, &UCB2IE, UCTXIE, UCRXIE, &UCB2IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_B2},
	#endif
	#if USCI_B_COUNT > 3
	{&UCB3CTL0, &UCB3CTL1, &UCB3STATW, &UCB3BRW, NULL, &UCB3I2COA0, &UCB3I2CSA, NULL, &UCB3RXBUF, &UCB3TXBUF, &UCB3IE, UCTXIE, UCRXIE, &UCB3IFG, UCTXIFG, UCRXIFG, PERIPH_USCI_B3},
	#endif

	/* Note: eUSCI has a bunch more I2C registers than UCBxI2COA & UCBxI2CSA, but they are all even offsets off UCBxI2COA so
	 * the code will just add offsets to the address for UCBxI2COA to resolve those.  These offsets appear to be consistent
	 * across different MSP430's using the newer eUSCI peripheral.
	 *
	 * Also, the eUSCI peripheral supports hardware Byte Counting of I2C information via UCBxTBCNT - this is available just
	 * after the UCBxSTATW register, so we resolve it automatically in code.
	 */
};
#endif

