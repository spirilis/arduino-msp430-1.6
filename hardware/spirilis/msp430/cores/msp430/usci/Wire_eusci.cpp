/* Energia-on-Arduino 1.6+ I2C Implementation
 *
 * eUSCI implementation
 */

#include <Energia.h>
#ifdef __MSP430_HAS_EUSCI_B0__

#include <Wire.h>
#include <usci/layout.h>
#include <usci/isr.h>



// Second argument (do_txrx_only) only used for old-style USCI (F2xxx/G2xxx).  Ignored in F5xxx/F6xxx USCI and eUSCI.
boolean TwoWire::isr(uint16_t intvect, boolean do_txrx_only)
{
	// USCI F5xxx/6xxx (newer) implementation
	register volatile uint16_t *_ctlw = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxCTL1;
	volatile uint8_t *_uctxbuf = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxTXBUF;
	volatile uint8_t *_ucrxbuf = SFRPTR_BYTE uscib_registers[_uscib_offset].UCBxRXBUF;
	register volatile uint16_t *_ifg = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxIFG;

	if (intvect >= USCI_I2C_UCRXIFG3) {
		// TX/RX
		if (intvect == USCI_I2C_UCTXIFG0) {
			uc_tx_handler:
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
						*_ctlw |= UCTXSTP;
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
						*_ctlw |= UCTXNACK; // Generate NACK b/c we're done
					else
						*_uctxbuf = _autoanswer.buffer[_autoanswer.head++];
				} else {
				#else
				if (1) {
				#endif
					if (_txbuf.head == _txbuf.tail)
						*_ctlw |= UCTXNACK;
					else
						*_uctxbuf = _txbuf.buffer[_txbuf.head++];
				}
			}
			return false;
		}

		if (intvect == USCI_I2C_UCRXIFG0) {
			uc_rx_handler:
			// RX code
			if (_twi_state == TWI_MRX) {
				_rxbuf.buffer[_rxbuf.head++] = *_ucrxbuf;
				if ( (_rxbuf.tail - _rxbuf.head) == 1 ) {  // Ready to read last byte; inform USCI to send NACK+STOP after
					// Last byte - STOP or REPEATED START?
					if (!_twi_repeat_request) {
						*_ctlw |= UCTXSTP;  // Only one byte left, generate STOP.
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
					*_ctlw |= UCTXNACK;  // Otherwise NACK and ignore.
				}
			}
			return false;
		}
		if (intvect == USCI_I2C_UCTXIFG1 || intvect == USCI_I2C_UCTXIFG2 || intvect == USCI_I2C_UCTXIFG3)
			goto uc_tx_handler;
		if (intvect == USCI_I2C_UCRXIFG1 || intvect == USCI_I2C_UCRXIFG2 || intvect == USCI_I2C_UCRXIFG3)
			goto uc_rx_handler;
	} else {
		// State change
		if (intvect == USCI_I2C_UCALIFG) {
			*_ifg &= ~UCALIFG;
			_twi_state = TWI_IDLE;
			_twi_error = TWI_ERROR_BUS_BUSY;
			*_ctlw |= UCSWRST;  // Pull us off the bus
			return true;
		}
		// NACK received
		if (intvect == USCI_I2C_UCNACKIFG) {
			*_ifg &= ~UCNACKIFG;

			if (_twi_state == TWI_MTX || _twi_state == TWI_MRX) {
				// This could be a Data NACK or Address NACK.  Hard to tell.
				_twi_error = TWI_ERROR_NACK;
				*_ctlw |= UCSWRST;
			}
			_twi_state = TWI_IDLE;
			return true;
		}
		// START condition
		if (intvect == USCI_I2C_UCSTTIFG) {
			*_ifg &= ~UCSTTIFG;

			_slave_address_index = 0;  /* TODO: Read UCBxADDRX and compare to list of UCBxI2COAy addresses to set
						    * _slave_address_index to whichever address was called.
						    */
			if (*_ctlw & UCTR) {
				// Slave TX mode
				_twi_state = TWI_STX;
				#ifdef WIRE_ENABLE_AUTOANSWER
				if (_autoanswer_enabled) {
					_autoanswer.head = 0;
					// No callback; autoanswer implies we have the data.
					if (_autoanswer.tail == 0 || _autoanswer.buffer == NULL) {
						*_ctlw |= UCTXNACK;  // But if autoanswer has no data, NACK!
						_twi_state = TWI_IDLE;
					}
				} else {
				#else
				if (1) {
				#endif
					_txbuf.head = _txbuf.tail = 0;
					// On Slave Transmit callback
					if (_stx_callback[_slave_address_index])
						_stx_callback[_slave_address_index]();
					// No data to send?  NACK.
					if (_txbuf.tail == 0) {
						*_ctlw |= UCTXNACK;
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
		if (intvect == USCI_I2C_UCSTPIFG) {
			*_ifg &= ~UCSTPIFG;
			if (_twi_state == TWI_SRX) {
				// Completion of Slave RX - Run callback
				// _rxbuf.tail has total length, _rxbuf.head *should* be 0
				if (_srx_callback[_slave_address_index] && _rxbuf.head < _rxbuf.tail) {
					// Zero-terminate data if possible (Arduino does this)
					if (_rxbuf.tail < _rxbuf.size)
						_rxbuf.buffer[_rxbuf.tail] = '\0';
					_srx_callback[_slave_address_index](_rxbuf.tail);
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
	volatile uint16_t *ctlw = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxCTL1;
	volatile uint16_t *brw = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxBR0;

	*ctlw |= UCSWRST;
	*brw = (uint16_t) (smclk / _i2c_master_freq);
	*ctlw &= ~UCSWRST;
}

void TwoWire::begin()
{
	_allocate_buffers();

	_twi_state = TWI_IDLE;
	_twi_error = 0;
	_twi_repeat_request = 0;
	_stx_callback[0] = NULL;
	_srx_callback[0] = NULL;

	volatile uint16_t *ctlw = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxCTL1;
	*ctlw = UCSWRST;
	*ctlw |= UCSSEL_2 | UCMST | UCMODE_3 | UCSYNC;

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

	volatile uint16_t *ctlw = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxCTL1;
	volatile uint16_t *i2coa0 = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxI2COA;
	volatile uint16_t *ie = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxIE;

	*ctlw |= UCSWRST;
	*i2coa0 = ouraddr | UCOAEN;
	if (ouraddr & 0x180)
		*ctlw |= UCA10;
	*ctlw &= ~(UCMST | UCTR);
	*ctlw &= ~UCSWRST;
	// TODO: With multiple slave addrs per UCB, detect which UCTXIEx and UCRXIEx's need enabling.
	*ie = UCTXIE0 | UCRXIE0 | UCSTTIE | UCSTPIE | UCALIE | UCNACKIE;
}

void TwoWire::end()
{
	volatile uint16_t *ctlw = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxCTL1;
	*ctlw |= UCSWRST;

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

	volatile uint16_t *ctlw = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxCTL1;
	volatile uint16_t *ie = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxIE;

	*ctlw |= UCSWRST;
	*ctlw |= UCMST | UCTR;

	if (_slave_addr & 0x180)
		*ctlw |= UCSLA10;  // 10-bit Slave Address
	else
		*ctlw &= ~UCSLA10;

	_twi_repeat_request = 0;
	_twi_state = TWI_MTX;  // Master Transmitter
	_twi_error = 0;

	*ctlw &= ~UCSWRST;
	*ie = UCTXIE0 | UCRXIE0 | UCSTTIE | UCSTPIE | UCALIE | UCNACKIE;

	// Check for timeout waiting for client to respond
	unsigned long millis_start = millis();
	*ctlw |= UCTXSTT;  // Initiate I2C START as Transmitter

	while ( (*ctlw & UCTXSTT) && (millis() - millis_start) < 50 )  // Wait until START + Address is finished sending...
		;
	if ( (millis() - millis_start) > 49 ) {  // Address NACK or fault in slave
		_twi_state = TWI_IDLE;
		_twi_error = TWI_ERROR_SLAVE_FAULT;
		*ctlw |= UCSWRST;
		if (!(_slave_addr & 0x4000)) {
			*ctlw &= ~UCMST;
			*ctlw &= ~UCSWRST;
		}
		return false;
	}

	while (_twi_state != TWI_IDLE && _twi_error == 0) {
		LPM0;
	}

	while (*ctlw & UCTXSTP)  // Wait for STOP condition to complete before resetting bus...
		;
	*ctlw |= UCSWRST;
	if (!(_slave_addr & 0x4000)) {
		*ctlw &= ~UCMST;
		*ctlw &= ~UCSWRST;
	}

	if (_twi_error)
		return false;
	return true;
}

uint8_t TwoWire::requestFrom(int addr, int len)
{
	if (len < 1)
		return 0;  // Nothing to receive!

	volatile uint16_t *ctlw = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxCTL1;
	volatile uint16_t *i2csa = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxI2CSA;
	volatile uint16_t *ie = SFRPTR_WORD uscib_registers[_uscib_offset].UCBxIE;

	*ctlw |= UCSWRST;

	_slave_addr = addr;
	*i2csa = addr;
	if (_slave_addr & 0x180)
		*ctlw |= UCSLA10;  // 10-bit Slave Address
	else
		*ctlw &= ~UCSLA10;

	*ctlw |= UCMST;
	*ctlw &= ~UCTR;

	_twi_repeat_request = 0;
	_rxbuf.head = 0;
	_rxbuf.tail = len;
	_twi_state = TWI_MRX;  // Master Receiver
	_twi_error = 0;

	*ctlw &= ~UCSWRST;
	*ie |= UCTXIE0 | UCRXIE0 | UCSTTIE | UCSTPIE | UCALIE | UCNACKIE;

	// Check for timeout waiting for client to respond
	unsigned long millis_start = millis();
	*ctlw |= UCTXSTT;  // Initiate I2C START as Receiver

	while ( (*ctlw & UCTXSTT) && (millis() - millis_start) < 50 )  // Wait until START + Address is finished sending...
		;
	if ( (millis() - millis_start) > 49 ) {  // Address NACK or fault in slave
		_twi_state = TWI_IDLE;
		_twi_error = TWI_ERROR_SLAVE_FAULT;
		*ctlw |= UCSWRST;
		if (!(_slave_addr & 0x4000)) {  // Are we a slave?
			*ctlw &= ~UCMST;
			*ctlw &= ~UCSWRST;
		}
		return 0;
	}

	if (len == 1)
		*ctlw |= UCTXSTP;  // STOP needs to be sent right away if it's just 1 byte!

	while (_twi_state != TWI_IDLE && _twi_error == 0) {
		LPM0;
	}
	while (*ctlw & UCTXSTP)  // Wait for STOP condition to complete before resetting bus...
		;
	*ctlw |= UCSWRST;
	if (!(_slave_addr & 0x4000)) {  // Are we a slave?
		*ctlw &= ~UCMST;
		*ctlw &= ~UCSWRST;
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
