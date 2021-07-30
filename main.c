/* 
 * File:   main.c
 * Author: 22490
 *
 * Created on July 14, 2021, 7:49 PM
 */


// PIC32MX795F512L Configuration Bit Settings

// 'C' source line config statements

// DEVCFG3
#pragma config USERID = 0xFFFF          // Enter Hexadecimal value (Enter Hexadecimal value)
#pragma config FSRSSEL = PRIORITY_7     // SRS Select (SRS Priority 7)
#pragma config FMIIEN = ON              // Ethernet RMII/MII Enable (MII Enabled)
#pragma config FETHIO = ON              // Ethernet I/O Pin Select (Default Ethernet I/O)
#pragma config FCANIO = ON              // CAN I/O Pin Select (Default CAN I/O)
#pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_1         // PLL Input Divider (1x Divider)
#pragma config FPLLMUL = MUL_24         // PLL Multiplier (24x Multiplier)
#pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = ON             // Secondary Oscillator Enable (Enabled)
#pragma config IESO = ON                // Internal/External Switch Over (Enabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config FWDTEN = ON              // Watchdog Timer Enable (WDT Enabled)

// DEVCFG0
#pragma config DEBUG = OFF              // Background Debugger Enable (Debugger is disabled)
#pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (ICE EMUC2/EMUD2 pins shared with PGC2/PGD2)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>``

#include <p32xxxx.h>
#include <proc/p32mx795f512l.h>
#include "serial1.h"
#include "serial2.h"
#include <math.h>

unsigned short result[8];
unsigned int CN_source[9];
double Kp = 2.7;
double Kd = -0.7;
int default_output = 1;
__uint_32 positives[8] = {PORTGbits.RG0, PORTEbits.RE0, PORTEbits.RE2, PORTEbits.RE4,
                            PORTEbits.RE6,PORTCbits.RC1, PORTCbits.RC3, PORTFbits.RF0};
__uint_32 negatives[8] = {PORTGbits.RG1, PORTEbits.RE1, PORTEbits.RE3, PORTEbits.RE5, 
                            PORTEbits.RE7, PORTCbits.RC2, PORTCbits.RC4, PORTFbits.RF1};
unsigned short Target[8]  = {};


#pragma interrupt ADC_ISR ipl2 vector 27
void ADC_ISR(void) // interrupt every 8  samples
{
    IFS1bits.AD1IF = 0; // clear interrupt flag
}

#pragma interrupt T4_ISR ipl2 vector 16
void T4_ISR(void){
    // do nothing
}

void delay(int time){ // 1 microseconds
    PR4 = time * 8;
    IEC0bits.T4IE = 1;
    while (!IFS0bits.T4IF){}
    IEC0bits.T4IE = 0;
}

