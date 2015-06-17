/* Energia-on-Arduino 1.6+ System Clock Request
 *
 * 2015 Eric Brundick <spirilis at linux dot com>
 */

#define IS_CPUFREQ_CPP
#include <Energia.h>
#include <cpu/cpufreq.h>
#include <cpu/tlv.h>
#include <cpu/pmm.h>
#include <pins_booster.h>


CPUFREQ cpu;
CPUACLK aclk;

CPUFREQ::CPUFREQ()
{
	/* This needs to be done by main(), as it needs to be set BEFORE the C++ constructors execute. */
}

CPUACLK::CPUACLK()
{
	/* This needs to be done by main(), as it needs to be set BEFORE the C++ constructors execute. */
}

#ifdef __MSP430_HAS_UCS__
const uint8_t _dcorsel_lookup_table[] = {
	1, 2, 6, 10, 16, 20, 25
};

ALWAYS_INLINE uint16_t _dcorsel_compute(unsigned long freq)
{
	int i = 0;
	uint8_t sfreq = (uint8_t) (freq / 1000000UL);
	for (i=0; i < 7; i++) {
		if (sfreq <= _dcorsel_lookup_table[i])
			return DCORSEL_1 * i;
	}
	return DCORSEL_1 * i;
}

const uint8_t _flld_lookup_table[] = {
	32, 64, 128
};

ALWAYS_INLINE uint16_t _flld_compute(unsigned long freq)
{
	int i = 0;
	uint8_t sfreq = (uint8_t) (freq / 1000000UL);
	for (i=0; i < 3; i++) {
		if (sfreq <= _flld_lookup_table[i])
			return i+1;
	}
	return 1;

	/*
	if (freq <= 32000000)
		return 1;
	if (freq <= 64000000)
		return 2;
	if (freq <= 128000000)
		return 3;
	return 1;
	 */
}

boolean _dcofll_activate(unsigned long freq)
{
	// Initialize DCO
	__bis_SR_register(SCG0);  // Disable FLL control loop
	UCSCTL0 = 0x0000;

	// Determine which DCORSEL we should use
	UCSCTL1 = _dcorsel_compute(freq);

	// FLL reference is 32768Hz, determine multiplier
	uint16_t flld = _flld_compute(freq);
	UCSCTL2 = ((flld/2) << 12) | (uint16_t)(freq / 32768UL / flld);

	__bic_SR_register(SCG0);  // Re-enable FLL control loop

	// Loop until XT1 & DCO fault flags have cleared
	do {
		#ifdef XT1HFOFFG
		UCSCTL7 &= ~(XT1LFOFFG | XT1HFOFFG | DCOFFG);
		#else
		UCSCTL7 &= ~(XT1LFOFFG | DCOFFG);
		#endif
		SFRIFG1 &= ~OFIFG;
		__delay_cycles(8000);
	} while (UCSCTL7 & DCOFFG);  // TODO: Time out at some point, return false

	return true;
}
#endif /* ifdef __MSP430_HAS_UCS__ */

// MSP430 FR58xx/59xx/6xxx series (Wolverine)
#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
inline void _fram_set_waitstates(unsigned long freq)
{
	uint16_t waits = (uint16_t) ( (freq-1) / 8000000UL );
	FRCTL0 = FRCTLPW | (NWAITS0 * waits);
}
#endif

