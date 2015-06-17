/* Energia-on-Arduino 1.6+ SPI Communications Peripheral support
 * USCI implementation
 */

#include <SPI.h>


#if defined(USCI_A_COUNT) || defined(USCI_B_COUNT)

#define SPI_OFFSET_USCI_B_START 0
#define SPI_OFFSET_USCI_A_START 4


/* Declare SPI Instances */
#ifdef USCI_A_COUNT
SPIClass SPI4(4);
#if USCI_A_COUNT > 1
SPIClass SPI5(5);
#endif
#if USCI_A_COUNT > 2
SPIClass SPI6(6);
#endif
#if USCI_A_COUNT > 3
SPIClass SPI7(7);
#endif
#endif /* ifdef USCI_A_COUNT */
//
#ifdef USCI_B_COUNT
SPIClass SPI(0);
#if USCI_B_COUNT > 1
SPIClass SPI1(1);
#endif
#if USCI_B_COUNT > 2
SPIClass SPI2(2);
#endif
#if USCI_B_COUNT > 3
SPIClass SPI3(3);
#endif
#endif /* ifdef USCI_B_COUNT */

/* Define clock update callback functions */
extern "C" {

#ifdef USCI_A_COUNT
void SPIClass_update_clock_A0(unsigned long freq) { SPI4.updateClock(freq); }
#if USCI_A_COUNT > 1
void SPIClass_update_clock_A1(unsigned long freq) { SPI5.updateClock(freq); }
#endif
#if USCI_A_COUNT > 2
void SPIClass_update_clock_A2(unsigned long freq) { SPI6.updateClock(freq); }
#endif
#if USCI_A_COUNT > 3
void SPIClass_update_clock_A3(unsigned long freq) { SPI7.updateClock(freq); }
#endif

const CPUFREQ_PERIPHERAL_CALLBACK SPIClass_clock_update_functions_USCI_A[USCI_A_COUNT] = {
	SPIClass_update_clock_A0,
#if USCI_A_COUNT > 1
	SPIClass_update_clock_A1,
#endif
#if USCI_A_COUNT > 2
	SPIClass_update_clock_A2,
#endif
#if USCI_A_COUNT > 3
	SPIClass_update_clock_A3,
#endif
};
#endif /* ifdef USCI_A_COUNT */
//
#ifdef USCI_B_COUNT
void SPIClass_update_clock_B0(unsigned long freq) { SPI.updateClock(freq); }
#if USCI_B_COUNT > 1
void SPIClass_update_clock_B1(unsigned long freq) { SPI1.updateClock(freq); }
#endif
#if USCI_B_COUNT > 2
void SPIClass_update_clock_B2(unsigned long freq) { SPI2.updateClock(freq); }
#endif
#if USCI_B_COUNT > 3
void SPIClass_update_clock_B3(unsigned long freq) { SPI3.updateClock(freq); }
#endif

const CPUFREQ_PERIPHERAL_CALLBACK SPIClass_clock_update_functions_USCI_B[USCI_B_COUNT] = {
	SPIClass_update_clock_B0,
#if USCI_B_COUNT > 1
	SPIClass_update_clock_B1,
#endif
#if USCI_B_COUNT > 2
	SPIClass_update_clock_B2,
#endif
#if USCI_B_COUNT > 3
	SPIClass_update_clock_B3,
#endif
};
#endif /* ifdef USCI_B_COUNT */

};  /* extern "C" (C linkage for system clock update callback functions) */



