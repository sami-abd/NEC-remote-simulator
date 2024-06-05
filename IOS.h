/* 
 * File:   IOs.h
 * Author: Sami, Sara, Spencer
 * Comments:
 * Revision history: 
 */
  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h>
#include "TimeDelay.h"

#define PB1 PORTAbits.RA2
#define PB2 PORTAbits.RA4
#define PB3 PORTBbits.RB4

#define POWER 0xE0E040BF
#define CH_UP 0xE0E048B7
#define CH_DOWN 0xE0E008F7
#define VOL_UP 0xE0E0E01F
#define VOL_DOWN 0xE0E0D02F

#define TRANSMITTER LATBbits.LATB9

void IOInit(); 
void IOCheck();
void debouncer();
void updateButtons();


#endif	/* XC_HEADER_TEMPLATE_H */

