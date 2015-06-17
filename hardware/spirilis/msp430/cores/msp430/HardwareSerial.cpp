/* Energia-on-Arduino 1.6+ HardwareSerial (USCI/eUSCI) implementation */

#define IS_HARDWARESERIAL_CPP
#include <HardwareSerial.h>


/* USCI/eUSCI ISRs */
#include <usci/isr.h>


// Basic init constructor (no hardware init here)
HardwareSerial::HardwareSerial(unsigned int tx_buffer_size, unsigned int rx_buffer_size, uint8_t uscia_offset)
{
	_tx_buffer = &uart_ring_buffer_tx[uscia_offset];
	_rx_buffer = &uart_ring_buffer_rx[uscia_offset];

	_tx_buffer->size = tx_buffer_size;
	_rx_buffer->size = rx_buffer_size;
	_uscia_offset = uscia_offset;
	_tx_buffer->buffer = NULL;
	_tx_buffer->head = 0;
	_tx_buffer->tail = 0;
	_rx_buffer->buffer = NULL;
	_rx_buffer->head = 0;
	_rx_buffer->tail = 0;
	_baudrate = 9600;  // Just a default; begin() overrides this
}


/* Declaration of Serial instances */
#ifdef USCI_A_COUNT
HardwareSerial Serial(DEFAULT_SERIAL_BUFFER_SIZE, DEFAULT_SERIAL_BUFFER_SIZE, 0);
#if USCI_A_COUNT > 1
HardwareSerial Serial1(DEFAULT_SERIAL_BUFFER_SIZE, DEFAULT_SERIAL_BUFFER_SIZE, 1);
#endif
#if USCI_A_COUNT > 2
HardwareSerial Serial2(DEFAULT_SERIAL_BUFFER_SIZE, DEFAULT_SERIAL_BUFFER_SIZE, 2);
#endif
#if USCI_A_COUNT > 3
HardwareSerial Serial3(DEFAULT_SERIAL_BUFFER_SIZE, DEFAULT_SERIAL_BUFFER_SIZE, 3);
#endif
#endif /* ifdef USCI_A_COUNT */


/* Hard C functions to handle per-Serial instance callbacks (serialEvent and cpu.updatePeripherals()) */
extern "C" {

#ifdef USCI_A_COUNT
usci_ring_buffer_t uart_ring_buffer_tx[USCI_A_COUNT], uart_ring_buffer_rx[USCI_A_COUNT];

void HardwareSerial_clock_update_A0(unsigned long freq) { Serial.configClock(freq); }
#if USCI_A_COUNT > 1
void HardwareSerial_clock_update_A1(unsigned long freq) { Serial1.configClock(freq); }
#endif
#if USCI_A_COUNT > 2
void HardwareSerial_clock_update_A2(unsigned long freq) { Serial2.configClock(freq); }
#endif
#if USCI_A_COUNT > 3
void HardwareSerial_clock_update_A3(unsigned long freq) { Serial3.configClock(freq); }
#endif

const CPUFREQ_PERIPHERAL_CALLBACK HardwareSerial_clock_update_functions[USCI_A_COUNT] = {
	HardwareSerial_clock_update_A0,
	#if USCI_A_COUNT > 1
	HardwareSerial_clock_update_A1,
	#endif
	#if USCI_A_COUNT > 2
	HardwareSerial_clock_update_A2,
	#endif
	#if USCI_A_COUNT > 3
	HardwareSerial_clock_update_A3
	#endif
};

void serialEvent() __attribute__((weak));
void serialEvent() { }
#if USCI_A_COUNT > 1
void serial1Event() __attribute__((weak));
void serial1Event() { }
#endif
#if USCI_A_COUNT > 2
void serial2Event() __attribute__((weak));
void serial2Event() { }
#endif
#if USCI_A_COUNT > 3
void serial3Event() __attribute__((weak));
void serial3Event() { }
#endif

void serialEventRun()
{
	if (Serial.available()) serialEvent();
	#if USCI_A_COUNT > 1
		if (Serial1.available()) serial1Event();
	#endif
	#if USCI_A_COUNT > 2
		if (Serial2.available()) serial2Event();
	#endif
	#if USCI_A_COUNT > 3
		if (Serial3.available()) serial3Event();
	#endif
}

#endif /* ifdef USCI_A_COUNT */
}; /* extern "C" */




