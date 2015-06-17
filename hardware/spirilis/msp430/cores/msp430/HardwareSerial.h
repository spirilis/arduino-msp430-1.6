/* Energia-on-Arduino 1.6+ HardwareSerial (USCI/eUSCI) implementation */

#ifndef HARDWARESERIAL_H
#define HARDWARESERIAL_H

#include <Energia.h>


#if defined(__MSP430_HAS_USCI__) || defined(__MSP430_HAS_USCI_A0__) || defined(__MSP430_HAS_EUSCI_A0__)

#include <inttypes.h>
#include <Stream.h>

typedef struct {
	uint8_t *buffer;
	unsigned int size;
	volatile unsigned int head;
	volatile unsigned int tail;
} usci_ring_buffer_t;

#define DEFAULT_SERIAL_BUFFER_SIZE 16

#include <usci/layout.h>

class HardwareSerial : public Stream {
	private:
		usci_ring_buffer_t *_tx_buffer;
		usci_ring_buffer_t *_rx_buffer;
		uint8_t _uscia_offset;
		unsigned long _baudrate;
		volatile uint8_t *txie;  // caching this so ::write() can run quicker
		uint8_t txie_bit;
	
	public:
		HardwareSerial(unsigned int tx_buffer_size, unsigned int rx_buffer_size, uint8_t uscia_offset);
		void setBufferSize(unsigned int txsize, unsigned int rxsize);  // Run this *before* begin()
		void begin(unsigned long, boolean do_init_pins=true);
		void end(boolean do_uninit_pins=true);
		void configClock(unsigned long freq);
		virtual int available(void);
		virtual int peek(void);
		virtual int read(void);
		virtual void flush(void);
		virtual size_t write(uint8_t);
		using Print::write;
		operator bool();
};

// C++ linkage
#ifdef USCI_A_COUNT
extern HardwareSerial Serial;
#if USCI_A_COUNT > 1
extern HardwareSerial Serial1;
#endif
#if USCI_A_COUNT > 2
extern HardwareSerial Serial2;
#endif
#if USCI_A_COUNT > 3
extern HardwareSerial Serial3;
#endif
#endif /* ifdef USCI_A_COUNT */

// C linkage (for ISR usage)
#ifdef __cplusplus
extern "C" {
#endif

#ifdef USCI_A_COUNT
extern usci_ring_buffer_t uart_ring_buffer_tx[], uart_ring_buffer_rx[];
#endif


/* Utility function used in ISR */

ALWAYS_INLINE void uscia_store_char(uint8_t uscia_offset, uint8_t c)
{
	usci_ring_buffer_t *rxbuf = &uart_ring_buffer_rx[uscia_offset];

	unsigned int i = (unsigned int) (rxbuf->head + 1) % rxbuf->size;

	// if we should be storing the received character into the location
	// just before the tail (meaning that the head would advance to the
	// current location of the tail), we're about to overflow the buffer
	// and so we don't write the character or advance the head.
	if (i != rxbuf->tail) {
		rxbuf->buffer[rxbuf->head] = c;
		rxbuf->head = i;
	}  // else ... ignore the character (buffer full)
}

#ifdef CHIP_HAS_EUSCI
ALWAYS_INLINE void uscia_xmit_char(uint8_t uscia_offset, volatile unsigned int *ucatxbuf)
#else
ALWAYS_INLINE void uscia_xmit_char(uint8_t uscia_offset, volatile uint8_t *ucatxbuf)
#endif
{
	usci_ring_buffer_t *txbuf = &uart_ring_buffer_tx[uscia_offset];

	if (txbuf->head == txbuf->tail) {
		// Buffer empty; disable interrupts
		volatile uint8_t *ie = SFRPTR_BYTE uscia_registers[uscia_offset].UCAxIE;
		volatile uint8_t *ifg = SFRPTR_BYTE uscia_registers[uscia_offset].UCAxIFG;
		*ie &= ~(uscia_registers[uscia_offset].usci_ie_tx);
		*ifg |= uscia_registers[uscia_offset].usci_ifg_tx;
		return;
	}

	uint8_t c = txbuf->buffer[txbuf->tail];
	txbuf->tail = (txbuf->tail + 1) % txbuf->size;

	*ucatxbuf = c;
}

void serialEventRun();  // used by main()

#ifdef __cplusplus
};  /* extern "C" */
#endif

#endif /* if defined(__MSP430_HAS_*USCI*) */

#endif /* HARDWARESERIAL_H */