#pragma interrupt CN_ISR ipl2 vector 26
void CN_ISR(void){
    /*
     * //rows
     * RB0 = CN12 = CN_source[0]
     * RB1 = CN13 = CN_source[1]
     * RB3 = CN5 = CN_source[2]
     * RB4 = CN6 = CN_source[3]
     * //columns
     * RB5 = CN7 = CN_source[4]
     * RG6 = CN8 = CN_source[5]
     * RG7 = CN9 = CN_source[6]
     * RG8 = CN10 = CN_source[7]
     * // not used
     * RG9 = CN11 = CN_source[8]
     */
    IEC1CLR = 0x0001; //disable interrupt
    if (!PORTBbits.RB0){
        // row 1
        if (!PORTBbits.RB5){
            // column 1
            delay(100);
            if (!PORTBbits.RB0 && ! PORTBbits.RB5){
                
            }
        }
        else if (!PORTGbits.RG6){
            // column 2
            delay(100);
            if (!PORTBbits.RB0 && ! PORTGbits.RG6){
                
            }
        }
        else if (!PORTGbits.RG7){
            // column 3
            delay(100);
            if (!PORTBbits.RB0 && ! PORTGbits.RG7){
                
            }
        }
        else{
            // column 4
            delay(100);
            if (!PORTBbits.RB0 && ! PORTGbits.RG8){
                
            }
        }
    }
    else if (!PORTBbits.RB1){
        // row 2
        if (!PORTBbits.RB5){
            // column 1
            delay(100);
            if (!PORTBbits.RB1 && ! PORTBbits.RB5){
                
            }
        }
        else if (!PORTGbits.RG6){
            // column 2
            delay(100);
            if (!PORTBbits.RB1 && ! PORTGbits.RG6){
                
            }
        }
        else if (!PORTGbits.RG7){
            // column 3
            delay(100);
            if (!PORTBbits.RB1 && ! PORTGbits.RG7){
                
            }
        }
        else{
            // column 4
            delay(100);
            if (!PORTBbits.RB1 && ! PORTGbits.RG8){
                
            }
        }
    }
    else if (!PORTBbits.RB3){
        // row 3
        if (!PORTBbits.RB5){
            // column 1
            delay(100);
            if (!PORTBbits.RB3 && ! PORTBbits.RB5){
                
            }
        }
        else if (!PORTGbits.RG6){
            // column 2
            delay(100);
            if (!PORTBbits.RB3 && ! PORTGbits.RG6){
                
            }
        }
        else if (!PORTGbits.RG7){
            // column 3
            delay(100);
            if (!PORTBbits.RB3 && ! PORTGbits.RG7){
                
            }
        }
        else{
            // column 4
            delay(100);
            if (!PORTBbits.RB3 && ! PORTGbits.RG8){
                
            }
        }
    }
    else{
        // row 4
        if (!PORTBbits.RB5){
            // column 1
            delay(100);
            if (!PORTBbits.RB4 && ! PORTBbits.RB5){
                
            }
        }
        else if (!PORTGbits.RG6){
            // column 2
            delay(100);
            if (!PORTBbits.RB4 && ! PORTGbits.RG6){
                
            }
        }
        else if (!PORTGbits.RG7){
            // column 3
            delay(100);
            if (!PORTBbits.RB4 && ! PORTGbits.RG7){
                
            }
        }
        else{
            // column 4
            delay(100);
            if (!PORTBbits.RB4 && ! PORTGbits.RG8){
                
            }
        }
    }
    /*
     
    //Determine source of interrupt
    if (CN_source[0] != PORTBbits.RB0){
        CN_source[0] = PORTBbits.RB0;
        //To do: determine function
        
    }
    else if (CN_source[1] != PORTBbits.RB1){
        CN_source[1] = PORTBbits.RB1;
        //To do: determine function
        
    }
    else if (CN_source[2] != PORTBbits.RB3){
        CN_source[2] = PORTBbits.RB3;
        //To do: determine function
        
    }
    else if (CN_source[3] != PORTBbits.RB4){
        CN_source[3] = PORTBbits.RB4;
        //To do: determine function
        
    }
    else if (CN_source[4] != PORTBbits.RB5){
        CN_source[4] = PORTBbits.RB5;
        //To do: determine function
        
    }
    else if (CN_source[5] != PORTGbits.RG6){
        CN_source[5] = PORTGbits.RG6;
        //To do: determine function
        
    }
    else if (CN_source[6] != PORTGbits.RG7){
        CN_source[6] = PORTGbits.RG7;
        //To do: determine function
        
    }
    else if (CN_source[7] != PORTGbits.RG8){
        CN_source[7] = PORTGbits.RG8;
        //To do: determine function
        
    }
    else if (CN_source[8] != PORTGbits.RG9){
        CN_source[8] = PORTGbits.RG9;
        //To do: determine function
        
    }
    else{}
     */
    IFS1CLR = 0x0001; //clear interrupt flag
    IEC1SET = 0x0001; //re-enable interrupt
}

unsigned int __attribute__((always_inline)) _VirtToPhys(const void* p) {
    return (int) p < 0 ? ((int) p & 0x1fffffffL) : (unsigned int) ((unsigned char*) p + 0x40000000L);
}


void GPIO_config(){
    // RF2,3,4,5 used for serial communication
    TRISBSET = 0xffff; // open GPIO B, and set as input, used for keyboard input, with CN interrupt
    //RB0-RB5
    TRISGSET = 0x03c0; //open GPIO G, set as input (6789)
    TRISGCLR = 0x3; //output(01)
    //RG6-RG9, RG0-RG1
    TRISDCLR = 0x1f; //open GPIO D, set as output, used for PWM
    // RD0-RD4
    TRISECLR = 0xff; // open GPIO E, set as output, used for controlling bars
    // RE0-RE7
    TRISCCLR = 0x1e; // open GPIO C, set as output, used for controlling bars
    // RC1-RC4
    TRISFCLR = 0x3; // set as output, used for controlling bars
    // RF0-RF1
    
}

void CN_config(){
    asm("di");
    /* Configure CN module */
    CNCON = 0x8000; //Enable Change Notice module
    CNEN = 0x3fe0; //Enable individual CN pins (5,6,7,8,9,10,11,12,13)
    CNPUE = 0x3fe0; //Enable weak pull ups for pins CN1, CN4 and CN18
    
    // read the ports for the first time
    CN_source[0] = PORTBbits.RB0;
    CN_source[1] = PORTBbits.RB1;
    CN_source[2] = PORTBbits.RB3;
    CN_source[3] = PORTBbits.RB4;
    CN_source[4] = PORTBbits.RB5;
    CN_source[5] = PORTGbits.RG6;
    CN_source[6] = PORTGbits.RG7;
    CN_source[7] = PORTGbits.RG8;
    CN_source[8] = PORTGbits.RG9;
    
    IPC6bits.CNIP = 2; //Set priority level = 2
    IPC6bits.CNIS = 3; //Set subpriority level = 3
    IFS1bits.CNIF = 0; //Clear interrupt flag
    IEC1bits.CNIE = 1; //Enable Change Notice interrupts
    asm("ei");
}

