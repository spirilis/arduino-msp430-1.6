/* Energia-on-Arduino 1.6+ TLV database reader for MSP430
 *
 * 2015 Eric Brundick <spirilis at linux dot com>
 */

#ifndef TLV_H
#define TLV_H

#include <Energia.h>

void * tlv_find(uint8_t tag_id, uint16_t * len);  // len is a pointer to a uint16_t which will be written, or NULL if you don't care

#endif /* TLV_H */
