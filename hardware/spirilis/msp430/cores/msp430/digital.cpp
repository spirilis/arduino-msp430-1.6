/* Energia-on-Arduino 1.6+ Digital I/O API
 *
 * This implements the standard Wiring/Arduino digitalWrite, digitalRead, pinMode API
 */

#include <Energia.h>

#ifdef __cplusplus
extern "C" {
#endif


// used by _resolve_index_from_portbit in digital.h
const uint8_t _BV_ARY8[] = { BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7 };
#include "digital.h"


void pinMode(uint16_t pin, uint16_t mode)
{
	uint16_t realpin = _resolve_pin(pin);
	if (!realpin)
		return;  // Not a real pin; abort

	uint8_t portbit = chip_pins[realpin].portbit;
	if (!portbit)
		return;  // this would be an error <CORE_ERROR>

	/* TODO: Check if this is a Timer pin, and if its Timer function is active.
	 * If so, run a timer-cleanup function which may possibly shut off the timer in question if
	 * no more CCRs are active on that timer.
	 */

	// Port Mapper ID can be included in the MSB of 'mode'
	#ifdef __MSP430_HAS_PORT_MAPPING__
	if (mode & 0xFF00) {
		volatile uint8_t *pxmap = SFRPTR_BYTE chip_ports[chip_pins[realpin].port].portmap;
		if (pxmap != NULL) {
			unsigned int i = _resolve_index_from_portbit(portbit);

			uint16_t _m = mode >> 8;
			//asm volatile("swpb %[m]" : [m] "=r" (_m) : "r" (_m));

			uint16_t gie_save = __get_SR_register() & GIE;
			_DINT();
			PMAPKEYID = PMAPKEY;  // Unlock PMAP with interrupts disabled
			PMAPCTL |= PMAPRECFG; // Configure to allow reconfiguration of PMAP again later
			*(pxmap+i) = (uint8_t) _m;
			PMAPKEYID = 0;  // Lock PMAP after writing
			__bis_SR_register(gie_save);  // Re-enable interrupts (if applicable)

			mode |= PORT_SELECTION0;  // Port Mapper function obtained with PxSEL0=1, PxSEL1=0
			#ifdef CHIP_HAS_PORT_SELECTION1
			mode &= ~PORT_SELECTION1;
			#endif
		}
	}
	#endif

	volatile uint8_t *pxsel = SFRPTR_BYTE chip_ports[chip_pins[realpin].port].portsel;
	if (pxsel != NULL) {
		if (mode & PORT_SELECTION0)
			*pxsel |= portbit;
		else
			*pxsel &= ~portbit;
	}
	#ifdef CHIP_HAS_PORT_SELECTION1
	volatile uint8_t *pxsel2 = SFRPTR_BYTE chip_ports[chip_pins[realpin].port].portsel2;
	if (pxsel2 != NULL) {
		if (mode & PORT_SELECTION1)
			*pxsel2 |= portbit;
		else
			*pxsel2 &= ~portbit;
	}
	#endif


	volatile uint8_t *pxdir = SFRPTR_BYTE chip_ports[chip_pins[realpin].port].portdir;
	volatile uint8_t *pxren = SFRPTR_BYTE chip_ports[chip_pins[realpin].port].portren;
	volatile uint8_t *pxout = SFRPTR_BYTE chip_ports[chip_pins[realpin].port].portout;

	switch (mode) {
		case INPUT:
			*pxdir &= ~portbit;
			*pxren &= ~portbit;
			break;

		case OUTPUT:
			*pxdir |= portbit;
			*pxren &= ~portbit;
			break;

		case INPUT_PULLUP:
			*pxdir &= ~portbit;
			*pxout |= portbit;
			*pxren |= portbit;
			break;

		case INPUT_PULLDOWN:
			*pxdir &= ~portbit;
			*pxout &= ~portbit;
			*pxren |= portbit;
			break;

	}
}

int digitalRead(uint16_t pin)
{
	uint16_t realpin = _resolve_pin(pin);
	if (!realpin)
		return LOW;  // Not a real pin; abort

	uint8_t portbit = chip_pins[realpin].portbit;

	volatile uint8_t *pxin = SFRPTR_BYTE chip_ports[chip_pins[realpin].port].portin;
	if (*pxin & portbit)
		return HIGH;
	return LOW;
}

void digitalWrite(uint16_t pin, uint8_t val)
{
	uint16_t realpin = _resolve_pin(pin);
	if (!realpin)
		return;  // Not a real pin; abort

	uint8_t portbit = chip_pins[realpin].portbit;

	volatile uint8_t *pxout = SFRPTR_BYTE chip_ports[chip_pins[realpin].port].portout;
	if (val == HIGH)
		*pxout |= portbit;
	else
		*pxout &= ~portbit;
}


/* GPIO Interrupts */

typedef void(*ISR_CALLBACK_PTR)(void);

#if defined(__MSP430_HAS_PORT1_R__) && (defined(P1IE) || defined(P1IE_))
static volatile ISR_CALLBACK_PTR _isr_callbacks_P1[8];
static volatile uint8_t _isr_change_vect_P1 = 0x00;

__attribute__((interrupt(PORT1_VECTOR)))
void P1_ISR(void)
{
	int i;
	uint8_t portbit;
	boolean still_sleeping = _sys_stay_asleep;

	for (i=0; i < 8; i++) {
		portbit = _BV_ARY8[i];

		if ( (P1IFG & portbit) && _isr_callbacks_P1[i] ) {
			_isr_callbacks_P1[i]();
			if (_isr_change_vect_P1 & portbit) {
				P1IES ^= portbit;
				P1IFG = (   (P1IFG & portbit) == (P1IES & portbit) ?  // Check that we haven't already re-triggered before clearing PxIFG
					    (P1IFG & ~portbit) : P1IFG   );
			} else {
				P1IFG &= ~portbit;
			}
		}
	}

	if (still_sleeping != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif

#if defined(__MSP430_HAS_PORT2_R__) && (defined(P2IE) || defined(P2IE_))
static volatile ISR_CALLBACK_PTR _isr_callbacks_P2[8];
static volatile uint8_t _isr_change_vect_P2 = 0x00;

__attribute__((interrupt(PORT2_VECTOR)))
void P2_ISR(void)
{
	int i;
	uint8_t portbit;
	boolean still_sleeping = _sys_stay_asleep;

	for (i=0; i < 8; i++) {
		portbit = _BV_ARY8[i];

		if ( (P2IFG & portbit) && _isr_callbacks_P2[i] ) {
			_isr_callbacks_P2[i]();
			if (_isr_change_vect_P2 & portbit) {
				P2IES ^= portbit;
				P2IFG = (   (P2IFG & portbit) == (P2IES & portbit) ?  // Check that we haven't already re-triggered before clearing PxIFG
					    (P2IFG & ~portbit) : P2IFG   );
			} else {
				P2IFG &= ~portbit;
			}
		}
	}

	if (still_sleeping != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif

#if defined(__MSP430_HAS_PORT3_R__) && (defined(P3IE) || defined(P3IE_))
static volatile ISR_CALLBACK_PTR _isr_callbacks_P3[8];
static volatile uint8_t _isr_change_vect_P3 = 0x00;

__attribute__((interrupt(PORT3_VECTOR)))
void P3_ISR(void)
{
	int i;
	uint8_t portbit;
	boolean still_sleeping = _sys_stay_asleep;

	for (i=0; i < 8; i++) {
		portbit = _BV_ARY8[i];

		if ( (P3IFG & portbit) && _isr_callbacks_P3[i] ) {
			_isr_callbacks_P3[i]();
			if (_isr_change_vect_P3 & portbit) {
				P3IES ^= portbit;
				P3IFG = (   (P3IFG & portbit) == (P3IES & portbit) ?  // Check that we haven't already re-triggered before clearing PxIFG
					    (P3IFG & ~portbit) : P3IFG   );
			} else {
				P3IFG &= ~portbit;
			}
		}
	}

	if (still_sleeping != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif

#if defined(__MSP430_HAS_PORT4_R__) && (defined(P4IE) || defined(P4IE_))
static volatile ISR_CALLBACK_PTR _isr_callbacks_P4[8];
static volatile uint8_t _isr_change_vect_P4 = 0x00;

__attribute__((interrupt(PORT4_VECTOR)))
void P4_ISR(void)
{
	int i;
	uint8_t portbit;
	boolean still_sleeping = _sys_stay_asleep;

	for (i=0; i < 8; i++) {
		portbit = _BV_ARY8[i];

		if ( (P4IFG & portbit) && _isr_callbacks_P4[i] ) {
			_isr_callbacks_P4[i]();
			if (_isr_change_vect_P4 & portbit) {
				P4IES ^= portbit;
				P4IFG = (   (P4IFG & portbit) == (P4IES & portbit) ?  // Check that we haven't already re-triggered before clearing PxIFG
					    (P4IFG & ~portbit) : P4IFG   );
			} else {
				P4IFG &= ~portbit;
			}
		}
	}

	if (still_sleeping != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif

#if defined(__MSP430_HAS_PORT5_R__) && (defined(P5IE) || defined(P5IE_))
static volatile ISR_CALLBACK_PTR _isr_callbacks_P5[8];
static volatile uint8_t _isr_change_vect_P5 = 0x00;

__attribute__((interrupt(PORT5_VECTOR)))
void P5_ISR(void)
{
	int i;
	uint8_t portbit;
	boolean still_sleeping = _sys_stay_asleep;

	for (i=0; i < 8; i++) {
		portbit = _BV_ARY8[i];

		if ( (P5IFG & portbit) && _isr_callbacks_P5[i] ) {
			_isr_callbacks_P5[i]();
			if (_isr_change_vect_P5 & portbit) {
				P5IES ^= portbit;
				P5IFG = (   (P5IFG & portbit) == (P5IES & portbit) ?  // Check that we haven't already re-triggered before clearing PxIFG
					    (P5IFG & ~portbit) : P5IFG   );
			} else {
				P5IFG &= ~portbit;
			}
		}
	}

	if (still_sleeping != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif

#if defined(__MSP430_HAS_PORT6_R__) && (defined(P6IE) || defined(P6IE_))
static volatile ISR_CALLBACK_PTR _isr_callbacks_P6[8];
static volatile uint8_t _isr_change_vect_P6 = 0x00;

__attribute__((interrupt(PORT6_VECTOR)))
void P6_ISR(void)
{
	int i;
	uint8_t portbit;
	boolean still_sleeping = _sys_stay_asleep;

	for (i=0; i < 8; i++) {
		portbit = _BV_ARY8[i];

		if ( (P6IFG & portbit) && _isr_callbacks_P6[i] ) {
			_isr_callbacks_P6[i]();
			if (_isr_change_vect_P6 & portbit) {
				P6IES ^= portbit;
				P6IFG = (   (P6IFG & portbit) == (P6IES & portbit) ?  // Check that we haven't already re-triggered before clearing PxIFG
					    (P6IFG & ~portbit) : P6IFG   );
			} else {
				P6IFG &= ~portbit;
			}
		}
	}

	if (still_sleeping != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif

#if defined(__MSP430_HAS_PORT7_R__) && (defined(P7IE) || defined(P7IE_))
static volatile ISR_CALLBACK_PTR _isr_callbacks_P7[8];
static volatile uint8_t _isr_change_vect_P7 = 0x00;

__attribute__((interrupt(PORT7_VECTOR)))
void P7_ISR(void)
{
	int i;
	uint8_t portbit;
	boolean still_sleeping = _sys_stay_asleep;

	for (i=0; i < 8; i++) {
		portbit = _BV_ARY8[i];

		if ( (P7IFG & portbit) && _isr_callbacks_P7[i] ) {
			_isr_callbacks_P7[i]();
			if (_isr_change_vect_P7 & portbit) {
				P7IES ^= portbit;
				P7IFG = (   (P7IFG & portbit) == (P7IES & portbit) ?  // Check that we haven't already re-triggered before clearing PxIFG
					    (P7IFG & ~portbit) : P7IFG   );
			} else {
				P7IFG &= ~portbit;
			}
		}
	}

	if (still_sleeping != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif

#if defined(__MSP430_HAS_PORT8_R__) && (defined(P8IE) || defined(P8IE_))
static volatile ISR_CALLBACK_PTR _isr_callbacks_P8[8];
static volatile uint8_t _isr_change_vect_P8 = 0x00;

__attribute__((interrupt(PORT8_VECTOR)))
void P8_ISR(void)
{
	int i;
	uint8_t portbit;
	boolean still_sleeping = _sys_stay_asleep;

	for (i=0; i < 8; i++) {
		portbit = _BV_ARY8[i];

		if ( (P8IFG & portbit) && _isr_callbacks_P8[i] ) {
			_isr_callbacks_P8[i]();
			if (_isr_change_vect_P8 & portbit) {
				P8IES ^= portbit;
				P8IFG = (   (P8IFG & portbit) == (P8IES & portbit) ?  // Check that we haven't already re-triggered before clearing PxIFG
					    (P8IFG & ~portbit) : P8IFG   );
			} else {
				P8IFG &= ~portbit;
			}
		}
	}

	if (still_sleeping != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif

#if defined(__MSP430_HAS_PORT9_R__) && (defined(P9IE) || defined(P9IE_))
static volatile ISR_CALLBACK_PTR _isr_callbacks_P9[8];
static volatile uint8_t _isr_change_vect_P9 = 0x00;

__attribute__((interrupt(PORT9_VECTOR)))
void P9_ISR(void)
{
	int i;
	uint8_t portbit;
	boolean still_sleeping = _sys_stay_asleep;

	for (i=0; i < 8; i++) {
		portbit = _BV_ARY8[i];

		if ( (P9IFG & portbit) && _isr_callbacks_P9[i] ) {
			_isr_callbacks_P9[i]();
			if (_isr_change_vect_P9 & portbit) {
				P9IES ^= portbit;
				P9IFG = (   (P9IFG & portbit) == (P9IES & portbit) ?  // Check that we haven't already re-triggered before clearing PxIFG
					    (P9IFG & ~portbit) : P9IFG   );
			} else {
				P9IFG &= ~portbit;
			}
		}
	}

	if (still_sleeping != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif

#if defined(__MSP430_HAS_PORT10_R__) && (defined(P10IE) || defined(P10IE_))
static volatile ISR_CALLBACK_PTR _isr_callbacks_P10[8];
static volatile uint8_t _isr_change_vect_P10 = 0x00;

__attribute__((interrupt(PORT10_VECTOR)))
void P10_ISR(void)
{
	int i;
	uint8_t portbit;
	boolean still_sleeping = _sys_stay_asleep;

	for (i=0; i < 8; i++) {
		portbit = _BV_ARY8[i];

		if ( (P10IFG & portbit) && _isr_callbacks_P10[i] ) {
			_isr_callbacks_P10[i]();
			if (_isr_change_vect_P10 & portbit) {
				P10IES ^= portbit;
				P10IFG = (   (P10IFG & portbit) == (P10IES & portbit) ?  // Check that we haven't already re-triggered before clearing PxIFG
					    (P10IFG & ~portbit) : P10IFG   );
			} else {
				P10IFG &= ~portbit;
			}
		}
	}

	if (still_sleeping != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif

#if defined(__MSP430_HAS_PORT11_R__) && (defined(P11IE) || defined(P11IE_))
static volatile ISR_CALLBACK_PTR _isr_callbacks_P11[8];
static volatile uint8_t _isr_change_vect_P11 = 0x00;

__attribute__((interrupt(PORT11_VECTOR)))
void P11_ISR(void)
{
	int i;
	uint8_t portbit;
	boolean still_sleeping = _sys_stay_asleep;

	for (i=0; i < 8; i++) {
		portbit = _BV_ARY8[i];

		if ( (P11IFG & portbit) && _isr_callbacks_P11[i] ) {
			_isr_callbacks_P11[i]();
			if (_isr_change_vect_P11 & portbit) {
				P11IES ^= portbit;
				P11IFG = (   (P11IFG & portbit) == (P11IES & portbit) ?  // Check that we haven't already re-triggered before clearing PxIFG
					    (P11IFG & ~portbit) : P11IFG   );
			} else {
				P11IFG &= ~portbit;
			}
		}
	}

	if (still_sleeping != _sys_stay_asleep)
		__bic_SR_register_on_exit(LPM4_bits);
}
#endif



void attachInterrupt(uint16_t pin, ISR_CALLBACK_PTR func, int mode)
{
	uint16_t realpin = _resolve_pin(pin);
	if (!realpin || !((mode == FALLING) || (mode == RISING) || (mode == CHANGE)))
		return;  // Not a real pin; abort

	uint8_t portbit = chip_pins[realpin].portbit;
	if (!portbit)
		return;  // this would be an error <CORE_ERROR>

	enum gpio_port_t port = chip_pins[realpin].port;

	_DINT();
	switch (port) {
		case P1:
		#if defined(__MSP430_HAS_PORT1_R__) && (defined(P1IE) || defined(P1IE_))
			if (mode != CHANGE) {
				P1IES = (mode ? P1IES | portbit : P1IES & ~portbit);
			} else {
				_isr_change_vect_P1 |= portbit;
				P1IES = (P1IES & ~portbit) | (P1IN & portbit);
			}
			P1IFG &= ~portbit;
			_isr_callbacks_P1[_resolve_index_from_portbit(portbit)] = func;
			P1IE |= portbit;
		#endif
			break;
		case P2:
		#if defined(__MSP430_HAS_PORT2_R__) && (defined(P2IE) || defined(P2IE_))
			if (mode != CHANGE) {
				P2IES = (mode ? P2IES | portbit : P2IES & ~portbit);
			} else {
				_isr_change_vect_P2 |= portbit;
				P2IES = (P2IES & ~portbit) | (P2IN & portbit);
			}
			P2IFG &= ~portbit;
			_isr_callbacks_P2[_resolve_index_from_portbit(portbit)] = func;
			P2IE |= portbit;
		#endif
			break;
		case P3:
		#if defined(__MSP430_HAS_PORT3_R__) && (defined(P3IE) || defined(P3IE_))
			if (mode != CHANGE) {
				P3IES = (mode ? P3IES | portbit : P3IES & ~portbit);
			} else {
				_isr_change_vect_P3 |= portbit;
				P3IES = (P3IES & ~portbit) | (P3IN & portbit);
			}
			P3IFG &= ~portbit;
			_isr_callbacks_P3[_resolve_index_from_portbit(portbit)] = func;
			P3IE |= portbit;
		#endif
			break;
		case P4:
		#if defined(__MSP430_HAS_PORT4_R__) && (defined(P4IE) || defined(P4IE_))
			if (mode != CHANGE) {
				P4IES = (mode ? P4IES | portbit : P4IES & ~portbit);
			} else {
				_isr_change_vect_P4 |= portbit;
				P4IES = (P4IES & ~portbit) | (P4IN & portbit);
			}
			P4IFG &= ~portbit;
			_isr_callbacks_P4[_resolve_index_from_portbit(portbit)] = func;
			P4IE |= portbit;
		#endif
			break;
		case P5:
		#if defined(__MSP430_HAS_PORT5_R__) && (defined(P5IE) || defined(P5IE_))
			if (mode != CHANGE) {
				P5IES = (mode ? P5IES | portbit : P5IES & ~portbit);
			} else {
				_isr_change_vect_P5 |= portbit;
				P5IES = (P5IES & ~portbit) | (P5IN & portbit);
			}
			P5IFG &= ~portbit;
			_isr_callbacks_P5[_resolve_index_from_portbit(portbit)] = func;
			P5IE |= portbit;
		#endif
			break;
		case P6:
		#if defined(__MSP430_HAS_PORT6_R__) && (defined(P6IE) || defined(P6IE_))
			if (mode != CHANGE) {
				P6IES = (mode ? P6IES | portbit : P6IES & ~portbit);
			} else {
				_isr_change_vect_P6 |= portbit;
				P6IES = (P6IES & ~portbit) | (P6IN & portbit);
			}
			P6IFG &= ~portbit;
			_isr_callbacks_P6[_resolve_index_from_portbit(portbit)] = func;
			P6IE |= portbit;
		#endif
			break;
		case P7:
		#if defined(__MSP430_HAS_PORT7_R__) && (defined(P7IE) || defined(P7IE_))
			if (mode != CHANGE) {
				P7IES = (mode ? P7IES | portbit : P7IES & ~portbit);
			} else {
				_isr_change_vect_P7 |= portbit;
				P7IES = (P7IES & ~portbit) | (P7IN & portbit);
			}
			P7IFG &= ~portbit;
			_isr_callbacks_P7[_resolve_index_from_portbit(portbit)] = func;
			P7IE |= portbit;
		#endif
			break;
		case P8:
		#if defined(__MSP430_HAS_PORT8_R__) && (defined(P8IE) || defined(P8IE_))
			if (mode != CHANGE) {
				P8IES = (mode ? P8IES | portbit : P8IES & ~portbit);
			} else {
				_isr_change_vect_P8 |= portbit;
				P8IES = (P8IES & ~portbit) | (P8IN & portbit);
			}
			P8IFG &= ~portbit;
			_isr_callbacks_P8[_resolve_index_from_portbit(portbit)] = func;
			P8IE |= portbit;
		#endif
			break;
		case P9:
		#if defined(__MSP430_HAS_PORT9_R__) && (defined(P9IE) || defined(P9IE_))
			if (mode != CHANGE) {
				P9IES = (mode ? P9IES | portbit : P9IES & ~portbit);
			} else {
				_isr_change_vect_P9 |= portbit;
				P9IES = (P9IES & ~portbit) | (P9IN & portbit);
			}
			P9IFG &= ~portbit;
			_isr_callbacks_P9[_resolve_index_from_portbit(portbit)] = func;
			P9IE |= portbit;
		#endif
			break;
		case P10:
		#if defined(__MSP430_HAS_PORT10_R__) && (defined(P10IE) || defined(P10IE_))
			if (mode != CHANGE) {
				P10IES = (mode ? P10IES | portbit : P10IES & ~portbit);
			} else {
				_isr_change_vect_P10 |= portbit;
				P10IES = (P10IES & ~portbit) | (P10IN & portbit);
			}
			P10IFG &= ~portbit;
			_isr_callbacks_P10[_resolve_index_from_portbit(portbit)] = func;
			P10IE |= portbit;
		#endif
			break;
		case P11:
		#if defined(__MSP430_HAS_PORT11_R__) && (defined(P11IE) || defined(P11IE_))
			if (mode != CHANGE) {
				P11IES = (mode ? P11IES | portbit : P11IES & ~portbit);
			} else {
				_isr_change_vect_P11 |= portbit;
				P11IES = (P11IES & ~portbit) | (P11IN & portbit);
			}
			P11IFG &= ~portbit;
			_isr_callbacks_P11[_resolve_index_from_portbit(portbit)] = func;
			P11IE |= portbit;
		#endif
			break;
		case PJ:
		case NOT_A_PORT:
			// Port J never has interrupts; this just shuts up a compiler warning
			break;

	}
	_EINT();
}

void detachInterrupt(uint16_t pin)
{
	uint16_t realpin = _resolve_pin(pin);
	if (!realpin)
		return;  // Not a real pin; abort

	uint8_t portbit = chip_pins[realpin].portbit;
	if (!portbit)
		return;  // this would be an error <CORE_ERROR>

	enum gpio_port_t port = chip_pins[realpin].port;

	switch (port) {
		case P1:
		#if defined(__MSP430_HAS_PORT1_R__) && (defined(P1IE) || defined(P1IE_))
			P1IE |= portbit;
			_isr_callbacks_P1[_resolve_index_from_portbit(portbit)] = NULL;
			_isr_change_vect_P1 &= ~portbit;
		#endif
			break;
		case P2:
		#if defined(__MSP430_HAS_PORT2_R__) && (defined(P2IE) || defined(P2IE_))
			P2IE |= portbit;
			_isr_callbacks_P2[_resolve_index_from_portbit(portbit)] = NULL;
			_isr_change_vect_P2 &= ~portbit;
		#endif
			break;
		case P3:
		#if defined(__MSP430_HAS_PORT3_R__) && (defined(P3IE) || defined(P3IE_))
			P3IE |= portbit;
			_isr_callbacks_P3[_resolve_index_from_portbit(portbit)] = NULL;
			_isr_change_vect_P3 &= ~portbit;
		#endif
			break;
		case P4:
		#if defined(__MSP430_HAS_PORT4_R__) && (defined(P4IE) || defined(P4IE_))
			P4IE |= portbit;
			_isr_callbacks_P4[_resolve_index_from_portbit(portbit)] = NULL;
			_isr_change_vect_P4 &= ~portbit;
		#endif
			break;
		case P5:
		#if defined(__MSP430_HAS_PORT5_R__) && (defined(P5IE) || defined(P5IE_))
			P5IE |= portbit;
			_isr_callbacks_P5[_resolve_index_from_portbit(portbit)] = NULL;
			_isr_change_vect_P5 &= ~portbit;
		#endif
			break;
		case P6:
		#if defined(__MSP430_HAS_PORT6_R__) && (defined(P6IE) || defined(P6IE_))
			P6IE |= portbit;
			_isr_callbacks_P6[_resolve_index_from_portbit(portbit)] = NULL;
			_isr_change_vect_P6 &= ~portbit;
		#endif
			break;
		case P7:
		#if defined(__MSP430_HAS_PORT7_R__) && (defined(P7IE) || defined(P7IE_))
			P7IE |= portbit;
			_isr_callbacks_P7[_resolve_index_from_portbit(portbit)] = NULL;
			_isr_change_vect_P7 &= ~portbit;
		#endif
			break;
		case P8:
		#if defined(__MSP430_HAS_PORT8_R__) && (defined(P8IE) || defined(P8IE_))
			P8IE |= portbit;
			_isr_callbacks_P8[_resolve_index_from_portbit(portbit)] = NULL;
			_isr_change_vect_P8 &= ~portbit;
		#endif
			break;
		case P9:
		#if defined(__MSP430_HAS_PORT9_R__) && (defined(P9IE) || defined(P9IE_))
			P9IE |= portbit;
			_isr_callbacks_P9[_resolve_index_from_portbit(portbit)] = NULL;
			_isr_change_vect_P9 &= ~portbit;
		#endif
			break;
		case P10:
		#if defined(__MSP430_HAS_PORT10_R__) && (defined(P10IE) || defined(P10IE_))
			P10IE |= portbit;
			_isr_callbacks_P10[_resolve_index_from_portbit(portbit)] = NULL;
			_isr_change_vect_P10 &= ~portbit;
		#endif
			break;
		case P11:
		#if defined(__MSP430_HAS_PORT11_R__) && (defined(P11IE) || defined(P11IE_))
			P11IE |= portbit;
			_isr_callbacks_P11[_resolve_index_from_portbit(portbit)] = NULL;
			_isr_change_vect_P11 &= ~portbit;
		#endif
			break;
		case PJ:
		case NOT_A_PORT:
			// Port J never has interrupts; this just shuts up a compiler warning
			break;
	}
}

boolean deriveInterruptDetails(uint16_t pin, volatile uint8_t **maskie_store, uint8_t *maskie_portbit_store)
{
	uint16_t realpin = _resolve_pin(pin);
	if (!realpin)
		return false;  // Not a real pin; abort

	uint8_t portbit = chip_pins[realpin].portbit;
	if (!portbit)
		return false;  // this would be an error <CORE_ERROR>

	enum gpio_port_t port = chip_pins[realpin].port;

	*maskie_store = SFRPTR_BYTE chip_ports[port].portie;  // If port isn't IRQ-capable, this ends up being NULL
	*maskie_portbit_store = portbit;

	return true;
}

#ifdef __cplusplus
};  /* extern "C" */
#endif
