#ifndef SERIAL2_H
#define SERIAL2_H

#define SERIAL2_MAX_QUEUE 50

void Serial2_init();
void Serial2_putchar(char c);
void Serial2_puts(const char* s);
unsigned char Serial2_available();
char Serial2_read();
void Serial2_gets(char* s);


#endif