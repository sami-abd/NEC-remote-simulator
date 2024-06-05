/* 
 * File:   main.c
 * Author: Sami, Sara, Spencer
 *
 * Created on November 26th, 2020
 */
// MPLAB header libraries
#include <xc.h>

#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

// User header files
#include "ChangeClk.h"
#include "IOS.h"
#include "IRTX.h"
#include "TimeDelay.h"
#include "UART2.h"


#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled

int main(void) {
    LATBbits.LATB8 = 1;  // Selecting analog
    initTimer2();         // Initialize timer2 is used for 38kHz pulses
    initTimer1();          //Initialize timer1 used to measure time of PB presses
    NewClk(8);           // Setting Clock to 8 MHz
    IOInit();        //initialize IO
    InitUART2();
    
    
    //entering state machine (infinite loop to check ADC)
    while(1)
    {
        IOCheck();// beginning the state machine
    }
    
    return 0;
}
