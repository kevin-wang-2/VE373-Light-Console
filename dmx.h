#ifndef _DMX_H    /* Guard against multiple inclusion */
#define _DMX_H

#include "package_structure.h"

typedef void (*dmx_callback_t)(unsigned int address, unsigned char value, dmx_bind_t *bind);

typedef struct {
    dmx_callback_t cb;
    unsigned fixture_id;
    unsigned param_id:3;
    unsigned bind:1;
} dmx_bind_t;

unsigned char DMX_buffer[512];
dmx_bind_t DMX_bind[512];

void setDMXValue(unsigned int address, unsigned char value);

#define CREATE_DMX_BIND(address, cb, fixture_id, param_id) DMX_bind[address].bind = 1;\
    DMX_bind[address].cb = cb;\
    DMX_bind[address].fixture_id = fixture_id;\
    DMX_bind[address].param_id = param_id

#endif /* _DMX_H */

/* *****************************************************************************
 End of File
 */