boolean CPUFREQ::setDCO(unsigned long freq)
{
	if (freq == cpu.mclk())
		return true;  // Nothing to do here!

	// MSP430 F2xxx & G2xxx Value-Line
	#ifdef __MSP430_HAS_BC2__
		uint8_t caldco, calbc1;

		// Some chips use TLV tags, some have the constant locations hardcoded...
		#ifdef TAG_DCO_30  // TLV-based (G2553 is like this)
			uint8_t *dco_tlv = (uint8_t *)tlv_find(TAG_DCO_30, NULL);
			if (dco_tlv) {
				switch (freq) {
					case 1000000UL:
						caldco = *(dco_tlv + CAL_DCO_1MHZ);
						calbc1 = *(dco_tlv + CAL_BC1_1MHZ);
						break;
					case 8000000UL:
						caldco = *(dco_tlv + CAL_DCO_8MHZ);
						calbc1 = *(dco_tlv + CAL_BC1_8MHZ);
						break;
					case 12000000UL:
						caldco = *(dco_tlv + CAL_DCO_12MHZ);
						calbc1 = *(dco_tlv + CAL_BC1_12MHZ);
						break;
					case 16000000UL:
						caldco = *(dco_tlv + CAL_DCO_16MHZ);
						calbc1 = *(dco_tlv + CAL_BC1_16MHZ);
						break;
					default:
						return false;  // Invalid frequency requested!
				}
			} else {
				return false;  // Error!
			}
		#else
		// Constant (G2452 is like this)
		switch (freq) {
			case 1000000UL:
				caldco = CALDCO_1MHZ;
				calbc1 = CALBC1_1MHZ;
				break;
			case 8000000UL:
				caldco = CALDCO_8MHZ;
				calbc1 = CALBC1_8MHZ;
				break;
			case 12000000UL:
				caldco = CALDCO_12MHZ;
				calbc1 = CALBC1_12MHZ;
				break;
			case 16000000UL:
				caldco = CALDCO_16MHZ;
				calbc1 = CALBC1_16MHZ;
				break;
			default:
				return false;  // Invalid frequency requested!
		}
		#endif

		DCOCTL = caldco;
		BCSCTL1 = calbc1;
		BCSCTL2 = 0x00;  // DIVM, DIVS = /1
	#endif

	// MSP430 F5xxx/6xxx
	#ifdef __MSP430_HAS_UCS__
		#if !defined(UCS_LFXT_PORT) || !defined(UCS_LFXT_XIN) || !defined(UCS_LFXT_XOUT)
		#error MSP430F5xxx/6xxx chip detected but variants config didnt include LFXT port/pin information!
		#endif
		aclk.getState();
		UCSCTL4 = (UCSCTL4 & ~(SELM_7 | SELS_7)) | SELM__DCOCLK | SELS__DCOCLK;
		if (aclk.state_mask() & CPUACLK_STATE_MASK_LFXT_RUNNING) {
			// FLL ref = LFXT
			UCSCTL3 = SELREF__XT1CLK;
		} else {
			// FLL ref = REFO
			UCSCTL3 = SELREF__REFOCLK;
		}
		if (freq < cpu.mclk()) {
			// Always lower CPU frequency before stepping down Core Voltage
			if (!_dcofll_activate(freq))
				return false;
			pmm_set_core_voltage(pmm_compute_core_voltage(freq));
		} else {
			// Raise PMMCoreV before raising CPU frequency
			pmm_set_core_voltage(pmm_compute_core_voltage(freq));
			if (!_dcofll_activate(freq))
				return false;
		}
		return true;
	#endif

	// MSP430 FR57xx (early FRAM series)
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_FRAM_FR5XX__)
	#endif

	// MSP430 FR58xx/59xx/6xxx series (Wolverine)
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
		uint16_t _csctl3, _csctl1;

		switch (freq) {
			case 1000000:
				_csctl3 = DIVA__1 | DIVS__1 | DIVM__1;
				_csctl1 = DCOFSEL_0;
				break;
			case 2670000:
				_csctl3 = DIVA__1 | DIVS__1 | DIVM__1;
				_csctl1 = DCOFSEL_1;
				break;
			case 3330000:
				_csctl3 = DIVA__1 | DIVS__1 | DIVM__1;
				_csctl1 = DCOFSEL_2;
				break;
			case 4000000:
				_csctl3 = DIVA__1 | DIVS__1 | DIVM__1;
				_csctl1 = DCOFSEL_3;
				break;
			case 5330000:
				_csctl3 = DIVA__1 | DIVS__1 | DIVM__1;
				_csctl1 = DCOFSEL_1 | DCORSEL;
				break;
			case 6670000:
				_csctl3 = DIVA__1 | DIVS__1 | DIVM__1;
				_csctl1 = DCOFSEL_2 | DCORSEL;
				break;
			case 8000000:
				_csctl3 = DIVA__1 | DIVS__1 | DIVM__1;
				_csctl1 = DCOFSEL_6;
				break;
			case 10500000:
				_csctl3 = DIVA__1 | DIVS__2 | DIVM__2;  // DCO = 21MHz, MCLK = DCO/2
				_csctl1 = DCOFSEL_5 | DCORSEL;
				break;
			case 12000000:
				_csctl3 = DIVA__1 | DIVS__2 | DIVM__2;  // DCO = 24MHz, MCLK = DCO/2
				_csctl1 = DCOFSEL_6 | DCORSEL;
				break;
			case 16000000:
				_csctl3 = DIVA__1 | DIVS__1 | DIVM__1;
				_csctl1 = DCOFSEL_4 | DCORSEL;
				break;
			default:
				return false;
		}

		// Configure clock
		CSCTL0_H = CSKEY_H;

		if (freq > cpu.mclk())  // Raising clock speed?  Set FRAM wait states *now*
			_fram_set_waitstates(freq);

		CSCTL3 = _csctl3;
		CSCTL2 = (CSCTL2 & SELA_7) | SELM__DCOCLK | SELS__DCOCLK;
		CSCTL1 = _csctl1;

		if (freq < cpu.mclk())  // Lowering clock speed?  Now's a safe time to lower FRAM wait states.
			_fram_set_waitstates(freq);

		CSCTL0_H = 0;
		return true;
	#endif

	// MSP430 FR4xxx/2xxx "Value Line" FRAM chips
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && !defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
	#endif

	// MSP430i series
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_MSP430I_CPU__)
	#endif

	return false;
}


