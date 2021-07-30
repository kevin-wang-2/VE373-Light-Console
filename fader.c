#include "fader.h"
#include "functional_utils.h"

int pid_tick;

void fader_create(unsigned int id, unsigned int *value_buffer, unsigned int *pwm_pin, unsigned int *positive_pin, unsigned int *negative_pin) {
    faders[id].id = id;
    faders[id].mode = INIT;
    faders[id].pwm_channel = PWM_CREATE_CHANNEL(pwm_pin);
    faders[id].value_buffer = value_buffer;
    faders[id].positive_pin = positive_pin;
    faders[id].negative_pin = negative_pin;
    PWM_SET_DUTY(faders[id].pwm_channel, 255);
    *faders[id].positive_pin = 1;
    *faders[id].negative_pin = 1;
}

void fader_init() {
    unsigned int i;
    for (i = 0; i < 8; i++) {
        PWM_SET_DUTY(faders[i].pwm_channel, 255);
        *faders[i].positive_pin = 1;
        *faders[i].negative_pin = 0;
    }
    createTimerEvent(fader_init_d, 1000);
}

void fader_init_d() {
    unsigned int i;
    for (i = 0; i < 8; i++) {
        faders[i].max_buffer = *faders[i].value_buffer;
        PWM_SET_DUTY(faders[i].pwm_channel, 255);
        *faders[i].positive_pin = 0;
        *faders[i].negative_pin = 1;
    }
    createTimerEvent(fader_init_s, 1000);
}

void fader_init_s() {
    unsigned int i;
    for (i = 0; i < 8; i++) {
        faders[i].min_buffer = *faders[i].value_buffer;
        PWM_SET_DUTY(faders[i].pwm_channel, 0);
        *faders[i].positive_pin = 0;
        *faders[i].negative_pin = 0;
        faders[i].mode = MANUAL;
    }
    createTickEvent(fader_controller, 5);
}

#define GET_SCALED_FADER_VALUE(i) ((*faders[i].value_buffer - faders[i].min_buffer) * 255 / (faders[i].max_buffer - faders[i].min_buffer))
#define MIN(a, b) ((a < b) ? a : b)
#define CAPPED_ABS(x) ((x > 0) ? MIN(x, 255) : MIN(-x, 255))

/**
 * PID Controller
 */
void fader_controller() {
    unsigned int i;
    for (i = 0; i < 8; i++) {
        if (faders[i].mode == AUTO) {
            int error = GET_SCALED_FADER_VALUE(i);
            if (error == 0 && (error - faders[i].last_error) == 0) {
                faders[i].mode = MANUAL;
            } else {
                int result = FADER_KP * error + FADER_KD * (error - faders[i].last_error);
                PWM_SET_DUTY(faders[i].pwm_channel, CAPPED_ABS(result));
                if (result > 0) {
                    *faders[i].positive_pin = 0;
                    *faders[i].negative_pin = 1;
                } else {
                    *faders[i].positive_pin = 1;
                    *faders[i].negative_pin = 0;
                }
                faders[i].last_error = error;
            }
        } else {
            switch (global_mode) {
                case CHANNEL:
                    if (DMX_buffer[page * 8 + i] != GET_SCALED_FADER_VALUE(i))
                        DMX_setValue(page * 8 + i, GET_SCALED_FADER_VALUE(i));
                    break;
                case FUNCTION:
                {
                    unsigned int select;
                    for (select = 0; select < selection_length; select) {
                        unsigned int channel = fixtures[fixture_selected[select]].address
                        + models[fixtures[fixture_selected[select]].model_id].params[i].offset;
                        if (DMX_buffer[channel] != GET_SCALED_FADER_VALUE(i))
                            setParameter(fixture_selected[select], i, GET_SCALED_FADER_VALUE(i));
                    }
                }
                    break;
                case CUE:
                    break;
            }
        }
    }
}