void Timer_config(){
    // set timer3 to be 0.25ms, since sample rate = 4kHz
    T3CONSET = 0x8000;
    TMR3 = 0;
    PR3 = 250;
    // set timer 2, not enabled (enable in PWM configuration)
    T2CON = 0x10;
    TMR2 = 0;
    PR2 = 1000;
    // set timer 1
    DDPCONbits.JTAGEN = 0;
    TRISA = 0xff00;
    T1CON = 0x8000;
    PR1 = 100;
    IFS0bits.T1IF = 0;
    IPC1bits.T1IP = 2;
    IPC1bits.T1IS = 3;
    IEC0bits.T1IE = 1;
    // timer 4 configuration, used for generating delay
    T4CONSET = 0x8000;
    TMR4 = 0;
    PR4 = 800;
    IFS0bits.T4IF = 0;
    IPC4bits.T4IP = 2;
    IPC4bits.T4IS = 3;
}

void ADC_config(){
    AD1PCFG = 0xfff0; // set mode of AN0/RB0-AN3 into analog
    AD1CON1 = 0x0040; // signed fractional 16-bit, SSRC = 010 (timer 3)
    // continue to set ADC
    AD1CON2 = 0x840c; // Vrefh = AVdd, Vrefl = AVss, interrupt at forth sample, scan
    AD1CON3 = 0x0100; // Tad = 2 Tpb, use PBCLK
    AD1CSSL = 0xf;
    AD1CON1bits.ADON = 1; //turn on
    AD1CON1SET = 0x0004; // auto-sample
    // configuration complete       
	IPC6SET = 0x0a000000; //priority 2
    IFS1CLR = 0x2;
    IEC1SET = 0x2;
}

void DMA_config(){
    // DMA Setup
    IEC1CLR = 0x10000;
    IFS1CLR = 0x10000;
    DMACONSET = 0x8000;
    DCH0CON = 0x3;
    DCH0ECON = 0;
    DCH0ECONbits.CHSIRQ = 33;
    DCH0SSA = _VirtToPhys(&ADC1BUF0);
    DCH0DSA = result;
    DCH0SSIZ = 8;
    DCH0DSIZ = 8;
    DCH0CSIZ = 8;
    DCH0INTCLR = 0x00ff00ff;
    DCH0CONSET = 0x80;
}

void PWM_config(){
    OC1CON = 0x0;
    OC1RS = 0;
    OC1R = 0;
    OC2CON = 0x0;
    OC2RS = 0;
    OC2R = 0;
    OC3CON = 0x0;
    OC3RS = 0;
    OC3R = 0;
    OC4CON = 0x0;
    OC4RS = 0;
    OC4R = 0;
    OC5CON = 0x0;
    OC5RS = 0;
    OC5R = 0;
    
    T3CONbits.ON = 1; //enable timer 2
    
    OC1CONbits.ON = 1;
    OC2CONbits.ON = 1;
    OC3CONbits.ON = 1;
    OC4CONbits.ON = 1;
    OC5CONbits.ON = 1;
}

#pragma interrupt T1_ISR ipl2 vector 4
void T1_ISR(){
    // channels: RD0 - RD4
    set_PWM(PORTDbits.RD0, 0, Target[0]);
    set_PWM(PORTDbits.RD1, 1, Target[1]);
    set_PWM(PORTDbits.RD2, 2, Target[2]);
    set_PWM(PORTDbits.RD3, 3, Target[3]);
    set_PWM(PORTDbits.RD4, 4, Target[4]);
    
}

void set_PWM(__uint_32 channel_num, int num, unsigned short target){
    int last = target;
    if (abs(result[num] - target) <= 1){ // stop
        channel_num = 0;
    }
    else if (result[num] < target){ //positive
        positives[num] = 1;
        negatives[num] = 0;
    }
    else{ //negative
        positives[num] = 0;
        negatives[num] = 1;
    }
    
    if (abs(result[num] - target) > 10){
        int E = result[num] - target;
        int output = Kp * E + Kd * (E - last);
        channel_num = output;
    }
    
}



/*
 * 
 */
int main(int argc, char** argv) {
    
    SYSKEY = 0x0; //write invalid key to force lock
    SYSKEY = 0xAA996655; //write Key1 to SYSKEY
    SYSKEY = 0x556699AA; //Write Key2 to SYSKEY
    OSCCONbits.PBDIV = 0b00; //PBCLK = SYSCLK = 8MHz
    OSCCONbits.COSC = 0;
    SYSKEY = 0x0; //Write invalid key to force lock 
    asm("di"); // Disable Interrupts
    INTCONbits.MVEC = 1;
    asm("ei");
    
    GPIO_config();
    Timer_config();
    ADC_config();
    DMA_config();
    Serial1_init();
    CN_config();
    PWM_config();
    

    while (1){
        
    }
    return (EXIT_SUCCESS);
}

