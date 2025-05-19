
// PIC24FJ128GA010 Configuration Bit Settings

// 'C' source line config statements

// CONFIG2
#pragma config POSCMOD = XT             // Primary Oscillator Select (XT Oscillator mode selected)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog Timer is enabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "libpic30.h"
#include "adc.h"
#include "buttons.h"


void init(void){
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    TRISA = 0x0000; //port A na wyjscie
    TRISD |= (1 << 6); //rd6 jako wejscie
    
    
}


int main(void) {
    
    init();
    
    unsigned long adc_value;
    unsigned char tryb = 0;
    unsigned char poprzedni_stan_przycisku =1;
    unsigned long opoznienie;
    
    while(1){
        adc_value = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
        if(adc_value == 0xFFFF){continue;}
        opoznienie = 40000 + (adc_value *4000);
        unsigned char stan_przycisku = PORTDbits.RD6;
        if(poprzedni_stan_przycisku == 1 && stan_przycisku == 0){
            tryb ^= 1;
            __delay32(40000);
        }
        poprzedni_stan_przycisku = stan_przycisku;
        if(tryb == 0){
            LATA = 0x0001;
            __delay32(opoznienie);
            LATA = 0x0000;
            __delay32(opoznienie);
        } else{
            LATA = 0xFFFF;
            __delay32(opoznienie);
            LATA = 0x0000;
            __delay32(opoznienie);
        
        }
        
        
    }
    
    
    return 0;
}

/*
 * File:   adc.c
 * Author: aleksandraszpakowska
 *
 * Created on April 11, 2025, 9:55 AM
 */
#include <xc.h>

#include <stdint.h>
#include <stdbool.h>

#include "adc.h"

/*********************************************************************
* Function: ADC_ReadPercentage(ADC_CHANNEL channel);
*
* Overview: Reads the requested ADC channel and returns the percentage 
*            of that conversions result (0-100%).
*
* PreCondition: channel is configured via the ADCConfigure() function
*
* Input: ADC_CHANNEL channel - enumeration of the ADC channels
*        available in this demo.  They should be meaningful names and 
*        not the names of the ADC pins on the device (as the demo code 
*        may be ported to other boards).
*         i.e. ADC_ReadPercentage(ADC_CHANNEL_POTENTIOMETER);
*
* Output: uint8_t indicating the percentage of the result 0-100% or
*         0xFF for an error
*
********************************************************************/
uint8_t ADC_ReadPercentage( ADC_CHANNEL channel )
{
    uint8_t percent;

    switch(channel)
    {
        case ADC_CHANNEL_5:
            break;
      
        case ADC_CHANNEL_4:
            break ;

        default:
            return 0xFF;
    }
    
    //A very crude percentage calculation
    percent = (ADC_Read10bit(channel) / 10);

    if(percent > 100)
    {
        percent = 100;
    }
    return percent;
}

/*********************************************************************
* Function: ADC_Read10bit(ADC_CHANNEL channel);
*
* Overview: Reads the requested ADC channel and returns the 10-bit
*           representation of this data.
*
* PreCondition: channel is configured via the ADCConfigure() function
*
* Input: ADC_CHANNEL channel - enumeration of the ADC channels
*        available in this demo.  They should be meaningful names and
*        not the names of the ADC pins on the device (as the demo code
*        may be ported to other boards).
*         i.e. - ADCReadPercentage(ADC_CHANNEL_POTENTIOMETER);
*
* Output: uint16_t the right adjusted 10-bit representation of the ADC
*         channel conversion or 0xFFFF for an error.
*
********************************************************************/
uint16_t ADC_Read10bit(ADC_CHANNEL channel)
{
    uint16_t i;
    
    switch(channel)
    {
        case ADC_CHANNEL_5:
            break;

        case ADC_CHANNEL_4:
            break ;

        default:
            return 0xFFFF;
    }

    AD1CHS = channel ;

    // Get an ADC sample
    AD1CON1bits.SAMP = 1;           //Start sampling
    for(i=0;i<1000;i++)
    {
        Nop(); //Sample delay, conversion start automatically
    } 

    AD1CON1bits.SAMP = 0;           //Start sampling
    for(i=0;i<1000;i++)
    {
        Nop(); //Sample delay, conversion start automatically
    }

    while(!AD1CON1bits.DONE);       //Wait for conversion to complete

    return ADC1BUF0;
}

