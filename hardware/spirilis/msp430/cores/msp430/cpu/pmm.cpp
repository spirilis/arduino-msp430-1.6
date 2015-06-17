/* Energia-on-Arduino 1.6+ PMM Core Voltage support
 *
 * 2015 Eric Brundick <spirilis at linux dot com>
 */

#include "pmm.h"

#ifdef __MSP430_HAS_PMM__

unsigned int pmm_core_voltage = 0;

unsigned int pmm_get_core_voltage()
{
	pmm_core_voltage = PMMCTL0 & 0x03;
	return pmm_core_voltage;
}

unsigned int pmm_set_core_voltage(unsigned int corevidx)
{
	if (corevidx > 0x03)
		return 0xFFFF;  // Invalid

	pmm_get_core_voltage();
	if (corevidx == pmm_core_voltage)
		return pmm_core_voltage;  // All done!

	unsigned int cur_lvl = pmm_core_voltage;

	PMMCTL0_H = 0xA5;  // Unlock
	if (corevidx < pmm_core_voltage) {
		// Orderly step-down
		while (cur_lvl > corevidx) {
			cur_lvl--;

			// Set SVS/SVM high side new level
			SVSMHCTL = SVSHE + SVSHRVL0 * cur_lvl + SVMHE + SVSMHRRL0 * cur_lvl;

			// Set SVM low side to new level
			SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * cur_lvl;

			// Wait till SVM is settled
			while ((PMMIFG & SVSMLDLYIFG) == 0)
				;

			// Clear already set flags
			PMMIFG &= ~(SVMLVLRIFG + SVMLIFG)
				;

			// Set VCore to new level
			PMMCTL0_L = PMMCOREV0 * cur_lvl;

			// Wait till new level reached
			if ((PMMIFG & SVMLIFG)) {
				while ((PMMIFG & SVMLVLRIFG) == 0)
					;
			}

			// Set SVS/SVM low side to new level
			SVSMLCTL = SVSLE + SVSLRVL0 * cur_lvl + SVMLE + SVSMLRRL0 * cur_lvl;
		}
	} else {
		// Orderly step-up
		while (cur_lvl < corevidx) {
			cur_lvl++;

			// Set SVS/SVM high side new level
			SVSMHCTL = SVSHE + SVSHRVL0 * cur_lvl + SVMHE + SVSMHRRL0 * cur_lvl;

			// Set SVM low side to new level
			SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * cur_lvl;

			// Wait till SVM is settled
			while ((PMMIFG & SVSMLDLYIFG) == 0)
				;

			// Clear already set flags
			PMMIFG &= ~(SVMLVLRIFG + SVMLIFG)
				;

			// Set VCore to new level
			PMMCTL0_L = PMMCOREV0 * cur_lvl;

			// Wait till new level reached
			if ((PMMIFG & SVMLIFG)) {
				while ((PMMIFG & SVMLVLRIFG) == 0)
					;
			}

			// Set SVS/SVM low side to new level
			SVSMLCTL = SVSLE + SVSLRVL0 * cur_lvl + SVMLE + SVSMLRRL0 * cur_lvl;
		}
	}
	PMMCTL0_H = 0x00;  // Lock PMM registers
	return cur_lvl;
}

unsigned int pmm_compute_core_voltage(unsigned long freq)
{
	if (freq <= 8000000)
		return 0;
	if (freq <= 12000000)
		return 1;
	if (freq <= 20000000)
		return 2;
	return 3;
}


#endif