// Init USCI
void HardwareSerial::begin(unsigned long baudrate, boolean do_init_pins)
{
	usci_layout_dummy();  // Ignore; just used to ensure usci/layout.cpp gets linked in
	usci_isr_dummy();  // Ignore; just used to ensure usci/isr.cpp gets linked in

	volatile uint8_t *ie = SFRPTR_BYTE uscia_registers[_uscia_offset].UCAxIE;
	*ie &= ~(uscia_registers[_uscia_offset].usci_ie_rx | uscia_registers[_uscia_offset].usci_ie_tx);

	txie = ie;
	txie_bit = uscia_registers[_uscia_offset].usci_ie_tx;

	volatile uint8_t *ifg = SFRPTR_BYTE uscia_registers[_uscia_offset].UCAxIFG;
	*ifg &= ~(uscia_registers[_uscia_offset].usci_ifg_rx);

	_baudrate = baudrate;

	// Allocate ring buffers
	if (_tx_buffer->buffer != NULL)
		free(_tx_buffer->buffer);
	if (_rx_buffer->buffer != NULL)
		free(_rx_buffer->buffer);
	_tx_buffer->buffer = (uint8_t *) malloc(_tx_buffer->size);
	_rx_buffer->buffer = (uint8_t *) malloc(_rx_buffer->size);
	if (_tx_buffer->buffer == NULL || _rx_buffer->buffer == NULL)
		return;  // Quietly fail, as our heap allocation failed!
	_tx_buffer->head = 0;
	_tx_buffer->tail = 0;
	_rx_buffer->head = 0;
	_rx_buffer->tail = 0;

	configClock(F_CPU);  // This cancels UCSWRST upon exit

	*ifg |= uscia_registers[_uscia_offset].usci_ifg_tx;
	*ie |= uscia_registers[_uscia_offset].usci_ie_rx;

	// UART active
	cpu.enablePeripheral(uscia_registers[_uscia_offset].periph_callback_mask, HardwareSerial_clock_update_functions[_uscia_offset]);

	// Set Pin Modes
	if (do_init_pins) {
		#ifdef __MSP430_HAS_PORT_MAPPING__
		if (uscia_pins[_uscia_offset].pmapid_txd_rxd) {  // Port Mapper used to configure this one
			pinMode(uscia_pins[_uscia_offset].pin_txd & 0xFF00, uscia_pins[_uscia_offset].pmapid_txd_rxd & 0xFF00);  // PMAPID in upper 8 bits
			pinMode(uscia_pins[_uscia_offset].pin_rxd & 0xFF00, (uscia_pins[_uscia_offset].pmapid_txd_rxd & 0x00FF) << 8);  // PMAPID in upper 8 bits
		} else {
			pinMode(uscia_pins[_uscia_offset].pin_txd & 0xFF00, OUTPUT | (uscia_pins[_uscia_offset].pin_txd & 0xFF));  // Pin set to Output + PxSEL
			pinMode(uscia_pins[_uscia_offset].pin_rxd & 0xFF00, uscia_pins[_uscia_offset].pin_rxd & 0xFF);  // Pin set to Input + PxSEL
		}
		#else
		pinMode(uscia_pins[_uscia_offset].pin_txd & 0xFF00, OUTPUT | (uscia_pins[_uscia_offset].pin_txd & 0xFF));  // Pin set to Output + PxSEL
		pinMode(uscia_pins[_uscia_offset].pin_rxd & 0xFF00, uscia_pins[_uscia_offset].pin_rxd & 0xFF);  // Pin set to Input + PxSEL
		#endif /* ifdef MSP430_HAS_PORT_MAPPING */
	} /* if(do_init_pins) */
}

