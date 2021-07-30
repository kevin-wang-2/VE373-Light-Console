#ifndef _FADER_H    /* Guard against multiple inclusion */
#define _FADER_H

#include "tick_timer.h"
#include "fixture.h"
#include "dmx.h"
#include "pwm.h"

typedef enum {
    INIT, MANUAL, AUTO
} fader_mode_t;

typedef struct {
    unsigned int id;
    fader_mode_t mode;
    unsigned int *value_buffer;
    unsigned int pwm_channel;
    unsigned int *positive_pin;
    unsigned int *negative_pin;
    unsigned int target;
    unsigned int max_buffer, min_buffer;
    unsigned int last_error;
} fader_t;

fader_t faders[8];

#define FADER_KP 2.5
#define FADER_KD 0.3

void fader_create(unsigned int id, unsigned int *value_buffer, unsigned int *pwm_pin, unsigned int *positive_pin, unsigned int *negative_pin);
void fader_init();
#define fader_set(id, value) \
    faders[id].target = value; \
    faders[id].mode = AUTO;

#endif /* _FADER_H */

/* *****************************************************************************
 End of File
 */
