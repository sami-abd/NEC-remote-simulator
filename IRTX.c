/*
 * File:   IR.c
 * Author: Sara
 *
 * Created on December 13, 2020, 6:37 PM
 */


//#include "xc.h"
#include "IRTX.h"
#include "UART2.h"
#include "TimeDelay.h"
#include <libpic30.h>
#include <p24fxxxx.h>
#include <p24F16KA101.h>

#define IROutput LATBbits.LATB9

unsigned long IRValue;
extern _Bool TMR2Flag; //flag to know if tmr2 is done 
extern _Bool TMR1FlagAlert; // global bool to see if 3 seconds has passed
extern volatile uint32_t TMR1ValueUs; // holds the value of TMR2 in (uS)



void delayUS(uint32_t USTime)
{
   setTMR2US(USTime); // set timer2 for USTime amount of time (us)
   Idle(); // Idle until timer is complete
}

void oscillateCarrierDuration(uint32_t USTime){ // oscilates the output at 34kHz for the specified duration
    
    setTMR2US(USTime);
    while(TMR2Flag == 0)
    {
        IROutput = !IROutput; // flips the IR output between high and low
        __delay32(34); // 
    }
    IROutput = 0;
    
}

void sendIR(uint32_t command) {
    // start signal
    oscillateCarrierDuration(4500);
    delayUS(4500);

    
    //command
    int i;
    for (i = 0; i < 32; ++i){   //iterate through bits from MSB to LSB
        oscillateCarrierDuration(560);     //send 0.56ms on 
        if ((command >> i) & 1) { //if bit 1 send 1.69ms off
            delayUS(1690);
        } else {                  //if bit 0 send 0.56ms off
            delayUS(560);
        }
    }
}