void HardwareSerial::end(boolean do_uninit_pins)
{
	// wait for transmission of outgoing data
	while (_tx_buffer->head != _tx_buffer->tail)
		;

	volatile uint8_t *ctl1 = SFRPTR_BYTE uscia_registers[_uscia_offset].UCAxCTL1;

	#ifdef CHIP_HAS_EUSCI
	*ctl1 |= UCSWRST_L;
	#else
	*ctl1 |= UCSWRST;
	#endif

	// UCxIE & UCxIFG gets cleared after UCSWRST so no need to clear it
	cpu.disablePeripheral(uscia_registers[_uscia_offset].periph_callback_mask);

	_rx_buffer->head = _rx_buffer->tail;

	// Clear Pin Modes
	if (do_uninit_pins) {
		pinMode(uscia_pins[_uscia_offset].pin_txd & 0xFF00, INPUT);  // Hi-Z
		pinMode(uscia_pins[_uscia_offset].pin_rxd & 0xFF00, INPUT);  // Hi-Z
	}
}

// This is also the method used by the callback for updating peripheral clocks
void HardwareSerial::configClock(unsigned long smclk_freq)
{
	unsigned int mod;
	unsigned long divider;
	unsigned char oversampling = 0;

	#ifdef CHIP_HAS_EUSCI
	volatile uint16_t *ctlw = SFRPTR_WORD uscia_registers[_uscia_offset].UCAxCTL1;
	volatile uint16_t *brw = SFRPTR_WORD uscia_registers[_uscia_offset].UCAxBR0;
	volatile uint16_t *mctl = SFRPTR_WORD uscia_registers[_uscia_offset].UCAxMCTL;
	#else
	volatile uint8_t *ctl0 = SFRPTR_BYTE uscia_registers[_uscia_offset].UCAxCTL0;
	volatile uint8_t *ctl1 = SFRPTR_BYTE uscia_registers[_uscia_offset].UCAxCTL1;
	volatile uint8_t *br0 = SFRPTR_BYTE uscia_registers[_uscia_offset].UCAxBR0;
	volatile uint8_t *br1 = SFRPTR_BYTE uscia_registers[_uscia_offset].UCAxBR1;
	volatile uint8_t *mctl = SFRPTR_BYTE uscia_registers[_uscia_offset].UCAxMCTL;
	#endif

	volatile uint8_t *abctl = SFRPTR_BYTE uscia_registers[_uscia_offset].UCAxABCTL;
	volatile uint8_t *ie = SFRPTR_BYTE uscia_registers[_uscia_offset].UCAxIE;
	volatile uint8_t *ifg = SFRPTR_BYTE uscia_registers[_uscia_offset].UCAxIFG;
	uint8_t _txie_bit = uscia_registers[_uscia_offset].usci_ie_tx;
	uint8_t _rxie_bit = uscia_registers[_uscia_offset].usci_ie_rx;
	uint8_t _txifg_bit = uscia_registers[_uscia_offset].usci_ifg_tx;

	uint8_t _txie_saved = *ie & _txie_bit;

	#ifdef CHIP_HAS_EUSCI
 	*ctlw = (*ctlw & 0xFF00) | UCSWRST;
	#else
 	*ctl1 = UCSWRST;
	#endif

	if (smclk_freq < 500000UL) {  // Sleep Walking?  Return with UART disabled.
		return;
	}

	#ifdef CHIP_HAS_EUSCI
	*ctlw = UCSSEL_2 | UCSWRST;
	#else
	*ctl1 |= UCSSEL_2;
	*ctl0 = 0;
	#endif
	*abctl = 0;

	if (smclk_freq / _baudrate >= 48)
		oversampling = 1;

	divider = (smclk_freq << 4) / _baudrate;

	if (!oversampling) {
		mod = ((divider & 0x0F) + 1) & 0x0E;     // UCBRSx (bit 1-3)
		divider >>= 4;
	} else {
		mod = divider & 0xFFF0;                  // UCBRFx = INT([(N/16) <96> INT(N/16)] <D7> 16)
		divider >>= 8;
	}
	#ifdef CHIP_HAS_EUSCI
		*brw = (uint16_t) divider;
		*mctl = (oversampling ? UCOS16 : 0x0000) | mod;
	#else
		uint16_t div16 = (uint16_t) divider;
		*br0 = (uint8_t) div16;
		asm volatile("swpb %[d]" : [d] "=r" (div16) : "r" (div16));
		*br1 = (uint8_t) div16;
		*mctl = (uint8_t)(oversampling ? UCOS16 : 0x00) | mod;
	#endif

	// Bring USCI_A out of reset, restore UCxIE settings
	#ifdef CHIP_HAS_EUSCI
	*ctlw &= ~UCSWRST;
	#else
	*ctl1 &= ~UCSWRST;
	#endif
	*ifg |= _txifg_bit;
	*ie |= _txie_saved | _rxie_bit;

}