boolean CPUFREQ::setHFXT(unsigned long freq)
{
	// MSP430 F2xxx & G2xxx Value-Line
	#ifdef __MSP430_HAS_BC2__
	#endif

	// MSP430 F5xxx/6xxx
	#ifdef __MSP430_HAS_UCS__
		#if !defined(XT2OFF) || !defined(SELM__XT2CLK)
			return false;  // HFXT not supported (or not supported for MCLK/SMCLK)!
		#endif
		// Check PMMCoreV to see if we need to raise it first
		if (freq > cpu.mclk()) {
			pmm_set_core_voltage(pmm_compute_core_voltage(freq));
		}

		// Activate HFXT function on port pin
		#if UCS_HFXT_PORT != 0
		*((uint8_t *)((uint8_t *)&UCS_HFXT_PORT + 0x0A)) |= UCS_HFXT_XIN | UCS_HFXT_XOUT;  // PxSEL enable
		#endif

		// Switch on HFXT driver
		UCSCTL6 &= ~XT2OFF;
		UCSCTL7 = 0x00;
		while (UCSCTL7 & XT2OFFG) {
			UCSCTL7 = 0x00;
			SFRIFG1 &= ~OFIFG;
			__delay_cycles(8000);  // TODO: Time out at some point, put chip in a stable state and return false.
		}

		// HFXT spun up, make active
		UCSCTL4 = (UCSCTL4 & ~(SELM_7 | SELS_7)) | SELM__XT2CLK | SELS__XT2CLK;

		// Shut off FLL for DCO
		__bis_SR_register(SCG0);

		// Did we drop in frequency?  If so, lower PMMCoreV if needed.
		if (freq < cpu.mclk()) {
			pmm_set_core_voltage(pmm_compute_core_voltage(freq));
		}

		return true;  // All set!
	#endif

	// MSP430 FR57xx (early FRAM series)
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_FRAM_FR5XX__)
	#endif

	// MSP430 FR58xx/59xx/6xxx series (Wolverine)
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
		#ifndef SELM__HFXTCLK
		return false;
		#else
		#ifdef CS_HFXT_PORT
		// Activate HFXT function on port pin
		*((uint8_t *)((uint8_t *)&CS_HFXT_PORT + 0x0A)) |= CS_HFXT_XIN | CS_HFXT_XOUT;     // PxSEL0 enable
		*((uint8_t *)((uint8_t *)&CS_HFXT_PORT + 0x0C)) &= ~(CS_HFXT_XIN | CS_HFXT_XOUT);  // PxSEL1 disable
		#else
		return false;  // No HFXT support in this variant
		#endif /* ifdef CS_HFXT_PORT */

		// Switch on HFXT driver
		CSCTL0_H = CSKEY_H;

		uint16_t freqrange = 0;
		if (freq > 4000000UL) freqrange++;
		if (freq > 8000000UL) freqrange++;
		if (freq > 16000000UL) freqrange++;
		CSCTL4 = (CSCTL4 & 0x00FF) | HFXTDRIVE_3 | (HFFREQ_1 * freqrange);

		while (CSCTL5 & HFXTOFFG) {
			CSCTL5 &= ~HFXTOFFG;
			SFRIFG1 &= ~OFIFG;
			__delay_cycles(8000);  // TODO: Time out at some point, put chip in a stable state and return false.
		}

		// HFXT spun up; make active
		if (freq > cpu.mclk())  // Raising CPU speed?  Set FRAM wait states *now*
			_fram_set_waitstates(freq);

		CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
		CSCTL2 = (CSCTL2 & ~(SELA_7)) | SELM__HFXTCLK | SELS__HFXTCLK;

		if (freq < cpu.mclk())  // Lowering CPU speed?  Now is a safe time to lower FRAM wait states.
			_fram_set_waitstates(freq);

		CSCTL0_H = 0;
		return true;  // All done!
		#endif /* ifndef SELM__HFXTCLK */
	#endif

	// MSP430 FR4xxx/2xxx "Value Line" FRAM chips
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && !defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
	#endif

	// MSP430i series
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_MSP430I_CPU__)
	#endif

	return false;
}

