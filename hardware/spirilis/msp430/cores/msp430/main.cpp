/* main.cpp - Starting point for Energia-on-Arduino 1.6+ Execution */

#include <Energia.h>

extern "C" {

void _init()
{
	WDTCTL = WDTPW | WDTHOLD;
	_DINT();
	// Wolverine - release LPMx.5 port freeze
	#ifdef LOCKLPM5
	// This part is required for FR59xx device to unlock the IOs
	PMMCTL0_H = PMMPW_H;           // open PMM
	PM5CTL0 &= ~LOCKLPM5;          // clear lock bit for ports
	PMMCTL0_H = 0;                 // lock PMM
	#endif

	// Clock init
	cpu.set_periph_mask(CPUFREQ_CHIP_SPECIFIC_PERIPH_MASK);
	cpu.set_periph_active(0x00000000);
	#ifdef F_CPU_USE_HFXT
	if (!CPUFREQ::setHFXT(F_CPU_DEFAULT)) {
		// Fallback in case HFXT doesn't exist or isn't successfully starting
		CPUFREQ::setDCO(1000000UL);
		cpu.set_mclk(1000000UL);
	} else {
		cpu.set_mclk(F_CPU_DEFAULT);
	}
	#else
	if (CPUFREQ::setDCO(F_CPU_DEFAULT)) {
		cpu.set_mclk(F_CPU_DEFAULT);
	} else {
		// Fallback in case F_CPU_DEFAULT was defined to an invalid frequency for this chip
		CPUFREQ::setDCO(1000000UL);
		cpu.set_mclk(1000000UL);
	}
	#endif

	// Init cpu object instance parameters and prepare for WDT interval-mode
	uint16_t wdt_div = 8192;
	cpu.set_wdt_setting(WDT_MDLY_8);
	if (F_CPU < 8000000UL) {
		wdt_div = 512;
		cpu.set_wdt_setting(WDT_MDLY_0_5);
	}
	cpu.set_micros_per_wdt(1000000UL / (cpu.mclk() / wdt_div));

	// Init default settings for aclk object instance
	aclk.getState();
	if ( (aclk.state_mask() & (CPUACLK_STATE_MASK_LFXT_RUNNING |
				   CPUACLK_STATE_MASK_USING_VLO |
				   CPUACLK_STATE_MASK_USING_REFO |
				   CPUACLK_STATE_MASK_ACLK_FIXED |
				   CPUACLK_STATE_MASK_USING_LFMODCLK)) == 0 ) {
		// ACLK in a bad state; ACLK=XT1 yet XT1 isn't running...
		if (!aclk.forceREFO()) {  // This will fail quickly and quietly on any device that doesn't include the REFOCLK
			aclk.forceVLO();  // ...and fall back to VLOCLK
		}
		aclk.getState();
	} else {
		if (aclk.state_mask() & CPUACLK_STATE_MASK_USING_VLO) {
			aclk.set_aclk(CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY);
		} else if ( aclk.state_mask() & (CPUACLK_STATE_MASK_USING_REFO |
						 CPUACLK_STATE_MASK_LFXT_RUNNING |
						 CPUACLK_STATE_MASK_ACLK_FIXED) /* MSP430i */ ) {
			aclk.set_aclk(32768);
		} else if ( aclk.state_mask() & CPUACLK_STATE_MASK_USING_LFMODCLK ) {  // FRAM chips only
			#ifdef CPUFREQ_CHIP_SPECIFIC_MODOSC_NOMINAL_FREQUENCY
			aclk.set_aclk(CPUFREQ_CHIP_SPECIFIC_MODOSC_NOMINAL_FREQUENCY / 128);
			#endif
		}
		aclk.set_micros_per_wdt(1000000UL / (aclk.aclk() / 512));
		aclk.set_wdt_setting(WDT_ADLY_16);  // WDT /512
	}
	_sys_millis = 0UL;
	_sys_micros = 0;

	// Initialize WDT and enable interrupts so it can begin the millis/micros counter
	#ifdef __MSP430_HAS_SFR__
	SFRIFG1 &= ~WDTIFG;
	SFRIE1 |= WDTIE;
	#else
	IFG1 &= ~WDTIFG;
	IE1 |= WDTIE;
	#endif
	WDTCTL = cpu.wdt_setting();
	_EINT();  // Timer now active!

	// For testing clocking system
	//cpu.exposeSMCLK();
	//aclk.exposeACLK();

	// Indicate using P1.0 that the system has been reset
	
	P1DIR |= BIT0;
	P1OUT |= BIT0;
	delay(125);
	P1OUT &= ~BIT0;
	delay(80);
	P1OUT |= BIT0;
	delay(125);
	P1OUT &= ~BIT0;
	P1DIR &= ~BIT0;

}

/* Using .crt_0400* for the section name stuffs this in between crt0_movedata (.crt_0300*) and crt0_call_init_then_main (.crt_0700*),
 * which runs the C++ constructors.
 */
__attribute__((section(".crt_0400call_arduino_init"))) __attribute__((naked))
void _run_arduino_init()
{
	_init();
}

int main()
{
	/* System runs _init() first to start the clocks, then global C++
	 * constructors, then main().
	 */

	// Off to the races!
	setup();
	while (1) {
		loop();

		#ifndef ENERGIA_SERIALEVENTRUN_DISABLE
		serialEventRun();
		#endif
	}
	return 0;  // never reaches here
}

}; /* extern "C" */
