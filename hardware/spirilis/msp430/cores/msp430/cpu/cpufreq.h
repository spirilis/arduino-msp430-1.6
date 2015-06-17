/* Energia-on-Arduino 1.6+ System Clock Request
 *
 * 2015 Eric Brundick <spirilis at linux dot com>
 */

#ifndef CPUFREQ_H
#define CPUFREQ_h

#include <Energia.h>
#include <pins_booster.h>

typedef void(*CPUFREQ_PERIPHERAL_CALLBACK)(uint32_t);  // Peripheral frequency change callback - takes uint32_t MCLK parameter

#ifndef CPUFREQ_CHIP_SPECIFIC_PERIPH_COUNT
#error Variants headers need to include CPUFREQ_CHIP_SPECIFIC_PERIPH_COUNT
#endif
#ifndef CPUFREQ_CHIP_SPECIFIC_PERIPH_MASK
#error Variants headers need to include CPUFREQ_CHIP_SPECIFIC_PERIPH_MASK
#endif
#ifndef CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY
#error Variants headers need to include CPUFREQ_CHIP_SPECIFIC_VLO_NOMINAL_FREQUENCY
#endif
#if defined(SELM__LFMODCLK) && !defined(CPUFREQ_CHIP_SPECIFIC_MODOSC_NOMINAL_FREQUENCY)
#error Variants headers need to include CPUFREQ_CHIP_SPECIFIC_MODOSC_NOMINAL_FREQUENCY
#endif

class CPUFREQ {
	public:
		CPUFREQ();
		boolean requestClock(unsigned long freq, boolean is_hfxt = false);
		boolean requestSleepWalk(void);  // Request MCLK=ACLK
		static boolean setDCO(unsigned long freq);
		static boolean setHFXT(unsigned long freq);

		boolean enablePeripheral(uint32_t bit, CPUFREQ_PERIPHERAL_CALLBACK callback);
		boolean disablePeripheral(uint32_t bit);

		boolean exposeSMCLK(void);

		inline void set_mclk(unsigned long f) { _mclk = f; };
		inline uint32_t mclk() { return _mclk; };
		inline void set_micros_per_wdt(unsigned long m) { _micros_per_wdt = m; };
		inline uint32_t micros_per_wdt() { return _micros_per_wdt; };
		inline void set_periph_mask(unsigned long m) { _periph_mask = m; };
		inline uint32_t periph_mask() { return _periph_mask; };
		inline void set_periph_active(unsigned long a) { _periph_active = a; };
		inline uint32_t periph_active() { return _periph_active; };
		inline void set_wdt_setting(uint16_t s) { _wdt_setting = s; };
		inline uint16_t wdt_setting() { return _wdt_setting; };

	private:
		uint32_t _mclk;
		uint32_t _micros_per_wdt;
		uint32_t _periph_mask;
		uint32_t _periph_active;
		CPUFREQ_PERIPHERAL_CALLBACK callbacks[CPUFREQ_CHIP_SPECIFIC_PERIPH_COUNT];
		void updatePeripherals(void);
		uint16_t _wdt_setting;
};

#define CPUACLK_STATE_MASK_LFXT_RUNNING		BIT0
#define CPUACLK_STATE_MASK_USING_VLO		BIT1
#define CPUACLK_STATE_MASK_USING_REFO		BIT2
#define CPUACLK_STATE_MASK_USING_LFMODCLK	BIT3  // FRAM chips often have this option in lieu of REFO
#define CPUACLK_STATE_MASK_SUPPLYING_MCLK	BIT4
#define CPUACLK_STATE_MASK_ACLK_FIXED		BIT5  // MSP430i series is like this

class CPUACLK {
	public:
		CPUACLK();
		boolean startLFXT(unsigned long freq);
		boolean stopLFXT();
		boolean forceVLO();
		boolean forceREFO();
		boolean forceLFMODCLK();  // FRAM chips have this
		unsigned int getState();  // Update state_mask and return (to check for LFXT errors)

		boolean exposeACLK();

		inline void set_aclk(unsigned long a) { _aclk = a; };
		inline uint32_t aclk() { return _aclk; };
		inline void set_micros_per_wdt(unsigned long m) { _micros_per_wdt = m; };
		inline uint32_t micros_per_wdt() { return _micros_per_wdt; };
		inline unsigned int state_mask() { return _state_mask; };
		inline void set_wdt_setting(uint16_t s) { _wdt_setting = s; };
		inline uint16_t wdt_setting() { return _wdt_setting; };
	private:
		uint32_t _aclk;
		uint32_t _micros_per_wdt;
		unsigned int _state_mask;
		uint16_t _wdt_setting;
};

extern CPUFREQ cpu;
extern CPUACLK aclk;

#endif /* CPUFREQ_H */