boolean CPUFREQ::requestSleepWalk()
{
	// MSP430 F2xxx & G2xxx Value-Line
	#ifdef __MSP430_HAS_BC2__
		set_mclk(aclk.aclk());
		set_wdt_setting(WDT_MDLY_8);
		set_micros_per_wdt(1000000UL / (_mclk / 8192));

		BCSCTL2 = SELM_3 | SELS;
		WDTCTL = wdt_setting();
		return true;
	#endif

	// MSP430 F5xxx/6xxx
	#ifdef __MSP430_HAS_UCS__
		// We leave VCore at whatever setting it is to avoid painstakingly messing with it while running on ACLK...
		__bis_SR_register(SCG0);  // Disable FLL since we don't need it
		uint16_t st = aclk.getState();
		set_mclk(aclk.aclk());
		set_wdt_setting(WDT_MDLY_8);
		set_micros_per_wdt(1000000UL / (_mclk / 8192));

		if (st & CPUACLK_STATE_MASK_USING_VLO) {
			UCSCTL4 = SELM__VLOCLK | SELS__VLOCLK | SELA__VLOCLK;
			WDTCTL = wdt_setting();
			return true;
		}
		if (st & CPUACLK_STATE_MASK_USING_REFO) {
			UCSCTL4 = SELM__REFOCLK | SELS__REFOCLK | SELA__REFOCLK;
			WDTCTL = wdt_setting();
			return true;
		}
		UCSCTL4 = SELM__XT1CLK | SELS__XT1CLK | SELA__XT1CLK;
		WDTCTL = wdt_setting();
		return true;
	#endif

	// MSP430 FR57xx (early FRAM series)
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_FRAM_FR5XX__)
	#endif

	// MSP430 FR58xx/59xx/6xxx series (Wolverine)
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
		uint16_t st = aclk.getState();
		set_mclk(aclk.aclk());
		set_wdt_setting(WDT_MDLY_8);
		set_micros_per_wdt(1000000UL / (_mclk / 8192));

		CSCTL0_H = CSKEY_H;

		// VLO
		if (st & CPUACLK_STATE_MASK_USING_VLO) {
			CSCTL2 = SELA__VLOCLK | SELS__VLOCLK | SELM__VLOCLK;
			CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
			_fram_set_waitstates(_mclk);
			CSCTL0_H = 0;
			return true;
		}
		// LFMODCLK
		if (st & CPUACLK_STATE_MASK_USING_LFMODCLK) {
			CSCTL2 = SELA__LFMODCLK | SELS__LFMODCLK | SELM__LFMODCLK;
			CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
			_fram_set_waitstates(_mclk);
			CSCTL0_H = 0;
			return true;
		}
		// XT1
		CSCTL2 = SELA__LFXTCLK | SELS__LFXTCLK | SELM__LFXTCLK;
		CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
		_fram_set_waitstates(_mclk);
		CSCTL0_H = 0;
		return true;
	#endif

	// MSP430 FR4xxx/2xxx "Value Line" FRAM chips
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && !defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
	#endif

	// MSP430i series
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_MSP430I_CPU__)
	#endif

	return false;
}

void CPUFREQ::updatePeripherals()
{
	unsigned int cbidx = 0;
	unsigned long mark = 1;
	int i;

	for (i=0; i < 32; i++) {
		if (_periph_mask & mark) {
			if (_periph_active & mark) {
				callbacks[cbidx](_mclk);
			}
			cbidx++;
			mark *= 2;
		}
	}
}

boolean CPUFREQ::enablePeripheral(uint32_t bit, CPUFREQ_PERIPHERAL_CALLBACK callback)
{
	unsigned int cbidx = 0;
	unsigned long mark = 1;
	int i;

	for (i=0; i < 32; i++) {
		if (_periph_mask & mark) {
			if (bit == mark) {
				callbacks[cbidx] = callback;
				return true;
			}
			cbidx++;
			mark *= 2;
		}
	}

	return false;
}

boolean CPUFREQ::disablePeripheral(uint32_t bit)
{
	unsigned int cbidx = 0;
	unsigned long mark = 1;
	int i;

	for (i=0; i < 32; i++) {
		if (_periph_mask & mark) {
			if (bit == mark) {
				if (callbacks[cbidx] != NULL) {
					callbacks[cbidx] = NULL;
					return true;
				} else {
					return false;
				}
			}
			cbidx++;
			mark *= 2;
		}
	}

	return false;
}

boolean CPUFREQ::requestClock(unsigned long freq, boolean is_hfxt)
{
	boolean ret;

	if (is_hfxt) {
		ret = CPUFREQ::setHFXT(freq);
	} else {
		ret = CPUFREQ::setDCO(freq);
	}

	if (ret) {
		set_mclk(freq);

		uint16_t wdt_div = 8192;
		set_wdt_setting(WDT_MDLY_8);
		if (freq < 8000000UL) {
			wdt_div = 512;
			set_wdt_setting(WDT_MDLY_0_5);
		}
		set_micros_per_wdt(1000000UL / (freq / wdt_div));

		WDTCTL = wdt_setting();

		updatePeripherals();
	}

	return ret;
}

