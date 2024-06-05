/*
 * File:   TimeDelay.c
 * Author: Sami, Sara, Spencer
 *
 * Created on November 5, 2020, 11:36 PM
 */
#include "TimeDelay.h"
#include "IOS.h"


// timer 2 flag
extern _Bool TMR2Flag;//flag to know if tmr2 is done
extern _Bool TMR1FlagAlert; // global bool to see if 3 seconds has passed
extern volatile uint32_t TMR1ValueUs; // holds the value of TMR2 in (uS)
extern _Bool readyToSend; // limits frequency of power signal to 1 per 3second press

void initTimer2() { //meant to generate 32kHz pulse
	//timer 2 setup
    T2CONbits.TSIDL = 0; //ensures working while idle
    T2CONbits.T32 = 0b00;
    T2CONbits.TCKPS = 0b00;
    T2CONbits.TCS = 0;
    
	//interrupt setup
    IPC1bits.T2IP = 0x4;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    TMR2 = 0;
} 

void initTimer1() { //meant for measuring the periods of on and off
	//timer 1 setup
    T1CONbits.TSIDL = 0; //ensures working while idle
    T1CONbits.TCKPS = 0b00;  //Clock prescale 1:1
    T1CONbits.TCS = 0;  //Internal clock
    
	//interrupt setup
    IPC0bits.T1IP = 0x3;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;  //enable timer interrupt
    TMR1 = 0;
} 

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag
    T2CONbits.TON = 0; //Turn off the timer
    TMR2 = 0;   //set value to zero
    TMR2Flag = 1; // global tmr2 end flag
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    
    IFS0bits.T1IF = 0;  //Clear timer 1 interrupt flag
//    T1CONbits.TON = 0;  //Turn off timer
//    TMR1 = 0; // setting tmr1 to 0
    
    TMR1ValueUs += 16350; // increments the timer value in my microseconds
    
    if (3000 < (TMR1ValueUs/1000))
        TMR1FlagAlert = 1;
    
}
void TMR1On(void) // turns on timer 1
{
    T1CONbits.TSIDL = 0; //ensures working while idle
    initTimer1(); // initialize 
    TMR1ValueUs = 0; //setting the us to 0
    PR1 = 65000; // maximum value to stop at  
    T1CONbits.TON = 1; //turns on the timer
    
    
}
uint32_t TMR1Off(void) // returns the time tmr1 was on for in ms 
{
    T1CONbits.TON = 0; //turns off tmr1
    TMR1FlagAlert = 0; // sets global flag for for TMR1 off
    uint32_t x = TMR1ValueUs;
    TMR1ValueUs = 0;
    return x/1000;
}

void setTMR2US(uint32_t USTime) //sets the timer2 on for specified (us)
{
    T2CONbits.TON = 1; //turn timer2 on
    TMR2Flag = 0;
    USTime *= 4; //this is the conversion for the timer to run for given (us)
    
    T2CONbits.TSIDL = 0; //ensures working while idle
    T2CONbits.T32 = 0b00; // avoiding interference between T2 and T1
    T2CONbits.TCKPS = 0b00; // internal clock
    T2CONbits.TCS = 0;  // prescale 1:1
    
    PR2 = USTime; //setting PR2 register with specified time duration
    
}
