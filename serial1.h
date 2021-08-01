#ifndef SERIAL1_H
#define SERIAL1_H

#include "utils.h"

#define U1_MAX_BUF 100

void Serial1_init();
void Serial1_sendPack(char *pack, unsigned int length);


#endif