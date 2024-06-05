/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
 
#ifndef IR_H
#define	IR_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define US_TIMER 250

void sendIR(uint32_t command);

void delayUS(uint32_t USTime);
void oscillateCarrierDuration(uint32_t USTime);

#endif	/* IR_H */

