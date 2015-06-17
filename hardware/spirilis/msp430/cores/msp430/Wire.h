/* Energia-on-Arduino 1.6+ I2C Implementation
 */

#ifndef _WIRE_H
#define _WIRE_H

#include <Energia.h>






// Eric Brundick extension - ability for I2C Slave devices to auto-answer with a pre-filled buffer upon STX mode
#ifdef WIRE_ENABLE_AUTOANSWER
#define WIRE_HAS_SLAVE_AUTOANSWER 1
#endif

// Another Eric Brundick extension - txbuf, rxbuf buffers are malloc()'d and their sizes can be tweaked before .begin()
#define WIRE_HAS_VARIABLE_BUFFER_SIZES 1

// Yet another Eric Brundick extension - ability to alter I2C Master Frequency
#define WIRE_HAS_SETSPEED 1

// Yet another Eric Brundick extension
#define WIRE_ENABLE_REPEATED_START 1
#ifdef WIRE_ENABLE_REPEATED_START
#define WIRE_HAS_REPEATED_START_WRITE_READ 1
#endif

// Eric Brundick extension - Configure whether or not this instance will manage pin modes (PxSEL/etc)
#define WIRE_HAS_MANAGEPINS 1








#define DEFAULT_TWOWIRE_BUFFER_LENGTH 8
#define DEFAULT_TWOWIRE_FREQUENCY 100000UL

typedef void(*TWOWIRE_SLAVE_TX_CALLBACK)(void);
typedef void(*TWOWIRE_SLAVE_RX_CALLBACK)(size_t);

typedef struct {
	uint8_t *buffer;
	size_t size;
	volatile unsigned int head;
	volatile unsigned int tail;  // should also contain actual size of data
} TwoWire_buffer;

class TwoWire : public Stream {
	private:
		uint8_t _uscib_offset;
		boolean _do_manage_pins;
		unsigned long _i2c_master_freq;
		uint16_t _slave_addr;
		#ifdef CHIP_HAS_EUSCI
		uint16_t _slave_address_index;
		#endif

		TwoWire_buffer _txbuf;
		TwoWire_buffer _rxbuf;

		#ifdef WIRE_ENABLE_AUTOANSWER
		TwoWire_buffer _autoanswer;
		boolean _autoanswer_enabled;
		#endif
		void _allocate_buffers();

		//volatile uint8_t _twi_state, _twi_error;
		volatile uint8_t _twi_repeat_request;

		// TODO: Implement 4-slave-address mode for eUSCI
		TWOWIRE_SLAVE_TX_CALLBACK _stx_callback[1];
		TWOWIRE_SLAVE_RX_CALLBACK _srx_callback[1];

	public:
		volatile uint8_t _twi_state, _twi_error;
		TwoWire(uint8_t uscib_offset);
		void begin();
		void begin(uint8_t);
		void begin(int);
		// TODO: Implement 4-slave-address mode with eUSCI with a variation of begin()
		void end();
		void managePins(boolean do_manage_pins) { _do_manage_pins = do_manage_pins; };

		void beginTransmission(uint8_t a) { beginTransmission((int)a); };
		void beginTransmission(int);
		boolean endTransmission();
		boolean endTransmission(uint8_t);

		uint8_t requestFrom(uint8_t a, uint8_t len) { return requestFrom((int)a, (int)len); };
		uint8_t requestFrom(int, int);

		virtual size_t write(uint8_t);
		virtual size_t write(const uint8_t *, size_t);
		virtual int available();
		virtual int read();
		virtual int peek();
		virtual void flush();

		void onReceive(TWOWIRE_SLAVE_RX_CALLBACK);
		void onRequest(TWOWIRE_SLAVE_TX_CALLBACK);
		// TODO: Implement 4-slave-address mode with eUSCI with a variation of onReceive & onRequest

		inline size_t write(unsigned long n) { return write((uint8_t)n); };
		inline size_t write(long n) { return write((uint8_t)n); };
		inline size_t write(unsigned int n) { return write((uint8_t)n); };
		inline size_t write(int n) { return write((uint8_t)n); };

		// API extensions and other stuff
		#ifdef WIRE_ENABLE_AUTOANSWER
			// USCI has only 1 slave address; addrinstance variable is ignored.
			// TODO: Implement 4-slave-address I2C with addrinstance variable
			boolean setAutoAnswer(const uint8_t *, size_t);
			boolean setAutoAnswer(uint8_t addrinstance, const uint8_t *b, size_t c) { return setAutoAnswer(b,c); };

			boolean setAutoAnswer(const char *str) { return setAutoAnswer((const uint8_t *)str, strlen(str)); };
			boolean setAutoAnswer(uint8_t addrinstance, const char *b) { return setAutoAnswer((const uint8_t *)b, strlen(b)); };

			void disableAutoAnswer(uint8_t addrinst) { disableAutoAnswer(); };
			void disableAutoAnswer();
		#endif

		// REPEATED START support
		#ifdef WIRE_ENABLE_REPEATED_START
		boolean endTransmissionThenWrite();  // Sends current dataset, then returns where the user should use Wire.write some more
		boolean endTransmissionThenRead(unsigned int count);  // Sends current dataset, then requests MRX mode for <count> bytes
		#endif

		boolean isr(uint16_t intvect, boolean do_txrx_only); // Handle Interrupt Vector UCBxIV - to be run only from within the Interrupt Service Routines
		void setBufferSize(unsigned int txsize, unsigned int rxsize) { _txbuf.size = txsize; _rxbuf.size = rxsize; };
		void setSpeed(unsigned long freq) { _i2c_master_freq = freq; updateClock(F_CPU); };
		void updateClock(unsigned long smclk);  // Refreshes UCBxBRx bits based on changing SMCLK
		boolean endTransmissionWorker();  // encapsulates the USCI logic in one place

		using Print::write;
};


/* Internal bits and values */
#define TWI_IDLE  0
#define TWI_MRX   1
#define TWI_MTX   2
#define TWI_SRX   3
#define TWI_STX   4
#define TWI_REQUEST_DECISION 20

#define TWI_REPEAT_START      0x01
#define TWI_REPEAT_START_SENT	0x80

#define TWI_ERROR_NONE		0
#define TWI_ERROR_NACK		1
#define TWI_ERROR_BUS_BUSY	2
#define TWI_ERROR_SLAVE_FAULT	3


#ifdef USCI_B_COUNT
extern TwoWire Wire;
#if USCI_B_COUNT > 1
extern TwoWire Wire1;
#endif
#if USCI_B_COUNT > 2
extern TwoWire Wire2;
#endif
#if USCI_B_COUNT > 3
extern TwoWire Wire3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern const CPUFREQ_PERIPHERAL_CALLBACK TwoWire_clock_update_functions[];
#ifdef __cplusplus
}; // extern "C"
#endif

#endif /* ifdef USCI_B_COUNT */


#endif /* _WIRE_H */
