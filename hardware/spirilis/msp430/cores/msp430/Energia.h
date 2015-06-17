/* Energia-Arduino port main header */

#ifndef ENERGIA_H
#define ENERGIA_H


#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "binary.h"
#include <pins_booster.h>


#define ALWAYS_INLINE inline __attribute__((always_inline))
#define NEVER_INLINE __attribute__((noinline))

#define F_CPU (cpu.mclk())

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define HIGH 0x1
#define LOW  0x0

#define LSBFIRST 0
#define MSBFIRST 1

#define RISING 0
#define FALLING 1
#define CHANGE 2

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x4
// PORT_SELECTION0 and PORT_SELECTION1 are now defined in variant_schema.h (included in pins_booster.h) so
// they're available in chip-specific variant headers for the USCI pin modes.

#define true 0x1
#define false 0x0

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105


typedef uint8_t boolean;
typedef uint8_t byte;

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() __bis_SR_register(GIE)
#define noInterrupts() __bic_SR_register(GIE)

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


typedef uint16_t word;

#define bit(b) (1UL << (b))

#ifdef __cplusplus
};
#endif

#include <avr/dtostrf.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#if defined(__cplusplus) && !defined(IS_CPUFREQ_CPP)
#include <cpu/cpufreq.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void setup(void);
void loop(void);

#define SFRPTR_BYTE (volatile uint8_t *)
#define SFRPTR_WORD (volatile uint16_t *)

void shiftOut(uint16_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint16_t dataPin, uint8_t clockPin, uint8_t bitOrder);
unsigned long pulseIn(uint16_t pin, uint8_t state, unsigned long timeout);
void pinMode(uint16_t, uint16_t);
void digitalWrite(uint16_t, uint8_t);
int digitalRead(uint16_t);
uint16_t analogRead(uint16_t);
void analogWrite(uint16_t, int);
void analogReference(uint16_t);
void analogFrequency(uint32_t);
void analogResolution(uint16_t);

void delay(uint32_t milliseconds);
void delayMicroseconds(unsigned int us);
void sleep(uint32_t milliseconds);
void sleepSeconds(uint32_t seconds);
void suspend(void);
extern volatile boolean _sys_stay_asleep;
ALWAYS_INLINE void wakeup() { _sys_stay_asleep = false; }

void attachInterrupt(uint16_t, void (*)(void), int mode);
void detachInterrupt(uint16_t);
boolean deriveInterruptDetails(uint16_t, volatile uint8_t **, uint8_t *);

extern volatile uint32_t _sys_millis;
extern volatile uint16_t _sys_micros;

ALWAYS_INLINE
unsigned long micros() { return _sys_millis * 1000 + (uint32_t)_sys_micros; }

ALWAYS_INLINE
unsigned long millis() { return _sys_millis; }


#ifdef __cplusplus
};  /* extern "C" */
#endif // __cplusplus


#ifdef __cplusplus
#include "WCharacter.h"
#include "WString.h"

inline unsigned int makeWord(unsigned int w) {
	return w;
}

inline unsigned int makeWord(unsigned char h, unsigned char l) {
	return (h << 8) | l;
}

inline unsigned int flipWord(unsigned int w) {
	register uint16_t _w = w;
	return (w << 8) | (_w >> 8);
}


#define word(...) makeWord(__VA_ARGS__)

unsigned long pulseIn(uint16_t pin, uint8_t state, unsigned long timeout = 1000000L);

void tone(uint16_t _pin, unsigned int frequency, unsigned long duration = 0);
void noTone(uint16_t _pin);

// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned int);
long map(long, long, long, long, long);

#include <HardwareSerial.h>

#endif // __cplusplus

#endif // ENERGIA_H
