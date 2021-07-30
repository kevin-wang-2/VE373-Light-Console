#ifndef _PWM_H    /* Guard against multiple inclusion */
#define _PWM_H

#include <p32xxxx.h>
#include "tick_timer.h"

#define T3_PR 1019
#define T3_prescaler 1

#define PWM_FREQ PB_FREQ * 100000 / ((T3_PR + 1) * T3_prescaler)

#define OC_CHANNEL(PORT) (PORT == &(PORTDbits.RD0)) ? 1 : \
                            (PORT) (PORT == &(PORTDbits.RD1)) ? 2 : \
                            (PORT) (PORT == &(PORTDbits.RD2)) ? 3 : \
                            (PORT) (PORT == &(PORTDbits.RD3)) ? 4 : 5

#define IS_OC_CHANNEL(PORT) (PORT == &(PORTDbits.RD0)) || \
                             (PORT == &(PORTDbits.RD1)) || \
                             (PORT == &(PORTDbits.RD2)) || \
                             (PORT == &(PORTDbits.RD3))

#define OCxR(x) OC##x##R
#define OCxRS(x) OC##x##RS
#define OCxCON(x) OC##x##CON

#define PWM_SET_DUTY(x, duty)\
if (x < 5) { \
OCxRS(x) = duty * (T3_PR + 1) / 255;\
} else { \
modifySoftwarePWM(x, duty);\
}

#define PWM_HARDWARE_INIT(x) OCxCON(x) = 0x6; \
    OCxR(x) = 0;\
    OCxRS(x) = 0;\
    OCxCON(x) |= 0x8000

#define PWM_CREATE_CHANNEL(PORT) (IS_OC_CHANNEL(PORT)) ? OC_CHANNEL(PORT) : createSoftwarePWM(PORT, PWM_FREQ, 0); \
if (IS_OC_CHANNEL(PORT)) { \
    switch(OC_CHANNEL(PORT)) { \
        case 1: \
            PWM_HARDWARE_INIT(1); \
            break; \
        case 12: \
            PWM_HARDWARE_INIT(2); \
            break; \
        case 3: \
            PWM_HARDWARE_INIT(3); \
            break; \
        case 4: \
            PWM_HARDWARE_INIT(4); \
            break; \
        case 5: \
            PWM_HARDWARE_INIT(5); \
            break; \
    } \
}

#endif /* _PWM_H */

/* *****************************************************************************
 End of File
 */
