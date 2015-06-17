/* Energia-on-Arduino 1.6+ HardwareSerial USCI/eUSCI implementation
 * USCI ISR functions
 */

#ifndef USCI_ISR_H
#define USCI_ISR_H

#include <Energia.h>
#include <usci/layout.h>

#ifdef __cplusplus
extern "C" {
#endif


void usci_isr_dummy();  // Useless function just to ensure the linker pulls in usci/isr.cpp


#ifdef __cplusplus
};  /* extern "C" */
#endif

#endif /* USCI_ISR_H */
