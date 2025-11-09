/*
 * File:   Main.c
 * Author: azroe
 *
 * Created on October 27, 2025, 4:35 PM
 */


#include "xc.h"

void config_cm1(void);
void config_cm2(void);
void config_cm3(void);
void config_cvref(void);


// Select oscillator here
#pragma config FNOSC = LPRC //Use 500 kHz oscillator

// States
enum { STRAIGHT, RIGHT, LEFT } state;

void _ISR _CompInterrupt(void)
{
    _CMIF = 0;                  // Clear general comparator interrupt flag
    
    if (CM1CONbits.CEVT == 1) { //if right is triggered
        if (CM3CONbits.COUT == 0) { //if center is off
            state = RIGHT; //go right
        }
    }
    else if (CM2CONbits.CEVT == 1) { //if left is triggered
        if (CM3CONbits.COUT == 0) { //if center is off
            state = LEFT; //go left
        }
    }
    else if (CM3CONbits.CEVT == 1) { //if center is triggered
        if (state != STRAIGHT) { //if state is not straight
            state = STRAIGHT; //go straight
        }
    }
    CM1CONbits.CEVT = 0;        // Clear individual comparator flag
    CM2CONbits.CEVT = 0; 
    CM3CONbits.CEVT = 0; 
}


int main(void) {
    config_cm1();
    config_cm2();
    config_cm3();
    config_cvref();
    // Configure oscillator (postscaler?)
_RCDIV = 0b000; //Sets postscaler to 1

// Configure Right Motor PWM
OC1CON1 = 0; // Clear all bits of OC1CON1
OC1CON2 = 0; // Clear all bits of OC1CON2
OC2CON1bits.OCTSEL = 0b111; // System clock as timing source
OC2CON2bits.SYNCSEL = 0x1F; // Self-synchronization
OC2CON2bits.OCTRIG = 0; // Synchronization mode
OC2CON1bits.OCM = 0b110; // Edge-aligned PWM mode

//Left Motor PWM configuration
OC3CON1bits.OCTSEL = 0b111; // System clock as timing source
OC3CON2bits.SYNCSEL = 0x1F; // Self-synchronization
OC3CON2bits.OCTRIG = 0; // Synchronization mode
OC3CON1bits.OCM = 0b110; // Edge-aligned PWM mode

//Configure on/off
_ANSA0 = 0;
_ANSA1 = 0;
//Right Motor Direction
_TRISA0 = 0;
//left Motor Direction
_TRISA1 = 0;
//configure sensors
_ANSA2 = 1; //analog
_ANSA3 = 1;
_ANSB14 = 1;
_TRISA2 = 1; //input
_TRISA3 = 1;
_TRISB14 = 1;

//Set state
state = STRAIGHT;

// Set direction forward
_LATA0 = 1;
_LATA1 = 1;

while (1){
    if (state == STRAIGHT) {
        //Right Motor PWM
        OC2RS = 80;
        OC2R = 30;
        
        //Left Motor PWM
        OC3RS = 80;
        OC3R = 30;
    }
    else if (state == RIGHT) {
        //Right Motor PWM
        OC2RS = 40;
        OC2R = 15;
        
        //Left Motor PWM
        OC3RS = 80;
        OC3R = 30;
    }
    else if (state == LEFT) {
        //Right Motor PWM
        OC2RS = 80;
        OC2R = 30;
        
        //Left Motor PWM
        OC3RS = 40;
        OC3R = 15;
    }
}
    return 0;
}

void config_cm1(void)
{
    // Comparator Interrupt
    _CMIE = 0;              // Disable comparator interrupt
    
    // CM1CON Register
    CM1CON = 0;
    CM1CONbits.COE = 0;     // Disable output pin -- Comparator
                            // result is NOT passed through to
                            // C1OUT pin (Pin 17)
    CM1CONbits.CPOL = 1;    // Invert sense, so +in high --> out low
    CM1CONbits.EVPOL = 0b11;// Interrupt occurs on all comparator
                            // output changes
    CM1CONbits.CREF = 1;    // Non-inverting input comes from
                            // internal CVref voltage
    CM1CONbits.CCH = 0b00;  // Inverting input comes from
                            // C1INB pin (pin 7)
    CM1CONbits.CON = 1;     // Enable input
    
    // Configure Interrupt
    CM1CONbits.CEVT = 0;    // Clear the event flag
    _CMIF = 0;              // Clear the global comparator interrupt flag
    
    // Enable comparator interrupt
    _CMIE = 1;              
    
}
void config_cm2(void)
{
    // Comparator Interrupt
    _CMIE = 0;              // Disable comparator interrupt
    
    // CM1CON Register
    CM2CON = 0;
    CM2CONbits.COE = 0;     // Disable output pin -- Comparator
                            // result is NOT passed through to
                            // C1OUT pin (Pin 17)
    CM2CONbits.CPOL = 1;    // Invert sense, so +in high --> out low
    CM2CONbits.EVPOL = 0b11;// Interrupt occurs on all comparator
                            // output changes
    CM2CONbits.CREF = 1;    // Non-inverting input comes from
                            // internal CVref voltage
    CM2CONbits.CCH = 0b01;  // Inverting input comes from
                            // C1INB pin (pin 8)
    CM2CONbits.CON = 1;     // Enable input
    
    // Configure Interrupt
    CM2CONbits.CEVT = 0;    // Clear the event flag
    _CMIF = 0;              // Clear the global comparator interrupt flag
    
    // Enable comparator interrupt
    _CMIE = 1;              
    
}

void config_cm3(void)
{
    // Comparator Interrupt
    _CMIE = 0;              // Disable comparator interrupt
    
    // CM1CON Register
    CM3CON = 0;
    CM3CONbits.COE = 0;     // Disable output pin -- Comparator
                            // result is NOT passed through to
                            // C1OUT pin (Pin 17)
    CM3CONbits.CPOL = 1;    // Invert sense, so +in high --> out low
    CM3CONbits.EVPOL = 0b11;// Interrupt occurs on all comparator
                            // output changes
    CM3CONbits.CREF = 1;    // Non-inverting input comes from
                            // internal CVref voltage
    CM3CONbits.CCH = 0b00;  // Inverting input comes from
                            // C1INB pin (pin 17)
    CM3CONbits.CON = 1;     // Enable input
    
    // Configure Interrupt
    CM3CONbits.CEVT = 0;    // Clear the event flag
    _CMIF = 0;              // Clear the global comparator interrupt flag
    
    // Enable comparator interrupt
    _CMIE = 1;              
    
}


void config_cvref(void)
{
    // CVRCON Register
    CVRCON = 0;
    CVRCONbits.CVROE = 0;       // Output Enable Bit -- Reference
                                // voltage is disconnected from
                                // CVref pin -- If this bit is set,
                                // then the selected reference
                                // voltage appears on the CVref
                                // pin (pin 17)
    CVRCONbits.CVRSS = 0;       // Source Selection Bit --
                                // CVrsrc = Vdd - Vss
    CVRCONbits.CVR = 0b10000;   // Value Selection Bits --
                                // CVR = 0b10000 --> CVref = 1.65 V
    CVRCONbits.CVREN = 1;       // Voltage Reference Enable Bit
}
