#include "dmx.h"

void DMX_setValue(unsigned int address, unsigned char value) {
    DMX_buffer[address] = value;
    if (DMX_bind[address].bind) {
        DMX_bind[address].cb(address, value, DMX_bind + address);
    }
}

