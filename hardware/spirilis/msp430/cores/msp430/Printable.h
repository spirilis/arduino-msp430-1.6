/* msp430-elf-gcc seems to have an odd issue with forward-declared classes in a header file chain that
 * does not ultimately include the actual class definition... so we've modified Printable.h to redirect to
 * Print.h, with the actual Printable class definition in "Printable_real.h" and only included within Print.h
 */

#include <Print.h>