boolean CPUFREQ::exposeSMCLK()
{
	#if !defined(CLKOUT_SMCLK_PORT) || !defined(CLKOUT_SMCLK_PIN)
	return false;
	#endif

	#if defined(P1SEL) && defined(P1SEL2)
		// G2xxx style
		*((uint8_t *)(&CLKOUT_SMCLK_PORT + 0x02)) |= CLKOUT_SMCLK_PIN;  // PxDIR = 1
		*((uint8_t *)(&CLKOUT_SMCLK_PORT + 0x06)) |= CLKOUT_SMCLK_PIN;  // PxSEL = 1

		// Location of PxSEL2 is a bit odd, so it's just easier to make a breakout test
		if (&CLKOUT_SMCLK_PORT == &P1IN)
			P1SEL2 &= ~CLKOUT_SMCLK_PIN;
		if (&CLKOUT_SMCLK_PORT == &P2IN)
			P2SEL2 &= ~CLKOUT_SMCLK_PIN;
		#ifdef __MSP430_HAS_PORT3_R__
		if (&CLKOUT_SMCLK_PORT == &P3IN)
			P3SEL2 &= ~CLKOUT_SMCLK_PIN;
		#endif
		#ifdef __MSP430_HAS_PORT4_R__
		if (&CLKOUT_SMCLK_PORT == &P4IN)
			P4SEL2 &= ~CLKOUT_SMCLK_PIN;
		#endif
	#elif defined(P1SEL) && !defined(P1SEL2)
		// F2xxx style, F5xxx/6xxx style
		*((uint8_t *)(&CLKOUT_SMCLK_PORT + 0x04)) |= CLKOUT_SMCLK_PIN;  // PxDIR = 1
		*((uint8_t *)(&CLKOUT_SMCLK_PORT + 0x0A)) |= CLKOUT_SMCLK_PIN;  // PxSEL = 1
	#elif defined(P1SEL0) && !defined(P1SEL1)
		// FR4xxx/2xxx style
		*((uint8_t *)(&CLKOUT_SMCLK_PORT + 0x04)) |= CLKOUT_SMCLK_PIN;  // PxDIR = 1
		*((uint8_t *)(&CLKOUT_SMCLK_PORT + 0x0A)) |= CLKOUT_SMCLK_PIN;  // PxSEL0 = 1
	#elif defined(P1SEL0) && defined(P1SEL1) && defined(P1SELC)
		// Wolverine, FR57xx style
		*((uint8_t *)(&CLKOUT_SMCLK_PORT + 0x04)) |= CLKOUT_SMCLK_PIN;  // PxDIR = 1
		*((uint8_t *)(&CLKOUT_SMCLK_PORT + 0x0C)) |= CLKOUT_SMCLK_PIN; // PxSEL1 = 1
		*((uint8_t *)(&CLKOUT_SMCLK_PORT + 0x0A)) |= CLKOUT_SMCLK_PIN;  // PxSEL0 = 1
	#endif

	return true;
}

boolean CPUACLK::exposeACLK()
{
	#if !defined(CLKOUT_ACLK_PORT) || !defined(CLKOUT_ACLK_PIN)
	return false;
	#endif

	#if defined(P1SEL) && defined(P1SEL2)
		// G2xxx style
		*((uint8_t *)(&CLKOUT_ACLK_PORT + 0x02)) |= CLKOUT_ACLK_PIN;  // PxDIR = 1
		*((uint8_t *)(&CLKOUT_ACLK_PORT + 0x06)) |= CLKOUT_ACLK_PIN;  // PxSEL = 1

		// Location of PxSEL2 is a bit odd, so it's just easier to make a breakout test
		if (&CLKOUT_ACLK_PORT == &P1IN)
			P1SEL2 &= ~CLKOUT_ACLK_PIN;
		if (&CLKOUT_ACLK_PORT == &P2IN)
			P2SEL2 &= ~CLKOUT_ACLK_PIN;
		#ifdef __MSP430_HAS_PORT3_R__
		if (&CLKOUT_ACLK_PORT == &P3IN)
			P3SEL2 &= ~CLKOUT_ACLK_PIN;
		#endif
		#ifdef __MSP430_HAS_PORT4_R__
		if (&CLKOUT_ACLK_PORT == &P4IN)
			P4SEL2 &= ~CLKOUT_ACLK_PIN;
		#endif
	#elif defined(P1SEL) && !defined(P1SEL2)
		// F2xxx style, F5xxx/6xxx style
		*((uint8_t *)(&CLKOUT_ACLK_PORT + 0x04)) |= CLKOUT_ACLK_PIN;  // PxDIR = 1
		*((uint8_t *)(&CLKOUT_ACLK_PORT + 0x0A)) |= CLKOUT_ACLK_PIN;  // PxSEL = 1
	#elif defined(P1SEL0) && !defined(P1SEL1)
		// FR4xxx/2xxx style
		*((uint8_t *)(&CLKOUT_ACLK_PORT + 0x04)) |= CLKOUT_ACLK_PIN;  // PxDIR = 1
		*((uint8_t *)(&CLKOUT_ACLK_PORT + 0x0A)) |= CLKOUT_ACLK_PIN;  // PxSEL0 = 1
	#elif defined(P1SEL0) && defined(P1SEL1) && defined(P1SELC)
		// Wolverine, FR57xx style
		*((uint8_t *)(&CLKOUT_ACLK_PORT + 0x04)) |= CLKOUT_ACLK_PIN;  // PxDIR = 1
		*((uint8_t *)(&CLKOUT_ACLK_PORT + 0x0C)) |= CLKOUT_ACLK_PIN; // PxSEL1 = 1
		*((uint8_t *)(&CLKOUT_ACLK_PORT + 0x0A)) |= CLKOUT_ACLK_PIN;  // PxSEL0 = 1
	#endif

	return true;
}

