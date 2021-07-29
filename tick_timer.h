#ifndef TICK_TIMER_H
#define TICK_TIMER_H

#include <p32xxxx.h>

#define PB_FREQ 24
#define T1_PR 800
#define T1_prescaler 1
#define MAX_TASK_CNT 100

#define T1_US(x) (x * PB_FREQ / T1_prescaler / T1_PR)
#define T1_MS(x) (x * PB_FREQ * 1000 / T1_prescaler / T1_PR)

typedef void (*timer_event_t)();

void T1_init();

// Event Driven
unsigned int createTimerEvent(timer_event_t event, unsigned int time);
unsigned int createTickEvent(timer_event_t event, unsigned int time);
void removeTimerEvent(int event);
void removeTivkEvent(int event);

// Timing Values
unsigned int millis();
unsigned int micros();

// Delays
void delay(unsigned int ms);
void delayMicroSecond(unsigned int us);

// Functionalities
unsigned int createSotwarePWM(volatile unsigned *pin, int frequency, int duty);
void modifySoftwarePWM(unsigned int id, int duty);

#endif