void SPIClass::begin(boolean do_init_pins)
{
	_maskie = NULL;
	_maskie_bit = _maskie_bit_saved = 0;
	_semaphore = false;

	CPUFREQ_PERIPHERAL_CALLBACK periph_clock_update_callback_funcptr;
	uint32_t periph_mask_bits;
	#ifdef CHIP_HAS_EUSCI
	volatile uint16_t *ctlw;
	#else
	volatile uint8_t *ctl0, *ctl1;
	#endif
	uint16_t pin_sclk, pin_mosi, pin_miso;
	#ifdef __MSP430_HAS_PORT_MAPPING__
	uint16_t pmapid_sclk, pmapid_mosi, pmapid_miso;
	#endif

	if (_offset >= SPI_OFFSET_USCI_A_START) {
		// USCI_A logic
		txbuf = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxTXBUF;
		rxbuf = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxRXBUF;
		stat = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxSTAT;

		#ifdef CHIP_HAS_EUSCI
		ctlw = SFRPTR_WORD uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxCTL1;
		#else
		ctl0 = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxCTL0;
		ctl1 = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxCTL1;
		#endif

		periph_clock_update_callback_funcptr = SPIClass_clock_update_functions_USCI_A[_offset - SPI_OFFSET_USCI_A_START];
		periph_mask_bits = uscia_registers[_offset - SPI_OFFSET_USCI_A_START].periph_callback_mask;

		pin_sclk = uscia_pins[_offset - SPI_OFFSET_USCI_A_START].pin_clk;
		pin_mosi = uscia_pins[_offset - SPI_OFFSET_USCI_A_START].pin_txd;
		pin_miso = uscia_pins[_offset - SPI_OFFSET_USCI_A_START].pin_rxd;
		#ifdef __MSP430_HAS_PORT_MAPPING__
		pmapid_sclk = uscia_pins[_offset - SPI_OFFSET_USCI_A_START].pmapid_ste_clk & 0x00FF;
		pmapid_mosi = flipWord(uscia_pins[_offset - SPI_OFFSET_USCI_A_START].pmapid_txd_rxd & 0xFF00);
		pmapid_miso = uscia_pins[_offset - SPI_OFFSET_USCI_A_START].pmapid_txd_rxd & 0x00FF;
		#endif
	} else {
		// USCI_B logic
		txbuf = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxTXBUF;
		rxbuf = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxRXBUF;
		stat = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxSTAT;

		#ifdef CHIP_HAS_EUSCI
		ctlw = SFRPTR_WORD uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxCTL1;
		#else
		ctl0 = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxCTL0;
		ctl1 = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxCTL1;
		#endif

		periph_clock_update_callback_funcptr = SPIClass_clock_update_functions_USCI_B[_offset - SPI_OFFSET_USCI_B_START];
		periph_mask_bits = uscib_registers[_offset - SPI_OFFSET_USCI_B_START].periph_callback_mask;

		pin_sclk = uscib_pins[_offset - SPI_OFFSET_USCI_B_START].pin_clk;
		pin_mosi = uscib_pins[_offset - SPI_OFFSET_USCI_B_START].pin_txd;
		pin_miso = uscib_pins[_offset - SPI_OFFSET_USCI_B_START].pin_rxd;
		#ifdef __MSP430_HAS_PORT_MAPPING__
		pmapid_sclk = uscib_pins[_offset - SPI_OFFSET_USCI_B_START].pmapid_ste_clk & 0x00FF;
		pmapid_mosi = flipWord(uscib_pins[_offset - SPI_OFFSET_USCI_B_START].pmapid_txd_rxd & 0xFF00);
		pmapid_miso = uscib_pins[_offset - SPI_OFFSET_USCI_B_START].pmapid_txd_rxd & 0x00FF;
		#endif
	}

	#ifdef CHIP_HAS_EUSCI
	*ctlw |= UCSWRST;
	*ctlw = UCSWRST | UCSSEL_2 | _params.get_mode_bits() | UCMST | UCSYNC;
	#else
	*ctl1 |= UCSWRST;
	*ctl0 = _params.get_mode_bits() | UCMST | UCSYNC;
	*ctl1 = UCSSEL_2 | UCSWRST;
	#endif

	updateClock(F_CPU);  // This also clears UCSWRST
	cpu.enablePeripheral(periph_mask_bits, periph_clock_update_callback_funcptr);

	// Pin modes
	if (do_init_pins) {
		#ifdef __MSP430_HAS_PORT_MAPPING__
		if (pmapid_sclk) {
			/* the << 8 is b/c those pins are referenced as physical chip pins, and
			 * pinMode expects to see any physical chip pin references in the MSB of the pin
			 * argument (a uint16_t).
			 *
			 * The << 8 for the mode is being done b/c pinMode expects to see any Port Mapper
			 * configuration info in the MSB of the mode argument (a uint16_t).
			 */
			pinMode(pin_sclk & 0xFF00, pmapid_sclk << 8);
		} else {
			pinMode(pin_sclk & 0xFF00, OUTPUT | (pin_sclk & 0xFF));  // Output + PxSEL
		}

		if (pmapid_mosi) {
			pinMode(pin_mosi & 0xFF00, pmapid_mosi << 8);
		} else {
			pinMode(pin_mosi & 0xFF00, OUTPUT | (pin_mosi & 0xFF));  // Output + PxSEL
		}

		if (pmapid_miso) {
			pinMode(pin_miso & 0xFF00, pmapid_miso << 8);
		} else {
			pinMode(pin_miso & 0xFF00, pin_miso & 0xFF);  // Input + PxSEL
		}
		#else
		pinMode(pin_sclk & 0xFF00, OUTPUT | (pin_sclk & 0xFF));  // Output + PxSEL
		pinMode(pin_mosi & 0xFF00, OUTPUT | (pin_mosi & 0xFF));  // Output + PxSEL
		pinMode(pin_miso & 0xFF00, (pin_miso & 0xFF));  // Input + PxSEL
		#endif
	} /* if(do_init_pins) */
}

