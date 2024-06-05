/*
 * File:   IO.c
 * Author: Sami, Sara, Spencer
 *
 * Created on December 13, 2020, 5:44 PM
 * this file includes all IO interrupts
 */
#include "IOS.h"
#include "UART2.h"

// timer flags made viewable from TimeDelay.c
extern _Bool TMR2Flag = 0; //flag to know if tmr2 is done
extern _Bool TMR1FlagAlert = 0; // global bool to see if 3 seconds has passed
extern volatile uint32_t TMR1ValueUs = 0; // holds the value of TMR2 in (uS)

extern uint8_t PButtons = 0b00000000; // LSB == RA2, bit1 = RA4. bit 2 = RB4

extern _Bool TMR1Began = 0; // Flag to keep track of how long buttons are pushed
 _Bool modeChangeRequest = 0; // change of mode flag
 extern _Bool readyToSend = 0;

 uint8_t mode; //mode 0 = channel, mode 1 = volume

void IOInit(void){
    AD1PCFG = 0xFFFF;
    
    TRISAbits.TRISA2 = 1;   //sets RA2 as input connected to PB1
    TRISAbits.TRISA4 = 1;   //sets RA4 as input connected to PB2
    TRISBbits.TRISB4 = 1;   //sets RB4 as input connected to PB3
    
    TRISBbits.TRISB9 = 0;   //sets RB9 as output IR Sensor
    
    CNPU1bits.CN0PUE = 1;   //pulling input hi
    CNPU1bits.CN1PUE = 1;   //pulling input hi
    CNPU2bits.CN30PUE = 1;  //pulling input hi

    CNEN2bits.CN30IE = 1;    //Enable CN30/RA2 as interrupt
    CNEN1bits.CN1IE = 1;    //Enable CN1/RB4 as interrupt
    CNEN1bits.CN0IE = 1;    //Enable CN0/RA4 as interrupt
    
    CNPU2bits.CN30PUE = 1;   //Pin CN30/RA2 pull down enable
    CNPU1bits.CN1PUE = 1;   //Pin CN1/RB4 pull down enable
    CNPU1bits.CN0PUE = 1;   //Pin CN0/RA4 pull down enable
    
    IPC4bits.CNIP = 7;      //button highest priority
    IFS1bits.CNIF = 0;      //Clear interrupt
    
    IEC1bits.CNIE = 1;      //Enables change notification interrupt
    
}

//ISR for Change of Notification
void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void){
    debouncer();    //debounces CN inputs
    IFS1bits.CNIF = 0;      // turning interrupt flag to 0  
    readyToSend = 1; //meant to limit power sending frequency
}

static void handleChannel() {
    if (PButtons == 0b00000001) //if pb1 was pressed
    {
        Disp2String("\rPB1 is pressed.   Channel mode: Channel Up              \n");
        sendIR(CH_UP);//send Ch Up command

    } 
    else if (PButtons == 0b00000010)   //if just PB2 pressed
    { 
        Disp2String("\rPB2 is pressed.   Channel mode: Channel Down            \n");
        sendIR(CH_DOWN);//send Ch Down command
    }
}

static void handleVolume() {
    if (PButtons == 0b00000001) //if pb1 was pressed
    {
        Disp2String("\rPB1 is pressed.   Volume mode: Volume Up                \n");
        sendIR(VOL_UP);//send Vol Up command

    } 
    else if (PButtons == 0b00000010)   //if just PB2 pressed
    { 
        Disp2String("\rPB2 is pressed.   Volume mode: Volume Down              \n");
        sendIR(VOL_DOWN);//send Vol Down command
    }
}

static void handleModeChange() {
    Disp2String("\rPB3 is pressed for less than 3 sec. ");
    mode = !mode;
    Disp2String(mode == 1 ? "Switched to Channel mode.               \n" :
        "Switched to Volume mode.                \n");
}

static void handlePB3() {
    if(TMR1Began == 0)
    {
        TMR1On();
        TMR1Began=1;
    }

    if(TMR1FlagAlert == 0) //if the 3 second Alert hasn't triggered 
        modeChangeRequest = 1; // set mode change request on
    else 
    {
        if(readyToSend == 1){
         modeChangeRequest = 0; //otherwise it has and mode change off
         Disp2String("\r PB3 is pressed for 3 sec or more. TV ON/Off                    \n");
         sendIR(POWER);
         readyToSend = 0;
        }
    }
}

void IOCheck(void){
    //mode = 0; // default channel mode
    if(PButtons == 0b0000100 ) //button 3 pressed
        handlePB3();
    else if(PButtons == 0b0000000) //no buttons are pressed
    {
        modeChangeRequest = 0; //ensure that mode change is off
        Idle(); //remain in idle until a change of notification
    }
    else // PB1/PB2 pressed
    {
        if (TMR1Began == 1) //if TMR1 began then reset it
        {
            TMR1Off();
            TMR1Began = 0;
        }
        
        if( modeChangeRequest == 1) // if a change in mode is requested switch modes
            handleModeChange();
        
        if (mode == 1) //mode is volume
            handleVolume();
        else if (mode == 0) //mode is channel
            handleChannel();
        
        
    }
    
    
}

void updateButtons()
{
    PButtons = 0b00000000; // defaults 0 unless checked
    
    //the last three bits represents the states of the pushbuttons
    if (PORTAbits.RA2==0){  //pb1
        PButtons |= 0b00000001;
    }
    if (PORTAbits.RA4==0){  //pb2
        PButtons |= 0b00000010;
    }
    if (PORTBbits.RB4==0){  //pb3
        PButtons |= 0b0000100;
    }
}

void debouncer() {
    uint16_t dmax = 13750; // this is the max ctr, optimized for 8 MHz
    uint16_t ctr = 0; // ctr used to  wait for debouncing

    uint8_t currentPB = 0b0000000;   // Holds current value of PButtons
    uint8_t prevPB = 0b00000000;  // holds the value of the previous PButtons

    while (dmax >= ctr){
        updateButtons();
        currentPB = PButtons;     //gets button input
        
        if (currentPB == prevPB){
            ctr++; // increments when state hasn't changed
        }
        else{
            ctr = 0;               //ctr only increments when prev==current
        }
        prevPB = currentPB;   // prev value is recorded
    }
}
   
