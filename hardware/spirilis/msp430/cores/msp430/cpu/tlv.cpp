/* Energia-on-Arduino 1.6+ TLV database reader for MSP430
 *
 * 2015 Eric Brundick <spirilis at linux dot com>
 */

#include <cpu/tlv.h>


#ifndef __MSP430_HAS_TLV__
// MSP430F2xxx/G2xxx

#define TLV_INFO_A ((void *)0x10C0)
#define TLV_ARRAY_LEN 64

void * tlv_find(uint8_t tag_id, uint16_t *len)
{
	uint16_t *ptr = (uint16_t *)TLV_INFO_A;
	uint16_t *ptr_start = ptr;

	// First word is a checksum
	ptr++;
	while ( (*ptr & 0xFF) != tag_id && (2 * (unsigned int)(ptr - ptr_start)) < TLV_ARRAY_LEN) {
		ptr += (*ptr >> 8) / 2;  // Pointer math; uint16_t pointers increment in units of 2 bytes
	}

	if ( (2 * (unsigned int)(ptr - ptr_start)) >= TLV_ARRAY_LEN) {
		return NULL;
	}

	if (len != NULL)
		*len = *ptr >> 8;
	return ++ptr;  // Point the user to the start of the TLV data itself
}
#endif /* !MSP430_HAS_TLV */

#ifdef __MSP430_HAS_TLV__
void * tlv_find(uint8_t tag_id, uint16_t *len)
{
	uint16_t *ptr = TLV_START;

	// First word is a checksum
	ptr++;
	while ( (*ptr & 0xFF) != tag_id && (ptr < TLV_END) ) {
		ptr += (*ptr >> 8) / 2;  // Pointer math; uint16_t pointers increment in units of 2 bytes
	}

	if (ptr >= TLV_END)
		return NULL;

	if (len != NULL)
		*len = *ptr >> 8;
	return ++ptr;  // Point the user to the start of the TLV data itself
}
#endif /* MSP430_HAS_TLV */
