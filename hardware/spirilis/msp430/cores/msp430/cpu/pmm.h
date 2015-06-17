/* Energia-on-Arduino 1.6+ PMM Core Voltage support
 *
 * 2015 Eric Brundick <spirilis at linux dot com>
 */

#ifndef PMM_H
#define PMM_H

#include <msp430.h>
#include <stdint.h>

#ifdef __MSP430_HAS_PMM__

extern unsigned int pmm_core_voltage;

unsigned int pmm_get_core_voltage(void);
unsigned int pmm_set_core_voltage(unsigned int corevidx);
unsigned int pmm_compute_core_voltage(unsigned long freq);

#endif /* MSP430_HAS_PMM */

#endif /* PMM_H */
