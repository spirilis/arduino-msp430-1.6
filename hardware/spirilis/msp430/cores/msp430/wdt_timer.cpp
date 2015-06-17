/* Energia-on-Arduino 1.6+ - WDT Interval Timer and all delay/sleep-related functions */

#include <Energia.h>

volatile uint32_t _sys_millis;
volatile uint16_t _sys_micros;
volatile boolean _sys_sleeping, _sys_stay_asleep;

__attribute__((interrupt(WDT_VECTOR)))
void watchdog_isr (void)
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	register uint32_t m = _sys_millis;
	register uint16_t f = _sys_micros;

	if (_sys_sleeping) {
		m += aclk.micros_per_wdt() / 1000;
		f += aclk.micros_per_wdt() % 1000;
	} else {
		m += cpu.micros_per_wdt() / 1000;
		f += cpu.micros_per_wdt() % 1000;
	}

	if (f >= 1000) {
		f -= 1000;
		m++;
	}

	_sys_millis = m;
	_sys_micros = f;

        /* Exit from LPM on reti */
        __bic_SR_register_on_exit(LPM4_bits);
}

void delay(uint32_t ms)
{
	register uint32_t start = millis();
	_sys_stay_asleep = true;

	while ( _sys_stay_asleep && (millis() - start) < ms ) {
		__bis_SR_register(LPM0_bits | GIE);
	}
	_sys_stay_asleep = false;
}

/* Energia uses a per-F_CPU specific set of rules, but we can't really do that here... so I am
 * just taking a stab at using the 1MHz rules
 */
void delayMicroseconds(uint32_t us)
{
	if (us-- == 0)
		return;
	if (us-- == 0)
		return;
	us >>= 2;

	if (F_CPU >= 1000000UL) {
		while (us) {
			__delay_cycles(16);
			us--;
		}
	} else {
		while (us) {
			us--;
		}
	}
}

/* These are relatively easy to implement with the CPUFREQ/CPUACLK classes */

void sleep(uint32_t ms)
{
	_sys_sleeping = true;
	_sys_stay_asleep = true;
	WDTCTL = aclk.wdt_setting();

	register uint32_t start = millis();
	while (_sys_stay_asleep && ((millis() - start) < ms) ) {
		__bis_SR_register(LPM3_bits | GIE);
	}

	_sys_sleeping = false;
	_sys_stay_asleep = false;

	WDTCTL = cpu.wdt_setting();
}

// Cheating this for now
void sleepSeconds(uint32_t sec)
{
	return sleep(sec * 1000);
}

void suspend()
{
	_sys_sleeping = true;
	_sys_stay_asleep = true;
	WDTCTL = WDTPW | WDTHOLD;

	while (_sys_stay_asleep) {
		__bis_SR_register(LPM4_bits | GIE);
	}

	_sys_sleeping = false;
	_sys_stay_asleep = false;

	WDTCTL = cpu.wdt_setting();
}
