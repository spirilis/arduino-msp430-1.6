/* Energia-on-Arduino 1.6+ I2C Implementation
 *
 * Common methods/functions not related to specific USCI variants.
 */

#include <Energia.h>
#include <Wire.h>
#include <usci/layout.h>
#include <usci/isr.h>


TwoWire Wire(0);
#if USCI_B_COUNT > 1
TwoWire Wire1(1);
#endif
#if USCI_B_COUNT > 2
TwoWire Wire2(2);
#endif
#if USCI_B_COUNT > 3
TwoWire Wire3(3);
#endif

// CPU Clock Request callbacks
extern "C" {
void TwoWire_clock_update_B0(unsigned long freq) { Wire.updateClock(freq); }
#if USCI_B_COUNT > 1
void TwoWire_clock_update_B1(unsigned long freq) { Wire1.updateClock(freq); }
#endif
#if USCI_B_COUNT > 2
void TwoWire_clock_update_B2(unsigned long freq) { Wire2.updateClock(freq); }
#endif
#if USCI_B_COUNT > 3
void TwoWire_clock_update_B3(unsigned long freq) { Wire3.updateClock(freq); }
#endif


const CPUFREQ_PERIPHERAL_CALLBACK TwoWire_clock_update_functions[USCI_B_COUNT] = {
	TwoWire_clock_update_B0,
#if USCI_B_COUNT > 1
	TwoWire_clock_update_B1,
#endif
#if USCI_B_COUNT > 2
	TwoWire_clock_update_B2,
#endif
#if USCI_B_COUNT > 3
	TwoWire_clock_update_B3,
#endif
}; /* const TwoWire_clock_update_functions[] */

}; /* extern "C" */




TwoWire::TwoWire(uint8_t uscib_offset)
{
	_txbuf.size = DEFAULT_TWOWIRE_BUFFER_LENGTH;
	_txbuf.buffer = NULL;
	_txbuf.head = _txbuf.tail = 0;
	_rxbuf.size = DEFAULT_TWOWIRE_BUFFER_LENGTH;
	_rxbuf.buffer = NULL;
	_rxbuf.head = _rxbuf.tail = 0;
	_uscib_offset = uscib_offset;
	_do_manage_pins = true;
	_slave_addr = 0x4000;

	_i2c_master_freq = DEFAULT_TWOWIRE_FREQUENCY;
	_twi_state = 0;
	_twi_repeat_request = 0;
	_stx_callback[0] = NULL;
	_srx_callback[0] = NULL;

	#ifdef WIRE_ENABLE_AUTOANSWER
	_autoanswer.size = 0;
	_autoanswer.buffer = NULL;
	_autoanswer.head = _autoanswer.tail = 0;
	_autoanswer_enabled = false;
	#endif

	// TODO: Think about pre-assigning some UCBx registers to make the rest of the code more compact
}

void TwoWire::_allocate_buffers()
{
	if (_txbuf.buffer != NULL)
		free(_txbuf.buffer);
	if (_rxbuf.buffer != NULL)
		free(_rxbuf.buffer);

	_txbuf.buffer = (uint8_t *)malloc(_txbuf.size);
	if (_txbuf.buffer == NULL)
		return;  // Silently fail
	_txbuf.head = _txbuf.tail = 0;

	_rxbuf.buffer = (uint8_t *)malloc(_rxbuf.size);
	if (_rxbuf.buffer == NULL) {
		free(_txbuf.buffer);
		return;  // Silently fail
	}
	_rxbuf.head = _rxbuf.tail = 0;
}


int TwoWire::available()
{
	return (_rxbuf.tail - _rxbuf.head);
}

int TwoWire::read()
{
	if ( (_rxbuf.tail - _rxbuf.head) <= 0 )
		return -1;
	return _rxbuf.buffer[_rxbuf.head++];
}

int TwoWire::peek()
{
	if ( (_rxbuf.tail - _rxbuf.head) <= 0 )
		return -1;
	return _rxbuf.buffer[_rxbuf.head];
}

void TwoWire::flush()
{
	_rxbuf.head = _rxbuf.tail;
}

size_t TwoWire::write(uint8_t c)
{
	if (_txbuf.tail >= _txbuf.size)
		return 0;
	_txbuf.buffer[_txbuf.tail++] = c;
	return 1;
}

size_t TwoWire::write(const uint8_t *buf, size_t len)
{
	if ( (_txbuf.tail+len) >= _txbuf.size ) {
		len = _txbuf.size - _txbuf.tail;
	}
	if (!len)
		return 0;
	memcpy(&_txbuf.buffer[_txbuf.tail], buf, len);
	_txbuf.tail += len;
	return len;
}

void TwoWire::onReceive(TWOWIRE_SLAVE_RX_CALLBACK cb)
{
	_srx_callback[0] = cb;
}

void TwoWire::onRequest(TWOWIRE_SLAVE_TX_CALLBACK cb)
{
	_stx_callback[0] = cb;
}