void SPIClass::end(boolean do_uninit_pins)
{
	uint32_t periph_mask_bits;
	volatile uint8_t *ctl1;
	uint16_t pin_sclk, pin_mosi, pin_miso;

	if (_offset >= SPI_OFFSET_USCI_A_START) {
		// USCI_A logic
		periph_mask_bits = uscia_registers[_offset - SPI_OFFSET_USCI_A_START].periph_callback_mask;
		ctl1 = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxCTL1;

		pin_sclk = uscia_pins[_offset - SPI_OFFSET_USCI_A_START].pin_clk;
		pin_mosi = uscia_pins[_offset - SPI_OFFSET_USCI_A_START].pin_txd;
		pin_miso = uscia_pins[_offset - SPI_OFFSET_USCI_A_START].pin_rxd;
	} else {
		// USCI_B logic
		periph_mask_bits = uscib_registers[_offset - SPI_OFFSET_USCI_B_START].periph_callback_mask;
		ctl1 = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxCTL1;

		pin_sclk = uscib_pins[_offset - SPI_OFFSET_USCI_B_START].pin_clk;
		pin_mosi = uscib_pins[_offset - SPI_OFFSET_USCI_B_START].pin_txd;
		pin_miso = uscib_pins[_offset - SPI_OFFSET_USCI_B_START].pin_rxd;
	}

	#ifdef CHIP_HAS_EUSCI
	*ctl1 |= UCSWRST_L;
	#else
	*ctl1 |= UCSWRST;
	#endif
	cpu.disablePeripheral(periph_mask_bits);

	// Pin modes
	if (do_uninit_pins) {
		pinMode(pin_sclk & 0xFF00, INPUT);  // Hi-Z
		pinMode(pin_mosi & 0xFF00, INPUT);  // Hi-Z
		pinMode(pin_miso & 0xFF00, INPUT);  // Hi-Z
	}
}

void SPIClass::updateClock(unsigned long smclk)
{
	#ifdef CHIP_HAS_EUSCI
	volatile uint16_t *brw;
	volatile uint16_t *ctlw;
	#else
	volatile uint8_t *br0, *br1;
	volatile uint8_t *ctl1;
	#endif

	if (_offset >= SPI_OFFSET_USCI_A_START) {
		// USCI_A logic
		#ifdef CHIP_HAS_EUSCI
		ctlw = SFRPTR_WORD uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxCTL1;
		brw = SFRPTR_WORD uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxBR0;
		#else
		ctl1 = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxCTL1;
		br0 = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxBR0;
		br1 = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxBR1;
		#endif
	} else {
		// USCI_B logic
		#ifdef CHIP_HAS_EUSCI
		ctlw = SFRPTR_WORD uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxCTL1;
		brw = SFRPTR_WORD uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxBR0;
		#else
		ctl1 = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxCTL1;
		br0 = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxBR0;
		br1 = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxBR1;
		#endif
	}

	uint16_t clkdiv = _params.get_clock_divider(smclk);  // see SPISettings class defined in SPI.h

	#ifdef CHIP_HAS_EUSCI
	*ctlw |= UCSWRST;
	*brw = clkdiv + 1;  // Unlike USCI, eUSCI interprets BRCLK exactly, with BRCLK=0 being equal to BRCLK=1
	*ctlw &= ~UCSWRST;
	#else
	*ctl1 |= UCSWRST;
	*br0 = (uint8_t) clkdiv;
	asm volatile ("swpb %[c]" : [c] "=r" (clkdiv) : "r" (clkdiv));
	*br1 = (uint8_t) clkdiv;
	*ctl1 &= ~UCSWRST;
	#endif

}

