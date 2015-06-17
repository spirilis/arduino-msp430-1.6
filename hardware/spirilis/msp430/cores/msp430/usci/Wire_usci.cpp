/* Energia-on-Arduino 1.6+ I2C Implementation
 *
 * Old-style USCI - F2xxx/G2xxx
 */

#include <Energia.h>
#ifdef __MSP430_HAS_USCI__

#include <Wire.h>
#include <usci/layout.h>
#include <usci/isr.h>



// Second argument (do_txrx_only) only used for old-style USCI (F2xxx/G2xxx).  Ignored in F5xxx/F6xxx USCI and eUSCI.
boolean TwoWire::isr(uint16_t intvect, boolean do_txrx_only)
{
	// USCI F2xxx/G2xxx (older) implementation
	register volatile uint8_t *_ctl1 = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxCTL1;
	volatile uint8_t *_uctxbuf = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxTXBUF;
	volatile uint8_t *_ucrxbuf = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxRXBUF;
	register volatile uint8_t *_ifg = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxSTAT;

	if (do_txrx_only) {
		// TX/RX
		volatile uint8_t *_txrxifg = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxIFG;
		uint8_t txifg = uscib_registers[_uscib_offset].usci_ifg_tx;
		uint8_t rxifg = uscib_registers[_uscib_offset].usci_ifg_rx;

		if (*_txrxifg & txifg) {
			// TX code
			if (_twi_state == TWI_MTX) {
				// If there is data to send, send it, otherwise stop
				if (_txbuf.head < _txbuf.tail) {
					*_uctxbuf = _txbuf.buffer[_txbuf.head++];
				} else {  // Last byte just sent?
					// Stop, or perhaps initiate REPEATED START
					_twi_state = TWI_IDLE;
					if (!_twi_repeat_request) {
						// STOP
						*_ctl1 |= UCTXSTP;
					} else {
						#ifdef WIRE_ENABLE_REPEATED_START
						// REPEATED START
						_twi_state = TWI_REQUEST_DECISION;
						return true;
						#endif
					}
					return true;
				}
			} else {
				// Slave transmit mode (TWI_STX)
				#ifdef WIRE_ENABLE_AUTOANSWER
				if (_autoanswer_enabled) {
					if (_autoanswer.head == _autoanswer.tail)
						*_ctl1 |= UCTXNACK; // Generate NACK b/c we're done
					else
						*_uctxbuf = _autoanswer.buffer[_autoanswer.head++];
				} else {
				#else
				if (1) {
				#endif
					if (_txbuf.head == _txbuf.tail)
						*_ctl1 |= UCTXNACK;
					else
						*_uctxbuf = _txbuf.buffer[_txbuf.head++];
				}
			}
			return false;
		}

		if (*_txrxifg & rxifg) {
			// RX code
			if (_twi_state == TWI_MRX) {
				_rxbuf.buffer[_rxbuf.head++] = *_ucrxbuf;
				if ( (_rxbuf.tail - _rxbuf.head) == 1 ) {  // Ready to read last byte; inform USCI to send NACK+STOP after
					// Last byte - STOP or REPEATED START?
					if (!_twi_repeat_request) {
						*_ctl1 |= UCTXSTP;  // Only one byte left, generate STOP.
						return false;
					} else {
						#ifdef WIRE_ENABLE_REPEATED_START
						// REPEATED START
						_twi_state = TWI_REQUEST_DECISION;
						return true;
						#endif
					}
				}
				if (_rxbuf.head >= _rxbuf.tail) {
					_rxbuf.head = 0;  // Reset head to 0 in preparation for user code to read
					_twi_state = TWI_IDLE;
					return true;
				}
			} else {
				// Slave Receive mode (TWI_SRX)
				if (_rxbuf.tail < _rxbuf.size) {  // Still room?
					_rxbuf.buffer[_rxbuf.tail++] = *_ucrxbuf;  // rxbuf.tail is used to indicate the final size of data
				} else {
					*_ctl1 |= UCTXNACK;  // Otherwise NACK and ignore.
				}
			}
			return false;
		}
	} else {
		// State change
		if (*_ifg & UCALIFG) {
			*_ifg &= ~UCALIFG;
			_twi_state = TWI_IDLE;
			_twi_error = TWI_ERROR_BUS_BUSY;
			*_ctl1 |= UCSWRST;  // Pull us off the bus
			return true;
		}
		// NACK received
		if (*_ifg & UCNACKIFG) {
			*_ifg &= ~UCNACKIFG;

			if (_twi_state == TWI_MTX || _twi_state == TWI_MRX) {
				// This could be a Data NACK or Address NACK.  Hard to tell.
				_twi_error = TWI_ERROR_NACK;
				*_ctl1 |= UCSWRST;
			}
			_twi_state = TWI_IDLE;
			return true;
		}
		// START condition
		if (*_ifg & UCSTTIFG) {
			*_ifg &= ~UCSTTIFG;

			if (*_ctl1 & UCTR) {
				// Slave TX mode
				_twi_state = TWI_STX;
				#ifdef WIRE_ENABLE_AUTOANSWER
				if (_autoanswer_enabled) {
					_autoanswer.head = 0;
					// No callback; autoanswer implies we have the data.
					if (_autoanswer.tail == 0 || _autoanswer.buffer == NULL) {
						*_ctl1 |= UCTXNACK;  // But if autoanswer has no data, NACK!
						_twi_state = TWI_IDLE;
					}
				} else {
				#else
				if (1) {
				#endif
					_txbuf.head = _txbuf.tail = 0;
					// On Slave Transmit callback
					if (_stx_callback[0])
						_stx_callback[0]();
					// No data to send?  NACK.
					if (_txbuf.tail == 0) {
						*_ctl1 |= UCTXNACK;
						_twi_state = TWI_IDLE;
					}
				}
			} else {
				// Slave RX mode
				_twi_state = TWI_SRX;
				_twi_error = 0;
				_twi_repeat_request = 0;
				_rxbuf.head = _rxbuf.tail = 0;
			}
		}
		// STOP condition
		if (*_ifg & UCSTPIFG) {
			*_ifg &= ~UCSTPIFG;
			if (_twi_state == TWI_SRX) {
				// Completion of Slave RX - Run callback
				// _rxbuf.tail has total length, _rxbuf.head *should* be 0
				if (_srx_callback[0] && _rxbuf.head < _rxbuf.tail) {
					// Zero-terminate data if possible (Arduino does this)
					if (_rxbuf.tail < _rxbuf.size)
						_rxbuf.buffer[_rxbuf.tail] = '\0';
					_srx_callback[0](_rxbuf.tail);
				}
			}
			_twi_state = TWI_IDLE;
			return true;
		}
		return false;
	}
	return false;
}

void TwoWire::updateClock(unsigned long smclk)
{
	volatile uint8_t *ctl1 = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxCTL1;
	volatile uint8_t *br0 = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxBR0;
	volatile uint8_t *br1 = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxBR1;

	*ctl1 |= UCSWRST;
	uint16_t br = (uint16_t) (smclk / _i2c_master_freq);

	*br0 = (uint8_t) br;
	asm volatile("swpb %[b]" : [b] "=r" (br) : "r" (br));
	*br1 = (uint8_t) br;
	*ctl1 &= ~UCSWRST;
}

void TwoWire::begin()
{
	_allocate_buffers();

	_twi_state = TWI_IDLE;
	_twi_error = 0;
	_twi_repeat_request = 0;
	_stx_callback[0] = NULL;
	_srx_callback[0] = NULL;

	volatile uint8_t *ctl0 = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxCTL0;
	volatile uint8_t *ctl1 = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxCTL1;
	*ctl1 = UCSSEL_2 | UCSWRST;
	*ctl0 = UCMST | UCMODE_3 | UCSYNC;

	volatile uint16_t *i2coa0 = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxI2COA;
	*i2coa0 = 0x0000;
	_slave_addr = 0x4000;  // Indicates not a slave

	updateClock(F_CPU);

	// Peripheral active
	cpu.enablePeripheral(uscib_registers[_uscib_offset].periph_callback_mask, TwoWire_clock_update_functions[_uscib_offset]);

	// Set Pin Modes
	if (_do_manage_pins) {
		#ifdef __MSP430_HAS_PORT_MAPPING__
		if (uscib_pins[_uscib_offset].pmapid_txd_rxd) {  // Port Mapper used to configure this one
			pinMode(uscib_pins[_uscib_offset].pin_txd & 0xFF00, uscib_pins[_uscib_offset].pmapid_txd_rxd & 0xFF00);  // PMAPID in upper 8 bits
			pinMode(uscib_pins[_uscib_offset].pin_rxd & 0xFF00, uscib_pins[_uscib_offset].pmapid_txd_rxd << 8);  // PMAPID in upper 8 bits
		} else {
			pinMode(uscib_pins[_uscib_offset].pin_txd & 0xFF00, uscib_pins[_uscib_offset].pin_txd & 0xFF);  // Pin set to Input + PxSEL
			pinMode(uscib_pins[_uscib_offset].pin_rxd & 0xFF00, uscib_pins[_uscib_offset].pin_rxd & 0xFF);  // Pin set to Input + PxSEL
		}
		#else
		pinMode(uscib_pins[_uscib_offset].pin_txd & 0xFF00, uscib_pins[_uscib_offset].pin_txd & 0xFF);  // Pin set to Input + PxSEL
		pinMode(uscib_pins[_uscib_offset].pin_rxd & 0xFF00, uscib_pins[_uscib_offset].pin_rxd & 0xFF);  // Pin set to Input + PxSEL
		#endif
	} /* if(_do_manage_pins) */
}

void TwoWire::begin(int ouraddr)
{
	begin();

	volatile uint8_t *ctl0 = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxCTL0;
	volatile uint8_t *ctl1 = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxCTL1;
	volatile uint16_t *i2coa0 = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxI2COA;
	volatile uint8_t *txrxie = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxIE;
	uint8_t txie = uscib_registers[_uscib_offset].usci_ie_tx;
	uint8_t rxie = uscib_registers[_uscib_offset].usci_ie_rx;
	volatile uint8_t *ie = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxI2CIE;

	*ctl1 |= UCSWRST;
	*i2coa0 = ouraddr;
	if (ouraddr & 0x180)
		*ctl0 |= UCA10;
	*ctl0 &= ~UCMST;
	*ctl1 &= ~UCSWRST;
	*ie |= UCSTTIE | UCSTPIE | UCALIE | UCNACKIE;
	*txrxie |= txie | rxie;
}

void TwoWire::end()
{
	volatile uint8_t *ctl1 = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxCTL1;
	*ctl1 |= UCSWRST;

	cpu.disablePeripheral(uscib_registers[_uscib_offset].periph_callback_mask);

	// Clear Pin Modes
	if (_do_manage_pins) {
		pinMode(uscib_pins[_uscib_offset].pin_txd & 0xFF00, INPUT);  // Hi-Z
		pinMode(uscib_pins[_uscib_offset].pin_rxd & 0xFF00, INPUT);  // Hi-Z
	}
}

void TwoWire::beginTransmission(int i2caddr)
{
	volatile uint16_t *i2csa = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxI2CSA;

	_slave_addr = i2caddr;
	*i2csa = i2caddr;
	_txbuf.head = _txbuf.tail = 0;
	_twi_state = TWI_IDLE;
	_twi_error = 0;
	_twi_repeat_request = 0;
}

boolean TwoWire::endTransmission()
{
	if (_txbuf.head >= _txbuf.tail)
		return false;  // Nothing to send!

	volatile uint8_t *ctl0 = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxCTL0;
	volatile uint8_t *ctl1 = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxCTL1;
	volatile uint8_t *txrxie = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxIE;
	uint8_t txie = uscib_registers[_uscib_offset].usci_ie_tx;
	uint8_t rxie = uscib_registers[_uscib_offset].usci_ie_rx;
	volatile uint8_t *ie = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxI2CIE;

	*ctl1 |= UCSWRST;

	if (_slave_addr & 0x180)
		*ctl0 |= UCSLA10;  // 10-bit Slave Address
	else
		*ctl0 &= ~UCSLA10;

	*ctl0 |= UCMST;
	*ctl1 = UCSSEL_2 | UCTR | UCSWRST;
	*ctl1 &= ~UCSWRST;
	_twi_repeat_request = 0;
	_twi_state = TWI_MTX;  // Master Transmitter
	_twi_error = 0;

	*ie = UCSTTIE | UCSTPIE | UCALIE | UCNACKIE;
	*txrxie |= txie | rxie;

	// Check for timeout waiting for client to respond
	unsigned long millis_start = millis();
	*ctl1 |= UCTR;
	*ctl1 |= UCTXSTT;  // Initiate I2C START as Transmitter

	while ( (*ctl1 & UCTXSTT) && (millis() - millis_start) < 50 )  // Wait until START + Address is finished sending...
		;
	if ( (millis() - millis_start) > 49 ) {  // Address NACK or fault in slave
		_twi_state = TWI_IDLE;
		_twi_error = TWI_ERROR_SLAVE_FAULT;
		*ctl1 = UCSSEL_2 | UCSWRST;
		if (!(_slave_addr & 0x4000)) {
			*ctl0 &= ~UCMST;
			*ctl1 &= ~UCSWRST;
		}
		return false;
	}

	while (_twi_state != TWI_IDLE && _twi_error == 0) {
		LPM0;
	}

	while (*ctl1 & UCTXSTP)  // Wait for STOP condition to complete before resetting bus...
		;
	*ctl1 = UCSSEL_2 | UCSWRST;
	if (!(_slave_addr & 0x4000)) {
		*ctl0 &= ~UCMST;
		*ctl1 &= ~UCSWRST;
	}

	if (_twi_error)
		return false;
	return true;
}

uint8_t TwoWire::requestFrom(int addr, int len)
{
	if (len < 1)
		return 0;  // Nothing to receive!

	volatile uint8_t *ctl0 = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxCTL0;
	volatile uint8_t *ctl1 = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxCTL1;
	volatile uint16_t *i2csa = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxI2CSA;
	volatile uint8_t *txrxie = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxIE;
	uint8_t txie = uscib_registers[_uscib_offset].usci_ie_tx;
	uint8_t rxie = uscib_registers[_uscib_offset].usci_ie_rx;
	volatile uint8_t *ie = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxI2CIE;

	*ctl1 |= UCSWRST;

	_slave_addr = addr;
	*i2csa = addr;
	if (_slave_addr & 0x180)
		*ctl0 |= UCSLA10;  // 10-bit Slave Address
	else
		*ctl0 &= ~UCSLA10;

	*ctl0 |= UCMST;
	*ctl1 = UCSSEL_2 | UCSWRST;
	*ctl1 &= ~UCSWRST;
	_twi_repeat_request = 0;
	_rxbuf.head = 0;
	_rxbuf.tail = len;
	_twi_state = TWI_MRX;  // Master Receiver
	_twi_error = 0;

	*ie = UCSTTIE | UCSTPIE | UCALIE | UCNACKIE;
	*txrxie |= txie | rxie;

	// Check for timeout waiting for client to respond
	unsigned long millis_start = millis();
	*ctl1 &= ~UCTR;
	*ctl1 |= UCTXSTT;  // Initiate I2C START as Receiver

	while ( (*ctl1 & UCTXSTT) && (millis() - millis_start) < 50 )  // Wait until START + Address is finished sending...
		;
	if ( (millis() - millis_start) > 49 ) {  // Address NACK or fault in slave
		_twi_state = TWI_IDLE;
		_twi_error = TWI_ERROR_SLAVE_FAULT;
		*ctl1 = UCSSEL_2 | UCSWRST;
		if (!(_slave_addr & 0x4000)) {  // Are we a slave?
			*ctl0 &= ~UCMST;
			*ctl1 &= ~UCSWRST;
		}
		return 0;
	}

	if (len == 1)
		*ctl1 |= UCTXSTP;  // STOP needs to be sent right away if it's just 1 byte!

	while (_twi_state != TWI_IDLE && _twi_error == 0) {
		LPM0;
	}
	while (*ctl1 & UCTXSTP)  // Wait for STOP condition to complete before resetting bus...
		;
	*ctl1 = UCSSEL_2 | UCSWRST;
	if (!(_slave_addr & 0x4000)) {  // Are we a slave?
		*ctl0 &= ~UCMST;
		*ctl1 &= ~UCSWRST;
	}

	if (_twi_error)
		return 0;
	return _rxbuf.tail;
}

// Not really implemented in earnest yet
boolean TwoWire::endTransmission(uint8_t sendStop)
{
	return endTransmission();
}

#ifdef WIRE_ENABLE_REPEATED_START
// Not really implemented in earnest yet
boolean TwoWire::endTransmissionThenWrite()
{
}

// Not really implemented in earnest yet
boolean TwoWire::endTransmissionThenRead(unsigned int count)
{
}
#endif /* ifdef WIRE_ENABLE_REPEATED_START */


#endif /* ifdef __MSP430_HAS_USCI_B0__ */
