/* 
 * File:   IOs.h
 * Author: Sami, Sara, Spencer
 *
 * Created on October 20, 2020, 6:51 PM
 */

#ifndef TIMEDELAY_H
#define	TIMEDELAY_H

#include <xc.h> // include processor files - each processor file is guarded. 
#define FCY 4000000
#include <libpic30.h>



//prototype functions
void initTimer2();
void initTimer1();
void TMR1On(void);  //function setup. Defined in .c file
uint32_t TMR1Off(void);  //function setup. Defined in .c file
void setTMR2US(uint32_t time_us);


#endif	/* TIMEDELAY_H */