unsigned int CPUACLK::getState(void)
{
	unsigned int state = 0x0000;

	// MSP430 F2xxx & G2xxx Value-Line
	#ifdef __MSP430_HAS_BC2__
		if (!(BCSCTL3 & LFXT1OF))
			state |= CPUACLK_STATE_MASK_LFXT_RUNNING;
		if ( (BCSCTL3 & LFXT1S_3) == LFXT1S_2 )
			state |= CPUACLK_STATE_MASK_USING_VLO;
		if ( (BCSCTL2 & SELM_3) == SELM_3 )
			state |= CPUACLK_STATE_MASK_SUPPLYING_MCLK;
		_state_mask = state;
		return state;
	#endif

	// MSP430 F5xxx/6xxx
	#ifdef __MSP430_HAS_UCS__
		if (!(UCSCTL6 & XT1OFF))
			state |= CPUACLK_STATE_MASK_LFXT_RUNNING;
		if ( (UCSCTL4 & SELA_7) == SELA__VLOCLK )
			state |= CPUACLK_STATE_MASK_USING_VLO;
		if ( (UCSCTL4 & SELA_7) == SELA__REFOCLK )
			state |= CPUACLK_STATE_MASK_USING_REFO;
		uint16_t m = UCSCTL4 & SELM_7;
		if (m == SELM__XT1CLK || m == SELM__VLOCLK || m == SELM__REFOCLK)
			state |= CPUACLK_STATE_MASK_SUPPLYING_MCLK;
		_state_mask = state;
		return state;
	#endif

	// MSP430 FR57xx (early FRAM series)
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_FRAM_FR5XX__)
	#endif

	// MSP430 FR58xx/59xx/6xxx series (Wolverine)
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
		uint16_t a = CSCTL2 & SELA_7;

		// Check for LFXT operation
		if ( !(CSCTL4 & LFXTOFF) && !(CSCTL5 & LFXTOFFG) ) {
			state |= CPUACLK_STATE_MASK_LFXT_RUNNING;
		} else {
			if (a == SELA__LFXTCLK) {  // SELA__LFXTCLK with a non-functioning LFXT == alternate way to select VLO or LFMODCLK
				if (CSCTL4 & VLOOFF)
					state |= CPUACLK_STATE_MASK_USING_LFMODCLK;
				else
					state |= CPUACLK_STATE_MASK_USING_VLO;
			}
		}
		if (a == SELA__VLOCLK)
			state |= CPUACLK_STATE_MASK_USING_VLO;
		if (a == SELA__LFMODCLK)
			state |= CPUACLK_STATE_MASK_USING_LFMODCLK;

		uint16_t m = CSCTL2 & SELM_7;
		if (m == SELM__LFXTCLK || m == SELM__VLOCLK || m == SELM__LFMODCLK)
			state |= CPUACLK_STATE_MASK_SUPPLYING_MCLK;
		_state_mask = state;
		return state;
	#endif

	// MSP430 FR4xxx/2xxx "Value Line" FRAM chips
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && !defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
	#endif

	// MSP430i series
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_MSP430I_CPU__)
	#endif
}

