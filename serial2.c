#include "serial2.h"
#include <p32xxxx.h>

struct {
    unsigned transmit_done: 1;
    unsigned available: 1;
} flags_2;

char send_queue_2[SERIAL2_MAX_QUEUE];
unsigned int send_start_2, send_end_2;
char recv_queue_2[SERIAL2_MAX_QUEUE];
unsigned int recv_start_2, recv_end_2;

void Serial1_init() {
    TRISFbits.TRISF4 = 0;
    TRISFbits.TRISF5 = 1;
    asm("di");
/*    
    U1MODE = 0x0; // no parity, 8-bit, 1bit stop
    U1STA = 0x5400;*/
    
    U2BRG = 6; // 9600
    U2MODEbits.PDSEL = 0b00; //8-bit, no parity
    U2MODEbits.STSEL = 0;  //1 stop bit
    U2MODEbits.UEN = 0b00;      //flow control not used
    U2STAbits.UTXEN = 1;  //enable transmitter
    IEC1bits.U3RXIE = 1;  //set receive interrupt
    IFS1bits.U3RXIF = 0;
    IPC7bits.U3IP = 2;      //set ipl2
    IPC7bits.U3IS = 0b11;  //set subpriority 3
    U2STAbits.URXISEL = 0b00; //interrupt when RXBUF is 3/4 or more full
    U2STAbits.URXEN = 1;  //enable receiver

    U2MODEbits.ON = 0;   //reset UART module
    U2MODEbits.ON = 1;   //enable UART module
    
    send_start_2 = send_end_2 = 0;
    recv_start_2 = recv_end_2 = 0;
    flags_2.transmit_done = 1;
    
    // U2MODEbits.ON = 1;
    asm("ei");
}


void Serial2_putchar(char c) {
    if (flags_2.transmit_done) {
        flags_2.transmit_done = 0;
        U2TXREG = c;
    } else {
        send_queue_2[send_end_2] = c;
        send_end_2++;
        if (send_end_2 == SERIAL2_MAX_QUEUE) send_end_2 = 0;
    }
}

void Serial2_puts(const char* s) {
    while (*s != 0) {
        Serial2_putchar(*s++);
    }
}

unsigned char Serial2_available() {
    return flags_2.available;
}

char Serial2_read() {
    if (flags_2.available) {
        unsigned int cur_2 = recv_start_2;
        recv_start_2++;
        if (recv_start_2 == SERIAL2_MAX_QUEUE) recv_start_2 = 0;
        if (recv_start_2 == recv_end_2) flags_2.available = 0;
        return recv_queue_2[cur_2];
    } else return 0;
}

void Serial2_gets(char* s) {
    if (flags_2.available) {
        do {
            *s++ = recv_queue_2[recv_start_2];
            recv_start_2++;
            if (recv_start_2 == SERIAL2_MAX_QUEUE) recv_start_2 = 0;
        } while (recv_start_2 != recv_end_2);
        *s++ = 0;
        flags_2.available = 0;
    } else *s = 0;
}

//int led_2;

#pragma interrupt Serial2_ISR ipl2 vector 31
void Serial2_ISR() {
    if (IFS1bits.U3TXIF) { // Transmit
        IFS1bits.U3TXIF = 0;
        if (send_start_2 == send_end_2) flags_2.transmit_done = 1;
        else {
            U2TXREG = send_queue_2[send_start_2];
            send_start_2++;
            if (send_start_2 == SERIAL2_MAX_QUEUE) send_start_2 = 0;
        }
    } else { // Receive
        IFS1bits.U3RXIF = 0;
        recv_queue_2[recv_end_2] = U2RXREG;
        recv_end_2++;
        if (recv_end_2 == SERIAL2_MAX_QUEUE) recv_end_2 = 0;
        if (recv_end_2 == recv_start_2) {
            recv_start_2++;
            if (recv_start_2 == SERIAL2_MAX_QUEUE) recv_start_2 = 0;
        }
        flags_2.available = 1;
    }
    //PORTDbits.RD1 = led_2;
    //led_2 = !led_2;
}

