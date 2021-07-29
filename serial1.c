#include "serial1.h"
#include <p32xxxx.h>

struct {
    unsigned transmit_done: 1;
    unsigned available: 1;
} flags;

char send_queue[SERIAL1_MAX_QUEUE];
unsigned int send_start, send_end;
char recv_queue[SERIAL1_MAX_QUEUE];
unsigned int recv_start, recv_end;

void Serial1_init() {
    TRISFbits.TRISF2 = 0;
    TRISFbits.TRISF3 = 1;
    asm("di");
/*    
    U1MODE = 0x0; // no parity, 8-bit, 1bit stop
    U1STA = 0x5400;*/
    
    U1BRG = 6; // 9600
    U1MODEbits.PDSEL = 0b00; //8-bit, no parity
    U1MODEbits.STSEL = 0;  //1 stop bit
    U1MODEbits.UEN = 0b00;      //flow control not used
    U1STAbits.UTXEN = 1;  //enable transmitter
    IEC0bits.U1RXIE = 1;  //set receive interrupt
    IFS0bits.U1RXIF = 0;
    IPC6bits.U1IP = 2;      //set ipl2
    IPC6bits.U1IS = 0b11;  //set subpriority 3
    U1STAbits.URXISEL = 0b00; //interrupt when RXBUF is 3/4 or more full
    U1STAbits.URXEN = 1;  //enable receiver

    U1MODEbits.ON = 0;   //reset UART module
    U1MODEbits.ON = 1;   //enable UART module
    
    /*
    IFS0CLR = 0x18000000;
    IPC6SET = 0x09;
    IEC0SET = 0x18000000;
    */
    send_start = send_end = 0;
    recv_start = recv_end = 0;
    flags.transmit_done = 1;
    
    // U1MODEbits.ON = 1;
    asm("ei");
}


void Serial1_putchar(char c) {
    if (flags.transmit_done) {
        flags.transmit_done = 0;
        U1TXREG = c;
    } else {
        send_queue[send_end] = c;
        send_end++;
        if (send_end == SERIAL1_MAX_QUEUE) send_end = 0;
    }
}

void Serial1_puts(const char* s) {
    while (*s != 0) {
        Serial1_putchar(*s++);
    }
}

unsigned char Serial1_available() {
    return flags.available;
}

char Serial1_read() {
    if (flags.available) {
        unsigned int cur = recv_start;
        recv_start++;
        if (recv_start == SERIAL1_MAX_QUEUE) recv_start = 0;
        if (recv_start == recv_end) flags.available = 0;
        return recv_queue[cur];
    } else return 0;
}

void Serial1_gets(char* s) {
    if (flags.available) {
        do {
            *s++ = recv_queue[recv_start];
            recv_start++;
            if (recv_start == SERIAL1_MAX_QUEUE) recv_start = 0;
        } while (recv_start != recv_end);
        *s++ = 0;
        flags.available = 0;
    } else *s = 0;
}

//int led;

#pragma interrupt Serial1_ISR ipl2 vector 24
void Serial1_ISR() {
    if (IFS0bits.U1TXIF) { // Transmit
        IFS0bits.U1TXIF = 0;
        if (send_start == send_end) flags.transmit_done = 1;
        else {
            U1TXREG = send_queue[send_start];
            send_start++;
            if (send_start == SERIAL1_MAX_QUEUE) send_start = 0;
        }
    } else { // Receive
        IFS0bits.U1RXIF = 0;
        recv_queue[recv_end] = U1RXREG;
        recv_end++;
        if (recv_end == SERIAL1_MAX_QUEUE) recv_end = 0;
        if (recv_end == recv_start) {
            recv_start++;
            if (recv_start == SERIAL1_MAX_QUEUE) recv_start = 0;
        }
        flags.available = 1;
    }
    //PORTDbits.RD1 = led;
    //led = !led;
}

