#include "serial1.h"
#include "package_structure.h"
#include <p32xxxx.h>

struct {
    unsigned done: 2;
} U1status;

unsigned char send_buf[U1_MAX_BUF];
unsigned int send_front, send_rear;

unsigned char recv_buf[U1_MAX_BUF];
unsigned int recv_front, recv_rear;

#define PACKAGE_LENGTH ((recv_front > recv_rear) ? (recv_front - recv_rear) : (recv_front + U1_MAX_BUF - recv_rear))

unsigned int current_package_length;

void Serial1_init() {
    atomic(
        U1BRG = 415;
        U1MODE = 0x8;
        /*
         * UTXISEL = 0; When Buffer has one space
         * URXISEL = 0; When Received message
         */
        U1STA = 0x1400;
        
        IFS0CLR = 0x18000000;
        IEC0SET = 0x18000000;
        IPC6bits.U1IP = 3;
        IPC6bits.U1IS = 3;
        
        send_front = send_rear = 0;
        recv_front = recv_rear = 0;
        
        current_package_length = 0;
        
        U1status.done = 3;
        
        U1MODESET = 0x8000;
    );
}

void Serial1_sendPack(char *pack, unsigned int length) {
    unsigned int pos;
    for (unsigned int pos = 0; pos < length; pos++) {
        if (U1status.done == 0) {
            send_buf[send_front++] = pack[pos];
            if (send_front > U1_MAX_BUF) send_front = 0;
        } else {
            U1TXREG = pack[pos];
            U1status.done--; 
        }
    }
}

#pragma interrupt U1_ISR ipl3 vector 24
void U1_ISR() {
    if (IFS0bits.U1TXIF) {
        // TX
        IFS0bits.U1TXIF = 0;
        if (send_front != send_rear) {
            U1TXREG = send_buf[send_rear++];
            if (send_rear == U1_MAX_BUF) send_rear = 0;
        } else U1TXREG;
    } else {
        // RX
        IFS0bits.U1RXIF = 0;
        recv_buf[recv_front++] = U1RXREG;
        if (recv_front == U1_MAX_BUF) recv_front = 0;
        
        if (recv_buf[recv_front - 1] & 0x8) {
            // Is header
            current_package_length = getPackageLength(recv_buf[recv_front - 1]);
        } else if (PACKAGE_LENGTH == current_package_length) {
            recv_rear = recv_rear + current_package_length;
            if (recv_rear > U1_MAX_BUF) recv_rear -= U1_MAX_BUF;
        }
    }
}