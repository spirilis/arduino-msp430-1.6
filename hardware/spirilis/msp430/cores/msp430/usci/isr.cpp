/* Energia-on-Arduino 1.6+ USCI/eUSCI implementation
 * USCI ISR functions
 */

#include <Energia.h>
#include <usci/layout.h>
#include <usci/isr.h>
#include <HardwareSerial.h>
#include <Wire.h>

/* ISRs for USCI peripheral
 *
 * UART, I2C supported master & slave
 * SPI slave supported here
 */

#ifdef __cplusplus
extern "C" {
#endif

void usci_isr_dummy() {;}
static boolean still_asleep;  // For callbacks, determine if we've run wakeup()


/* USCIABxTXRX (F2xxx/G2xxx style) */

#if defined(__MSP430_HAS_USCI__) || defined(__MSP430_HAS_USCI_AB0__)
__attribute__((interrupt(USCIAB0TX_VECTOR)))
void USCIAB0_TX_ISR(void)
{
	still_asleep = _sys_stay_asleep;

	/* USCI_A0 TX IRQ */
	if (IFG2 & UCA0TXIFG) {
		if (UCA0CTL0 & UCSYNC) {
			// SPI slave mode
			// TODO
		} else {
			// UART
			uscia_xmit_char(0, &UCA0TXBUF);
		}
	}

	if (IFG2 & UCB0TXIFG) {
		if ( (UCB0CTL0 & UCMODE_3) == UCMODE_3 ) {
			if (Wire.isr(0, true)) {
				__bic_SR_register_on_exit(LPM4_bits);
				return;
			}
		} else {
			// TODO: SPI Slave
		}
	}

	if (_sys_stay_asleep != still_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}

__attribute__((interrupt(USCIAB0RX_VECTOR)))
void USCIAB0_RX_ISR(void)
{
	still_asleep = _sys_stay_asleep;
	uint8_t c;

	/* USCI_A0 RX IRQ */
	if (IFG2 & UCA0RXIFG) {
		if (UCA0CTL0 & UCSYNC) {
			// SPI slave mode
			// TODO
		} else {
			// UART
			c = UCA0RXBUF;
			uscia_store_char(0, c);
		}
	}

	if (IFG2 & UCB0RXIFG) {
		if ( (UCB0CTL0 & UCMODE_3) == UCMODE_3 ) {
			// I2C - only state changes are processed in RX vector
			uint8_t statifg = 0;
			while ( (UCB0STAT & (UCNACKIFG | UCSTPIFG | UCSTTIFG | UCALIFG)) ) {
				if (Wire.isr(statifg, false)) {
					__bic_SR_register_on_exit(LPM4_bits);
					return;
				}
			}
		} else {
			// TODO: SPI Slave
		}
	}

	if (_sys_stay_asleep != still_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif /* USCIAB0TXRX */

#if defined(__MSP430_HAS_USCI_AB1__)
__attribute__((interrupt(USCIAB1TX_VECTOR)))
void USCIAB1_TX_ISR(void)
{
	still_asleep = _sys_stay_asleep;

	/* USCI_A1 TX IRQ */
	if (UC1IFG & UCA1TXIFG) {
		if (UCA1CTL0 & UCSYNC) {
			// SPI slave mode
			// TODO
		} else {
			// UART
			uscia_xmit_char(1, &UCA1TXBUF);
		}
	}

	if (UC1IFG & UCB1TXIFG) {
		if ( (UCB1CTL0 & UCMODE_3) == UCMODE_3 ) {
			if (Wire1.isr(0, true)) {
				__bic_SR_register_on_exit(LPM4_bits);
				return;
			}
		} else {
			// TODO: SPI Slave
		}
	}

	if (_sys_stay_asleep != still_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}

__attribute__((interrupt(USCIAB1RX_VECTOR)))
void USCIAB1_RX_ISR(void)
{
	still_asleep = _sys_stay_asleep;
	uint8_t c;

	/* USCI_A1 RX IRQ */
	if (UC1IFG & UCA1RXIFG) {
		if (UCA1CTL0 & UCSYNC) {
			// SPI slave mode
			// TODO
		} else {
			// UART
			c = UCA1RXBUF;
			uscia_store_char(1, c);
		}
	}

	if (UC1IFG & UCB1RXIFG) {
		if ( (UCB1CTL0 & UCMODE_3) == UCMODE_3 ) {
			// I2C - only state changes are processed in RX vector
			uint8_t statifg;
			while ( (statifg = (UCB1STAT & (UCNACKIFG | UCSTPIFG | UCSTTIFG | UCALIFG))) ) {
				if (Wire1.isr(statifg, false)) {
					__bic_SR_register_on_exit(LPM4_bits);
					return;
				}
			}
		} else {
			// TODO: SPI Slave
		}
	}

	if (_sys_stay_asleep != still_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif /* USCIAB1TXRX */


/* USCI_Ax/Bx and eUSCI_Ax/Bx */

#if defined(__MSP430_HAS_USCI_A0__) || defined(__MSP430_HAS_EUSCI_A0__)
__attribute__((interrupt(USCI_A0_VECTOR)))
void USCIA0_ISR(void)
{
	uint8_t c;

	if (UCA0CTL0 & UCSYNC) {
		// SPI slave mode
		still_asleep = _sys_stay_asleep;
		// TODO
		if (_sys_stay_asleep != still_asleep)
			__bic_SR_register_on_exit(LPM4_bits);
	} else {
		#ifdef CHIP_HAS_EUSCI
		// UART mode
		switch (UCA0IV) {
			case USCI_UART_UCRXIFG:
				c = UCA0RXBUF;
				uscia_store_char(0, c);
				break;
			case USCI_UART_UCTXIFG:
				uscia_xmit_char(0, &UCA0TXBUF);
				break;
		}
		#else
		// UART mode
		switch (UCA0IV) {
			case USCI_UCRXIFG:
				c = UCA0RXBUF;
				uscia_store_char(0, c);
				break;
			case USCI_UCTXIFG:
				uscia_xmit_char(0, &UCA0TXBUF);
				break;
		}
		#endif
	}
}
#endif /* USCI_A0 */

#if defined(__MSP430_HAS_USCI_A1__) || defined(__MSP430_HAS_EUSCI_A1__)
__attribute__((interrupt(USCI_A1_VECTOR)))
void USCIA1_ISR(void)
{
	uint8_t c;

	if (UCA1CTL0 & UCSYNC) {
		// SPI slave mode
		still_asleep = _sys_stay_asleep;
		// TODO
		if (_sys_stay_asleep != still_asleep)
			__bic_SR_register_on_exit(LPM4_bits);
	} else {
		#ifdef CHIP_HAS_EUSCI
		// UART mode
		switch (UCA1IV) {
			case USCI_UART_UCRXIFG:
				c = UCA1RXBUF;
				uscia_store_char(1, c);
				break;
			case USCI_UART_UCTXIFG:
				uscia_xmit_char(1, &UCA1TXBUF);
				break;
		}
		#else
		// UART mode
		switch (UCA1IV) {
			case USCI_UCRXIFG:
				c = UCA1RXBUF;
				uscia_store_char(1, c);
				break;
			case USCI_UCTXIFG:
				uscia_xmit_char(1, &UCA1TXBUF);
				break;
		}
		#endif
	}
}
#endif /* USCI_A1 */

#if defined(__MSP430_HAS_USCI_A2__) || defined(__MSP430_HAS_EUSCI_A2__)
__attribute__((interrupt(USCI_A2_VECTOR)))
void USCIA2_ISR(void)
{
	uint8_t c;

	if (UCA2CTL0 & UCSYNC) {
		// SPI slave mode
		still_asleep = _sys_stay_asleep;
		// TODO
		if (_sys_stay_asleep != still_asleep)
			__bic_SR_register_on_exit(LPM4_bits);
	} else {
		#ifdef CHIP_HAS_EUSCI
		// UART mode
		switch (UCA2IV) {
			case USCI_UART_UCRXIFG:
				c = UCA2RXBUF;
				uscia_store_char(2, c);
				break;
			case USCI_UART_UCTXIFG:
				uscia_xmit_char(2, &UCA2TXBUF);
				break;
		}
		#else
		// UART mode
		switch (UCA2IV) {
			case USCI_UCRXIFG:
				c = UCA2RXBUF;
				uscia_store_char(2, c);
				break;
			case USCI_UCTXIFG:
				uscia_xmit_char(2, &UCA2TXBUF);
				break;
		}
		#endif
	}
}
#endif /* USCI_A2 */

#if defined(__MSP430_HAS_USCI_A3__) || defined(__MSP430_HAS_EUSCI_A3__)
__attribute__((interrupt(USCI_A3_VECTOR)))
void USCIA3_ISR(void)
{
	uint8_t c;

	if (UCA3CTL0 & UCSYNC) {
		// SPI slave mode
		still_asleep = _sys_stay_asleep;
		// TODO
		if (_sys_stay_asleep != still_asleep)
			__bic_SR_register_on_exit(LPM4_bits);
	} else {
		#ifdef CHIP_HAS_EUSCI
		// UART mode
		switch (UCA3IV) {
			case USCI_UART_UCRXIFG:
				c = UCA3RXBUF;
				uscia_store_char(3, c);
				break;
			case USCI_UART_UCTXIFG:
				uscia_xmit_char(3, &UCA0TXBUF);
				break;
		}
		#else
		// UART mode
		switch (UCA3IV) {
			case USCI_UCRXIFG:
				c = UCA3RXBUF;
				uscia_store_char(3, c);
				break;
			case USCI_UCTXIFG:
				uscia_xmit_char(3, &UCA0TXBUF);
				break;
		}
		#endif
	}
}
#endif /* USCI_A3 */

#if defined(__MSP430_HAS_USCI_B0__) || defined(__MSP430_HAS_EUSCI_B0__)
__attribute__((interrupt(USCI_B0_VECTOR)))
void USCIB0_ISR(void)
{
	still_asleep = _sys_stay_asleep;

	#ifdef CHIP_HAS_EUSCI
	if ( (UCB0CTLW0 & UCMODE_3) == UCMODE_3 ) {
	#else
	if ( (UCB0CTL0 & UCMODE_3) == UCMODE_3 ) {
	#endif
		// I2C
		uint16_t iv;
		while ( (iv = UCB0IV) != USCI_NONE ) {
			if (Wire.isr(iv, false)) {
				__bic_SR_register_on_exit(LPM4_bits);
				return;
			}
		}
	} else {
		// TODO: SPI Slave
	}

	if (still_asleep != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif /* USCI_B0 */

#if defined(__MSP430_HAS_USCI_B1__) || defined(__MSP430_HAS_EUSCI_B1__)
__attribute__((interrupt(USCI_B1_VECTOR)))
void USCIB1_ISR(void)
{
	still_asleep = _sys_stay_asleep;

	#ifdef CHIP_HAS_EUSCI
	if ( (UCB1CTLW0 & UCMODE_3) == UCMODE_3 ) {
	#else
	if ( (UCB1CTL0 & UCMODE_3) == UCMODE_3 ) {
	#endif
		// I2C
		uint16_t iv;
		while ( (iv = UCB1IV) != USCI_NONE ) {
			if (Wire1.isr(iv, false)) {
				__bic_SR_register_on_exit(LPM4_bits);
				return;
			}
		}
	} else {
		// TODO: SPI Slave
	}

	if (still_asleep != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif /* USCI_B1 */

#if defined(__MSP430_HAS_USCI_B2__) || defined(__MSP430_HAS_EUSCI_B2__)
__attribute__((interrupt(USCI_B2_VECTOR)))
void USCIB2_ISR(void)
{
	still_asleep = _sys_stay_asleep;

	#ifdef CHIP_HAS_EUSCI
	if ( (UCB2CTLW0 & UCMODE_3) == UCMODE_3 ) {
	#else
	if ( (UCB2CTL0 & UCMODE_3) == UCMODE_3 ) {
	#endif
		// I2C
		uint16_t iv;
		while ( (iv = UCB2IV) != USCI_NONE ) {
			if (Wire2.isr(iv, false)) {
				__bic_SR_register_on_exit(LPM4_bits);
				return;
			}
		}
	} else {
		// TODO: SPI Slave
	}

	if (still_asleep != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif /* USCI_B2 */

#if defined(__MSP430_HAS_USCI_B3__) || defined(__MSP430_HAS_EUSCI_B3__)
__attribute__((interrupt(USCI_B3_VECTOR)))
void USCIB3_ISR(void)
{
	still_asleep = _sys_stay_asleep;

	#ifdef CHIP_HAS_EUSCI
	if ( (UCB3CTLW0 & UCMODE_3) == UCMODE_3 ) {
	#else
	if ( (UCB3CTL0 & UCMODE_3) == UCMODE_3 ) {
	#endif
		// I2C
		uint16_t iv;
		while ( (iv = UCB3IV) != USCI_NONE ) {
			if (Wire3.isr(iv, false)) {
				__bic_SR_register_on_exit(LPM4_bits);
				return;
			}
		}
	} else {
		// TODO: SPI Slave
	}

	if (still_asleep != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif /* USCI_B3 */

#ifdef __cplusplus
}; /* extern "C" */
#endif