boolean CPUACLK::startLFXT(unsigned long freq)
{
	// MSP430 F2xxx & G2xxx Value-Line
	#ifdef __MSP430_HAS_BC2__
		#ifdef CPUFREQ_OVERRIDE_XCAP
		uint8_t bc3 = CPUFREQ_OVERRIDE_XCAP | LFXT1S_0;
		#else
		uint8_t bc3 = XCAP_3 | LFXT1S_0;
		#endif

		#if BCS2_LFXT_PORT != 0
		*((uint8_t *)((uint8_t *)&BCS2_LFXT_PORT + 0x0A)) |= BCS2_LFXT_XIN | BCS2_LFXT_XOUT;  // PxSEL enable
		// Location of PxSEL2 is a bit odd, so it's just easier to make a breakout test
		if (&BCS2_LFXT_PORT == &P1IN)
			P1SEL2 &= ~(BCS2_LFXT_XIN | BCS2_LFXT_XOUT);
		if (&BCS2_LFXT_PORT == &P2IN)
			P2SEL2 &= ~(BCS2_LFXT_XIN | BCS2_LFXT_XOUT);
		#ifdef __MSP430_HAS_PORT3_R__
		if (&BCS2_LFXT_PORT == &P3IN)
			P3SEL2 &= ~(BCS2_LFXT_XIN | BCS2_LFXT_XOUT);
		#endif
		#ifdef __MSP430_HAS_PORT4_R__
		if (&BCS2_LFXT_PORT == &P4IN)
			P4SEL2 &= ~(BCS2_LFXT_XIN | BCS2_LFXT_XOUT);
		#endif
		#endif /* if BCS2_LFXT_PORT != 0 */

		BCSCTL3 = (BCSCTL3 & ~(XCAP_3 | LFXT1S_3)) | bc3;  // Preserve XT2 settings

		// Wait for LFXT1 to start-
		uint16_t count = 2000;
		do {
			BCSCTL3 &= ~LFXT1OF;
			__delay_cycles(8000);
			count--;
		} while (count && (BCSCTL3 & LFXT1OF));
		if (!count)
			return false;  // Failed to start LFXT1
		return true;
	#endif

	// MSP430 F5xxx/6xxx
	#ifdef __MSP430_HAS_UCS__
		#ifdef CPUFREQ_OVERRIDE_XCAP
		uint16_t xcap = CPUFREQ_OVERRIDE_XCAP;
		#else
		uint16_t xcap = XCAP_3;
		#endif

		#if UCS_LFXT_PORT != 0
		*((uint8_t *)((uint8_t *)&UCS_LFXT_PORT + 0x0A)) |= UCS_LFXT_XIN | UCS_LFXT_XOUT;  // PxSEL enable
		#endif
		UCSCTL6 = (UCSCTL6 & ~(XCAP_3 | XT1DRIVE_3 | XTS | XT1BYPASS | XT1OFF)) | xcap | XT1DRIVE_3;
		uint16_t count = 4000;
		do {
			UCSCTL7 &= ~XT1LFOFFG;
			SFRIFG1 &= ~OFIFG;
			__delay_cycles(8000);
			count--;
		} while (count && (UCSCTL7 & XT1LFOFFG));
		if (!count) {
			return false;  // Timed out starting XT1
		}

		// Switch FLL reference to XT1CLK, ACLK to XT1CLK
		if (!(__get_SR_register() & SCG0)) {
			__bis_SR_register(SCG0);  // Disable FLL, switch REF to REFO, re-enable FLL
			UCSCTL3 = (UCSCTL3 & ~(SELREF_7)) | SELREF__XT1CLK;
			__bic_SR_register(SCG0);
		}

		UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | SELA__XT1CLK;
		set_aclk(freq);
		set_wdt_setting(WDT_ADLY_16);
		set_micros_per_wdt(1000000UL / (freq / 512));
		return true;
	#endif

	// MSP430 FR57xx (early FRAM series)
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_FRAM_FR5XX__)
	#endif

	// MSP430 FR58xx/59xx/6xxx series (Wolverine)
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
		// Activate LFXT function on port pins
		#ifdef CS_LFXT_PORT
		*((uint8_t *)((uint8_t *)&CS_LFXT_PORT + 0x0A)) |= CS_LFXT_XIN | CS_LFXT_XOUT;     // PxSEL0 enable
		*((uint8_t *)((uint8_t *)&CS_LFXT_PORT + 0x0C)) &= ~(CS_LFXT_XIN | CS_LFXT_XOUT);  // PxSEL1 disable
		#endif
		CSCTL0_H = CSKEY_H;

		CSCTL4 = (CSCTL4 & 0xFF00) | LFXTDRIVE_3;  // Clears LFXTBYPASS | LFXTOFF

		uint16_t count = 4000;
		do {
			CSCTL5 &= ~LFXTOFFG;
			SFRIFG1 &= ~OFIFG;
			__delay_cycles(8000);
			count--;
		} while (count && (CSCTL5 & LFXTOFFG));
		if (!count) {
			CSCTL0_H = 0;
			return false;  // Timed out starting LFXT
		}

		// Switch ACLK to LFXT
		CSCTL2 = (CSCTL2 & (SELS_7 | SELM_7)) | SELA__LFXTCLK;

		CSCTL0_H = 0;

		set_aclk(freq);
		set_wdt_setting(WDT_ADLY_16);
		set_micros_per_wdt(1000000UL / (freq / 512));
		return true;
	#endif

	// MSP430 FR4xxx/2xxx "Value Line" FRAM chips
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && !defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
	#endif

	// MSP430i series
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_MSP430I_CPU__)
	return false;  // ACLK is a fixed-frequency internal osc
	#endif

	return false;
}

boolean CPUACLK::stopLFXT()
{
	// MSP430 F2xxx & G2xxx Value-Line
	#ifdef __MSP430_HAS_BC2__
		// Forcing to VLOCLK does the same thing
		return forceVLO();
	#endif

	// MSP430 F5xxx/6xxx
	#ifdef __MSP430_HAS_UCS__
		// First: Find new source for FLL or MCLK, if either are using XT1
		getState();
		if (!(__get_SR_register() & SCG0)) {  // FLL active?
			if ( (UCSCTL3 & SELREF_7) == SELREF__XT1CLK ) {
				__bis_SR_register(SCG0);  // Disable FLL, switch REF to REFO, re-enable FLL
				UCSCTL3 = (UCSCTL3 & ~(SELREF_7)) | SELREF__REFOCLK;
				__bic_SR_register(SCG0);
			}
		}
		if (state_mask() & CPUACLK_STATE_MASK_SUPPLYING_MCLK) {
			UCSCTL3 = (UCSCTL3 & ~(SELM_7 | SELS_7)) | SELM__REFOCLK | SELS__REFOCLK;
		}
		if ( (state_mask() & (CPUACLK_STATE_MASK_USING_VLO | CPUACLK_STATE_MASK_USING_REFO)) == 0 ) {
			// XT1CLK sourcing ACLK too!  Switch to REFOCLK.
			UCSCTL3 = (UCSCTL3 & ~SELA_7) | SELA__REFOCLK;
		}
		return true;
	#endif

	// MSP430 FR57xx (early FRAM series)
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_FRAM_FR5XX__)
	#endif

	// MSP430 FR58xx/59xx/6xxx series (Wolverine)
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
		CSCTL0_H = CSKEY_H;
		CSCTL4 |= LFXTOFF;  // Force LFXT off, then force switching to VLO
		CSCTL0_H = 0;
		return forceVLO();
	#endif

	// MSP430 FR4xxx/2xxx "Value Line" FRAM chips
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && !defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
	#endif

	// MSP430i series
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_MSP430I_CPU__)
	return false;  // ACLK is a fixed-frequency internal osc
	#endif

	return false;
}