int HardwareSerial::available()
{
	return (unsigned int) (_rx_buffer->size + _rx_buffer->head - _rx_buffer->tail) % _rx_buffer->size;
}

int HardwareSerial::peek(void)
{
	if (_rx_buffer->head == _rx_buffer->tail) {
		return -1;
	} else {
		return _rx_buffer->buffer[_rx_buffer->tail];
	}
}

int HardwareSerial::read(void)
{
	// if the head isn't ahead of the tail, we don't have any characters
	if (_rx_buffer->head == _rx_buffer->tail) {
		return -1;
	} else {
		uint8_t c = _rx_buffer->buffer[_rx_buffer->tail];
		_rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % _rx_buffer->size;
		return c;
	}
}

void HardwareSerial::flush()
{
	while (_tx_buffer->head != _tx_buffer->tail)
		;
}

size_t HardwareSerial::write(uint8_t c)
{
	unsigned int i = (_tx_buffer->head + 1) % _tx_buffer->size;

	// If the output buffer is full, there's nothing for it other than to
	// wait for the interrupt handler to empty it a bit
	if (i == _tx_buffer->tail) {
		// Buffer is full - determine what to do next.
		//
		volatile uint8_t *ctl1 = SFRPTR_BYTE uscia_registers[_uscia_offset].UCAxCTL1;
		#ifdef CHIP_HAS_EUSCI
		if (*ctl1 & UCSWRST_L) {
		#else
		if (*ctl1 & UCSWRST) {
		#endif
			// Peripheral suspended; we'll be getting nowhere by busy-waiting!
			return 0;
		}
		//
		//
		if ( !(__get_SR_register() & GIE) ) {
			// Running in interrupt context or with interrupts disabled means we would sit here forever!
			return 0;
		}

		// Else - Just busy-wait until a slot opens up.
		while (i == _tx_buffer->tail)
			;
	}

	_tx_buffer->buffer[_tx_buffer->head] = c;
	_tx_buffer->head = i;

	*txie |= txie_bit;

	return 1;
}

HardwareSerial::operator bool()
{
	volatile uint8_t *ctl1 = SFRPTR_BYTE uscia_registers[_uscia_offset].UCAxCTL1;

	#ifdef CHIP_HAS_EUSCI
	if (*ctl1 & UCSWRST_L) {
	#else
	if (*ctl1 & UCSWRST) {
	#endif
		return false;
	}
	return true;
}

// This should be run *before* begin() if you want to modify the buffer sizes.
void HardwareSerial::setBufferSize(unsigned int txsize, unsigned int rxsize)
{
	_tx_buffer->size = txsize;
	_rx_buffer->size = rxsize;
}