/*********************************************************************
* Function: bool ADC_ChannelEnable(ADC_CHANNEL channel, ADC_CONFIGURATION configuration);
*
* Overview: Configures the ADC module to specified setting
*
* PreCondition: none
*
* Input: ADC_CHANNEL channel - the channel to enable
*        ADC_CONFIGURATION configuration - the mode in which to run the ADC
*
* Output: bool - true if successfully configured.  false otherwise.
*
********************************************************************/
bool ADC_ChannelEnable(ADC_CHANNEL channel)
{
    switch(channel)
    {
        case ADC_CHANNEL_5:
            AD1PCFGbits.PCFG5 = 0 ;
            return true;

        case ADC_CHANNEL_4:
            AD1PCFGbits.PCFG4 = 0 ;
            return true ;

        default:
            return false;
    }
    
    return false;
}

/*********************************************************************
* Function: bool ADC_SetConfiguration(ADC_CONFIGURATION configuration)
*
* Overview: Configures the ADC module to specified setting
*
* PreCondition: none
*
* Input: ADC_CONFIGURATION configuration - the mode in which to run the ADC
*
* Output: bool - true if successfully configured.  false otherwise.
*
********************************************************************/
bool ADC_SetConfiguration(ADC_CONFIGURATION configuration)
{
    if(configuration == ADC_CONFIGURATION_DEFAULT)
    {
        AD1CON2bits.VCFG = 0x0;
        AD1CON3bits.ADCS = 0xFF;
        AD1CON1bits.SSRC = 0x0;
        AD1CON3bits.SAMC = 0b10000;
        AD1CON1bits.FORM = 0b00;
        AD1CON2bits.SMPI = 0x0;
        AD1CON1bits.ADON = 1;
        return true;
    }
		
    return false;
}

/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include <xc.h>

#include <stdbool.h>

#include "buttons.h"

/*** Button Definitions *********************************************/
//      S1  is MCLR button
#define S3_PORT  PORTDbits.RD6
#define S6_PORT  PORTDbits.RD7
#define S5_PORT  PORTAbits.RA7       //Overlaps with D10
#define S4_PORT  PORTDbits.RD13

#define S3_TRIS  TRISDbits.TRISD6
#define S6_TRIS  TRISDbits.TRISD7
#define S5_TRIS  TRISAbits.TRISA7
#define S4_TRIS  TRISDbits.TRISD13

#define BUTTON_PRESSED      0
#define BUTTON_NOT_PRESSED  1

#define PIN_INPUT           1
#define PIN_OUTPUT          0

/*********************************************************************
 * Function: bool BUTTON_IsPressed(BUTTON button);
 *
 * Overview: Returns the current state of the requested button
 *
 * PreCondition: button configured via BUTTON_SetConfiguration()
 *
 * Input: BUTTON button - enumeration of the buttons available in
 *        this demo.  They should be meaningful names and not the names
 *        of the buttons on the silk-screen on the board (as the demo
 *        code may be ported to other boards).
 *         i.e. - ButtonIsPressed(BUTTON_SEND_MESSAGE);
 *
 * Output: true if pressed; false if not pressed.
 *
 ********************************************************************/
bool BUTTON_IsPressed ( BUTTON button )
{
    switch(button)
    {
        case BUTTON_S3:
            return ( (S3_PORT == BUTTON_PRESSED) ? true : false);

        case BUTTON_S6:
            return ( (S6_PORT == BUTTON_PRESSED) ? true : false);

        case BUTTON_S5:
            return ( ( S5_PORT == BUTTON_PRESSED ) ? true : false ) ;

        case BUTTON_S4:
            return ( ( S4_PORT == BUTTON_PRESSED ) ? true : false ) ;

        default:
            return false;
    }
    
    return false;
}

/*********************************************************************
* Function: void BUTTON_Enable(BUTTON button);
*
* Overview: Returns the current state of the requested button
*
* PreCondition: button configured via BUTTON_SetConfiguration()
*
* Input: BUTTON button - enumeration of the buttons available in
*        this demo.  They should be meaningful names and not the names
*        of the buttons on the silk-screen on the board (as the demo
*        code may be ported to other boards).
*         i.e. - ButtonIsPressed(BUTTON_SEND_MESSAGE);
*
* Output: None
*
********************************************************************/
void BUTTON_Enable(BUTTON button)
{
    switch(button)
    {
        case BUTTON_S3:
            S3_TRIS = PIN_INPUT ;
            break ;

        case BUTTON_S6:
            S6_TRIS = PIN_INPUT;
            break;

        case BUTTON_S5:
            S5_TRIS = PIN_INPUT ;
            break ;

        case BUTTON_S4:
            S4_TRIS = PIN_INPUT ;
            break ;

        default:
            break ;
    }
}


