#include "functional_utils.h"
#include "fader.h"
#include "dmx.h"
#include "fixture.h"

void faderChange() {
    unsigned int fader;
    switch (global_mode) {
        case CHANNEL:
            for (fader = 0; fader < 8; fader++) {
                fader_set(fader, DMX_buffer[page * 8 + fader]);
            }
            break;
        case FUNCTION:
            for (fader = 0; fader < 8; fader++) {
                fader_set(fader, 0);
            }
            break;
        case CUE:
            break;
    }
}