boolean CPUACLK::forceVLO()
{
	// MSP430 F2xxx & G2xxx Value-Line
	#ifdef __MSP430_HAS_BC2__
		BCSCTL3 = (BCSCTL3 & ~(LFXT1S_3)) | LFXT1S_2;
		set_aclk(CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY);
		set_wdt_setting(WDT_ADLY_16);
		set_micros_per_wdt(1000000UL / (CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY / 512));
		return true;
	#endif

	// MSP430 F5xxx/6xxx
	#ifdef __MSP430_HAS_UCS__
		UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | SELA__VLOCLK;
		set_aclk(CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY);
		set_wdt_setting(WDT_ADLY_16);
		set_micros_per_wdt(1000000UL / (CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY / 512));

		return true;
	#endif

	// MSP430 FR57xx (early FRAM series)
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_FRAM_FR5XX__)
	#endif

	// MSP430 FR58xx/59xx/6xxx series (Wolverine)
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
		CSCTL0_H = CSKEY_H;
		CSCTL2 = (CSCTL2 & (SELS_7 | SELM_7)) | SELA__VLOCLK;
		CSCTL0_H = 0;

		set_aclk(CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY);
		set_wdt_setting(WDT_ADLY_16);
		set_micros_per_wdt(1000000UL / (CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY / 512));

		return true;
	#endif

	// MSP430 FR4xxx/2xxx "Value Line" FRAM chips
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && !defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
	#endif

	// MSP430i series
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_MSP430I_CPU__)
	return false;  // MSP430i has no VLO
	#endif

	return false;
}

boolean CPUACLK::forceREFO()
{
	// MSP430 F2xxx & G2xxx Value-Line
	#ifdef __MSP430_HAS_BC2__
	return false;  // REFOCLK not present
	#endif

	// MSP430 F5xxx/6xxx
	#ifdef __MSP430_HAS_UCS__
		UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | SELA__REFOCLK;
		set_aclk(32768);
		set_wdt_setting(WDT_ADLY_16);
		set_micros_per_wdt(1000000UL / (32768 / 512));

		return true;
	#endif
	//
	// MSP430 FR57xx (early FRAM series)
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_FRAM_FR5XX__)
	return false;  // REFOCLK not present
	#endif

	// MSP430 FR58xx/59xx/6xxx series (Wolverine)
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
	return false;  // REFOCLK not present
	#endif

	// MSP430 FR4xxx/2xxx "Value Line" FRAM chips
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && !defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
	#endif

	// MSP430i series
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_MSP430I_CPU__)
	return false;  // MSP430i has no REFO
	#endif

	return false;
}

boolean CPUACLK::forceLFMODCLK()
{
	// MSP430 F2xxx & G2xxx Value-Line
	#ifdef __MSP430_HAS_BC2__
	return false;  // LFMODCLK not present
	#endif

	// MSP430 F5xxx/6xxx
	#ifdef __MSP430_HAS_UCS__
	return false;  // LFMODCLK not present
	#endif
	//
	// MSP430 FR57xx (early FRAM series)
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_FRAM_FR5XX__)
	return false;  // LFMODCLK not present
	#endif

	// MSP430 FR58xx/59xx/6xxx series (Wolverine)
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
		CSCTL0_H = CSKEY_H;
		CSCTL2 = (CSCTL2 & (SELS_7 | SELM_7)) | SELA__LFMODCLK;
		CSCTL0_H = 0;

		set_aclk(CPUFREQ_CHIP_SPECIFIC_MODOSC_NOMINAL_FREQUENCY / 128);  // LFMODCLK = MODOSC / 128, typically ~37KHz or so
		set_wdt_setting(WDT_ADLY_16);
		set_micros_per_wdt(1000000UL / (_aclk / 512));

		return true;
	#endif

	// MSP430 FR4xxx/2xxx "Value Line" FRAM chips
	#if defined(__MSP430_HAS_CS__) && !defined(__MSP430_HAS_FRAM_FR5XX__) && defined(__MSP430_HAS_PMM_FRAM__) && !defined(__MSP430_HAS_CAP_TOUCH_IO_1__)
	return false;  // LFMODCLK not present
	#endif

	// MSP430i series
	#if defined(__MSP430_HAS_CS__) && defined(__MSP430_HAS_MSP430I_CPU__)
	return false;  // LFMODCLK not present
	#endif

	return false;
}