uint8_t SPIClass::transfer(uint8_t c)
{
	register volatile uint8_t *_stat = stat;

	while (*_stat & UCBUSY)
		;
	*txbuf = c;
	while (*_stat & UCBUSY)
		;
	return *rxbuf;
}

uint16_t SPIClass::transfer16(uint16_t w)
{
	uint16_t r;

	/* Optimization discovered while examining msp430-elf-gcc ASM output; when referencing a pointer stored as a private
	 * member of a class, GCC will repeatedly pull the pointer value from the class instance memory into a work register, then
	 * clobber or otherwise use that register for something else after it's done, then have to *go back* and re-retrieve
	 * the value again if it's used later in the function.  By declaring these as local copies of the class instance-stored
	 * members, we tell GCC to pick a register to store these and stick with it.  Shorter ASM output results.
	 */
	register volatile uint8_t *_txbuf = txbuf;
	register volatile uint8_t *_rxbuf = rxbuf;
	register volatile uint8_t *_stat = stat;

	while (*_stat & UCBUSY)
		;

	if (_params.modedata & SPI_SETTINGS_MODEMASK_BITORDER) {
		// MSB first
		asm volatile("swpb %[w]" : [w] "=r" (w) : "r" (w));
		*_txbuf = (uint8_t) w; // Send MSB of original data
		while (*_stat & UCBUSY)
			;
		r = *_rxbuf;  // Read MSB of return data into LSB of 'r'
		asm volatile("swpb %[r]" : [r] "=r" (r) : "r" (r));
		asm volatile("swpb %[w]" : [w] "=r" (w) : "r" (w));
		*_txbuf = (uint8_t) w;  // Send LSB of original data
		while (*_stat & UCBUSY)
			;
		r |= *_rxbuf;  // Read the LSB of return data
	} else {
		// LSB first
		*_txbuf = (uint8_t) w;  // Send LSB of original data
		while (*_stat & UCBUSY)
			;
		r = *_rxbuf;  // Read LSB of return data
		asm volatile("swpb %[r]" : [r] "=r" (r) : "r" (r));
		asm volatile("swpb %[w]" : [w] "=r" (w) : "r" (w));
		*_txbuf = (uint8_t) w;  // Send MSB of original data
		while (*_stat & UCBUSY)
			;
		r |= *_rxbuf;  // Read MSB of return data into LSB of 'r'
		asm volatile("swpb %[r]" : [r] "=r" (r) : "r" (r));
	}

	return r;
}

uint16_t SPIClass::transfer9(uint16_t w)
{
	// TODO
	return 0;
}

void SPIClass::setBitOrder(uint8_t bitorder)
{
	_params.modedata &= ~SPI_SETTINGS_MODEMASK_BITORDER;
	if (bitorder == MSBFIRST)
		_params.modedata |= SPI_SETTINGS_MODEMASK_BITORDER;

	_update_spi_mode();
}

void SPIClass::setDataMode(uint8_t dataMode)
{
	_params.modedata &= ~SPI_SETTINGS_MODEMASK_DATAMODE;
	_params.modedata |= dataMode & SPI_SETTINGS_MODEMASK_DATAMODE;
	_update_spi_mode();
}
	
