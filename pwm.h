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
                            (PORT) (PORT == &(PORTDbits.RD3)) ? 4 : \
                            5;

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

#define PWM_CREATE_CHANNEL(PORT)\
(IS_OC_CHANNEL(PORT)) ? OC_CHANNEL(PORT) : createSoftwarePWM(PORT, PWM_FREQ, 0); \
if (IS_OC_CHANNEL(PORT)) { \
OCxCON(OC_CHANNEL(PORT)) = 0x6; \
OCxR(OC_CHANNEL(PORT)) = 0; \
OCxRS(OC_CHANNEL(PORT)) = 0; \
OCxCON(OC_CHANNEL(PORT)) |= 0x8000; \
}

#endif /* _PWM_H */

/* *****************************************************************************
 End of File
 */
