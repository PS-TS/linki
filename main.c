/* 
 * File:   main.c
 * Author: local
 *
 * Created on March 24, 2025, 10:34 AM
 */
#include "p24FJ128GA010.h"

volatile unsigned int program = 0;
volatile unsigned char bitCounter = 0x0;

void __attribute__((interrupt)) _CNInterrupt(void)
{
    for (volatile int i = 0; i < 10000; i++);
    bitCounter = 0x0;
    LATA=0;
    if (PORTDbits.RD13 == 0) {
        program = (program + 1) % 9;
        for (volatile int i = 0; i < 10000; i++);
    } 
    else if (PORTDbits.RD6 == 0) {
        if (program == 0)
            program = 8;
        else
            program = program - 1;
        for (volatile int i = 0; i < 10000; i++);
    }

    IFS1bits.CNIF = 0;
}

int main() {
    LATA = 0;
    TRISA = 0;
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD13 = 1;
    CNEN1bits.CN15IE = 1;  // Enable CN15 (RD6)
    CNEN2bits.CN19IE = 1;  // Enable CN19 (RD13)
    IEC1bits.CNIE = 1;     // Enable CN interrupts
    IFS1bits.CNIF = 0;     // Clear CN interrupt flag
    
    while (1) {
    // Define static variables to keep state between loop iterations
        static unsigned int i = 0;
        static unsigned int j = 0;
        static unsigned int x = 0;
        static unsigned int b = 0;
        static unsigned int jedn = 0;
        static unsigned int dzies = 0;
        static unsigned int wynik = 0;
        static unsigned int seed = 115;
        static unsigned int inner_delay = 0;
        static unsigned int previousProgram = 0xFFFF;
        
        if (previousProgram != program) {
            // Reset/init variables depending on the new program
            switch (program) {
                case 0:  // Incremental up-counter
                    i = 0;
                    break;

                case 1:  // Incremental down-counter
                    i = 0x00FF;
                    break;

                case 2:  // Gray code up
                    i = 0;
                    break;

                case 3:  // Gray code down
                    i = 0x00FF;
                    break;

                case 4:  // BCD up
                    i = 0;
                    break;

                case 5:  // BCD down
                    i = 99;
                    break;

                case 6:  // Bit shift left then right
                    i = 0;
                    wynik = 7;  // initial pattern
                    break;

                case 7:  // Your custom pattern
                    i = 0;
                    j = 8;      // example initial value, adjust as needed
                    x = 1;
                    wynik = 0;
                    break;

                case 8:  // Fixed output
                    i = 0;
                    break;

                default:
                    i = 0;
                    wynik = 0xFF;
                    break;
            }

            previousProgram = program;  // update previousProgram
        }

        
        switch(program) {
            case 0:
                // Incremental up-counter
                LATA = i;
                // Simple delay (replace with timer for better accuracy)
                for (inner_delay = 0; inner_delay < 65000; inner_delay++);
                i++;
                if (i > 0x00FF) i = 0;
                break;

            case 1:
                // Incremental down-counter
                LATA = i;
                for (inner_delay = 0; inner_delay < 65000; inner_delay++);
                if (i == 0) i = 0x00FF;
                else i--;
                break;

            case 2:
                // Gray code up
                LATA = i ^ (i >> 1);
                for (inner_delay = 0; inner_delay < 65000; inner_delay++);
                i++;
                if (i > 0x00FF) i = 0;
                break;

            case 3:
                // Gray code down
                LATA = i ^ (i >> 1);
                for (inner_delay = 0; inner_delay < 65000; inner_delay++);
                if (i == 0) i = 0x00FF;
                else i--;
                break;

            case 4:
                // BCD up
                jedn = i % 10;
                dzies = i / 10;
                wynik = (dzies * 16) + jedn;
                LATA = wynik;
                for (inner_delay = 0; inner_delay < 65000; inner_delay++);
                i++;
                if (i >= 100) i = 0;
                break;

            case 5:
                // BCD down
                jedn = i % 10;
                dzies = i / 10;
                wynik = (dzies * 16) + jedn;
                LATA = wynik;
                for (inner_delay = 0; inner_delay < 65000; inner_delay++);
                if (i == 0) i = 99;
                else i--;
                break;

            case 6:
                // Bit shift left then right
                LATA = wynik;
                for (inner_delay = 0; inner_delay < 35000; inner_delay++);
                if (i < 5) {
                    wynik <<= 1;
                    i++;
                } else if (i < 9) {
                    wynik >>= 1;
                    i++;
                } else {
                    wynik = 7;
                    i = 0;
                }
                break;

            case 7:

                LATA = wynik;
                for (inner_delay = 0; inner_delay < 65000; inner_delay++);
                if (j > 0) {
                    if (i < j) {
                        wynik += x;
                        if(i!=0)
                            x=x*2;
                        i=i+1;
                    }
                    else if(i==j){
                        x=1;
                        j=j-1;
                        i=0;
                    }
                }
                else{
                    j=8;
                    wynik = 0;
                    x=1;
                }
                break;

            case 8:
                seed = (1103515245 * seed + 12345) & 0x7fffffff;
                LATA = seed % 64;
                for (inner_delay = 0; inner_delay < 65000; inner_delay++);
                for (inner_delay = 0; inner_delay < 65000; inner_delay++);
                for (inner_delay = 0; inner_delay < 65000; inner_delay++);
                break;

            default:
                LATA = 255;
                i = 0; // reset state variables
                break;
        }

    }

    return 0;
}