void SPIClass::_update_spi_mode()
{
	#ifdef CHIP_HAS_EUSCI
	volatile uint16_t *ctlw;
	#else
	volatile uint8_t *ctl0, *ctl1;
	#endif

	if (_offset >= SPI_OFFSET_USCI_A_START) {
		// USCI_A logic
		#ifdef CHIP_HAS_EUSCI
		ctlw = SFRPTR_WORD uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxCTL1;
		#else
		ctl0 = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxCTL0;
		ctl1 = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxCTL1;
		#endif
	} else {
		// USCI_B logic
		#ifdef CHIP_HAS_EUSCI
		ctlw = SFRPTR_WORD uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxCTL1;
		#else
		ctl0 = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxCTL0;
		ctl1 = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxCTL1;
		#endif
	}

	#ifdef CHIP_HAS_EUSCI
	*ctlw |= UCSWRST;
	*ctlw = (*ctlw & 0x00FF) | _params.get_mode_bits() | UCMST | UCSYNC;
	*ctlw &= ~UCSWRST;
	#else
	*ctl1 |= UCSWRST;
	*ctl0 = _params.get_mode_bits() | UCMST | UCSYNC;
	*ctl1 &= ~UCSWRST;
	#endif
}

boolean SPIClass::beginTransaction(SPISettings settings) {
	// Semaphore check & set should be an atomic operation
	uint16_t gie_save = __get_SR_register() & GIE;
	_DINT();
	if (_semaphore) {
		__bis_SR_register(gie_save);
		asm volatile("nop");
		return false;
	}

	_semaphore = true;
	__bis_SR_register(gie_save);
	asm volatile("nop");
	// Done with semaphore operations

	if (_maskie != NULL) {
		// GPIO interrupt registered; mask it!
		_maskie_bit_saved = *_maskie & _maskie_bit;
		*_maskie &= ~_maskie_bit;
	} else {
		if (_maskie_bit == 0xFF) {  // Non-GPIO interrupt source; disable GIE!
			_DINT();
		}
	}

	// Copy new SPI settings for the duration of this transaction
	_save_params();
	_copy_params(settings);
	if (_params.modedata == _params_save.modedata && _params.spi_master_clk == _params_save.spi_master_clk)
		return true;  // No need to modify the peripheral

	// Update peripheral with new settings
	volatile uint8_t *ctl0, *ctl1;

	if (_offset >= SPI_OFFSET_USCI_A_START) {
		// USCI_A logic
		ctl0 = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxCTL0;
		ctl1 = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxCTL1;
	} else {
		// USCI_B logic
		ctl0 = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxCTL0;
		ctl1 = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxCTL1;
	}

	*ctl1 |= UCSWRST;
	*ctl0 = _params.get_mode_bits() | UCMST | UCSYNC;
	updateClock(F_CPU);

	return true;
}

void SPIClass::endTransaction() {
	_semaphore = false;

	// Restore original SPI settings
	if (_params.modedata != _params_save.modedata || _params.spi_master_clk != _params_save.spi_master_clk) {
		_copy_params(_params_save);

		volatile uint8_t *ctl0, *ctl1;

		if (_offset >= SPI_OFFSET_USCI_A_START) {
			// USCI_A logic
			ctl0 = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxCTL0;
			ctl1 = SFRPTR_BYTE uscia_registers[_offset - SPI_OFFSET_USCI_A_START].UCAxCTL1;
		} else {
			// USCI_B logic
			ctl0 = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxCTL0;
			ctl1 = SFRPTR_BYTE uscib_registers[_offset - SPI_OFFSET_USCI_B_START].UCBxCTL1;
		}

		*ctl1 |= UCSWRST;
		*ctl0 = _params.get_mode_bits() | UCMST | UCSYNC;
		updateClock(F_CPU);
	}

	// Re-enable masked interrupts (if applicable)
	if (_maskie != NULL) {
		*_maskie |= _maskie_bit_saved;
	} else {
		if (_maskie_bit == 0xFF) {  // Non-GPIO interrupt source; re-enable GIE!
			_EINT();
		}
	}
}



#endif /* USCI_A_COUNT or USCI_B_COUNT defined */
