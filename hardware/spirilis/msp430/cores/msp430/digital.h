/* Common functions used in digital and/or analog I/O by other components of the Energia core */

#ifndef _DIGITAL_H
#define _DIGITAL_H


/* Resolving physical pin from pin ID provided by user */
static inline uint16_t _resolve_pin(uint16_t pin) {
	if ( (pin & 0x80FF) > 0x8000 )
		return 0;  // Invalid pin - these are used for ADC analogRead() only
	if (pin > 255) {
		register uint16_t _p = pin & 0xFF00;
		asm volatile("swpb %[p]" : [p] "=r" (_p) : "r" (_p));
		return _p;
	}
	if (boosterpack_pins[pin] == NOT_A_PIN)
		return 0;  // Not a real pin
	return boosterpack_pins[pin];
}

/* Derive an integer bit position from a binary value (least significant bit reported) */
extern const uint8_t _BV_ARY8[];
static inline int _resolve_index_from_portbit(uint8_t bit)
{
	int i;

	if (!bit)
		return -1;

	for (i=0; i < 8; i++) {
		if (bit & _BV_ARY8[i])
			return i;
	}
	return -1;  // shouldn't reach here
}



#endif /* _DIGITAL_H */
