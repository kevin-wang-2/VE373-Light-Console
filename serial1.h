#ifndef SERIAL1_H
#define SERIAL1_H

#define SERIAL1_MAX_QUEUE 50

void Serial1_init();
void Serial1_putchar(char c);
void Serial1_puts(const char* s);
unsigned char Serial1_available();
char Serial1_read();
void Serial1_gets(char* s);


#endif