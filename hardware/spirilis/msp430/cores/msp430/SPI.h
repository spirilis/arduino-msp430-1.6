/* Energia-on-Arduino 1.6+ SPI Communications Peripheral support
 */

#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

#include <Energia.h>
#include <inttypes.h>
#include <usci/layout.h>
#include <usci/isr.h>


/* SPI C++ source file located in peripheral-specific folders
 * e.g. usci/spi.cpp or usi/spi.cpp
 */

// Paul Stoffregen's transactional SPI support
// Reference: https://github.com/PaulStoffregen/SPI/blob/master/SPI.h
#define SPI_HAS_TRANSACTION 1

/* Feature added by Eric Brundick for "Energia-on-Arduino"
 *
 * beginTransaction() sets a semaphore, if another piece of code attempts to run
 * beginTransaction() before endTransaction() gets run it returns false.  User code has to
 * actually check return value of beginTransaction() to act on this information; the library
 * can't stop rogue or poorly written code from running .transfer*() despite that.
 */
#define SPI_HAS_TRANSACTION_SEMAPHORE 1

// 16-bit .transfer16() support (typically two 8-bit transfers)
#define SPI_HAS_TRANSFER16 1

// 9-bit .transfer9() support (often requires crazy slow/inefficient bit banging for the 9th bit)
#define SPI_HAS_TRANSFER9 1



#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3


#define SPI_SETTINGS_MODEMASK_BITORDER 0x8000
#define SPI_SETTINGS_MODEMASK_DATAMODE 0x0003

#if defined(USCI_A_COUNT) || defined(USCI_B_COUNT)
static const uint16_t _spi_datamode_bits[] = { UCCKPH, 0, (UCCKPL | UCCKPH), UCCKPL };
#endif

class SPISettings {
	public:
		SPISettings(uint32_t spiclk, uint8_t bitorder, uint8_t dataMode) {
			spi_master_clk = spiclk;
			if (bitorder == MSBFIRST)
				modedata |= 0x8000;
			else
				modedata &= ~0x8000;

			modedata = (modedata & ~SPI_SETTINGS_MODEMASK_DATAMODE) | (dataMode & SPI_SETTINGS_MODEMASK_DATAMODE);
		}

		SPISettings() {
			spi_master_clk = 4000000UL;
			modedata = 0x8000;  // MSBFIRST, SPI_MODE0
		}

	private:
		uint32_t spi_master_clk;
		uint16_t modedata;

		uint16_t get_mode_bits() {
			uint16_t reg = 0;

			#ifdef USCI_B_COUNT
			if (modedata & SPI_SETTINGS_MODEMASK_BITORDER) {
				reg |= UCMSB;
			}
			reg |= _spi_datamode_bits[modedata & SPI_SETTINGS_MODEMASK_DATAMODE];
			#endif

			return reg;
		}

		uint16_t get_clock_divider(unsigned long smclk) {
			uint16_t quot = smclk / spi_master_clk;
			if (smclk % spi_master_clk != 0) {
				/* Partial division will leave us with a divider that produces too fast a clock
				 *
				 * Also this condition will always match if smclk < spi_master_clk,
				 * ensuring clockdiv of /1 will always end up the case there.
				 */
				quot++;
			}

			quot--;  // fBitClock = fBRCLK / (UCBRx + 1), so subtract one before setting the register.
			return quot;
		}

		friend class SPIClass;
};

class SPIClass {
	private:
		unsigned int _offset;
		SPISettings _params, _params_save;
		volatile uint8_t *_maskie;
		uint8_t _maskie_bit, _maskie_bit_saved;
		volatile boolean _semaphore;
		volatile uint8_t *txbuf, *rxbuf, *stat;

		void _save_params() {
			_params_save.spi_master_clk = _params.spi_master_clk;
			_params_save.modedata = _params.modedata;
		}

		void _copy_params(SPISettings inst) {
			_params.spi_master_clk = inst.spi_master_clk;
			_params.modedata = inst.modedata;
		}

		void _update_spi_mode();

	public:
		SPIClass(unsigned int _peripheralOffset) {
			_offset = _peripheralOffset; _maskie = NULL; _maskie_bit = 0; _semaphore = false;
			SPISettings temp_settings;  // should instantiate with default params
			_copy_params(temp_settings);
		}

		void begin(boolean do_init_pins=true);
		void end(boolean do_uninit_pins=true);

		uint8_t transfer(uint8_t);
		uint16_t transfer16(uint16_t);
		uint16_t transfer9(uint16_t);

		void setBitOrder(uint8_t bitorder);
		void setDataMode(uint8_t dataMode);

		void setClockDivider(uint16_t clockDiv) {
			_params.spi_master_clk = F_CPU / clockDiv;
			updateClock(F_CPU);
		}

		void usingInterrupt(uint16_t pin) {
			if (!deriveInterruptDetails(pin, &_maskie, &_maskie_bit)) {
				_maskie = NULL;
				_maskie_bit = 0xFF;  // Have SPI transaction just disable all interrupts
				return;
			}
		}

		boolean beginTransaction(SPISettings settings);
		void endTransaction();

		void attachInterrupt() { };  // Not implemented or used (all Arduino platforms define this but ignore it)
		void detachInterrupt() { };  // Likewise

		void updateClock(unsigned long smclk);  // Extension to support dynamic system clocking
};


/* Declared SPIClass instances */

#ifdef USCI_A_COUNT
extern SPIClass SPI4;
#define SPI_A0 SPI4
#if USCI_A_COUNT > 1
extern SPIClass SPI5;
#define SPI_A1 SPI5
#endif
#if USCI_A_COUNT > 2
extern SPIClass SPI6;
#define SPI_A2 SPI6
#endif
#if USCI_A_COUNT > 3
extern SPIClass SPI7;
#define SPI_A3 SPI7
#endif
#endif /* ifdef USCI_A_COUNT */
//
#ifdef USCI_B_COUNT
extern SPIClass SPI;
#define SPI_B0 SPI
#if USCI_B_COUNT > 1
extern SPIClass SPI1;
#define SPI_B1 SPI1
#endif
#if USCI_B_COUNT > 2
extern SPIClass SPI2;
#define SPI_B2 SPI2
#endif
#if USCI_B_COUNT > 3
extern SPIClass SPI3;
#define SPI_B3 SPI3
#endif
#endif /* ifdef USCI_B_COUNT */



#endif /* SPI_H_INCLUDED */
