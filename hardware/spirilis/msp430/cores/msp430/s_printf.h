#ifndef SPRINTF_H
#define SPRINTF_H

#include <stdint.h>
#include <stdarg.h>

/* Printf() derived from oPossum's code - http://forum.43oh.com/topic/1289-tiny-printf-c-version/
 * Doctored up to make GCC happy.  Implemented as s_printf().
 */

#ifdef __cplusplus
extern "C" {
#endif

uint16_t s_printf(char *instr, char *format, ...);

#ifdef __cplusplus
};  /* extern "C" */
#endif

#endif
