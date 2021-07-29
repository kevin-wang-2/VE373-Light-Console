#include "tick_timer.h"

#define TMR_NS() (TMR1 * T1_prescaler * 1000 / (PB_FREQ * 8))
#define null 0

typedef execution_chain_t;

typedef struct {
    timer_event_t event;
    unsigned int time;
    unsigned int delay;
    unsigned int id;
    unsigned idle: 1;
    unsigned loop: 1;
    execution_chain_t *next;
} execution_chain_t;

static execution_chain_t *head;
static execution_chain_t cap[MAX_TASK_CNT];
static unsigned int first_idle;

static unsigned int isr_time;

typedef struct {
    unsigned *pin;
    unsigned int freq_cycle;
    unsigned int duty_cycle;
    unsigned int cur_cycle;
    unsigned int target_duty;
} software_pwm_t;

software_pwm_t pwm_list[MAX_TASK_CNT];
unsigned int pwm_pin_cnt;

unsigned int createTask() {
    int cur = first_idle;
    // 2. Set basic identities
    if (cur != MAX_TASK_CNT) {
        cap[cur].idle = 0;
        cap[cur].id = cur;
    }
    // 3. Find next idle
    while (first_idle != MAX_TASK_CNT && cap[first_idle].idle == 0) first_idle++;
    return cur;
}

void insertTask(unsigned int id) {
    // 0. No task in list, directly set
    if (head == null) head = &cap[id];
    // 0.1. Execute before head
    if (head->delay > cap[id].time) {
        head->delay -= cap[id].time;
        cap[id].delay = cap[id].time;
        cap[id].next = head;
        head = &cap[id];
        return;
    }
    // 1. Find proper position
    // TODO: rewrite
    execution_chain_t *ptr = head;
    int accumulated_delay = 0;
    while (ptr->next != null) {
        accumulated_delay += ptr->delay;
        if (accumulated_delay + ptr->next->delay >= cap[id].time) break; 
        ptr = ptr->next;
    }
    // 2. Change delay
    int remain_delay = ptr->next->delay;
    cap[id].delay = cap[id].time - accumulated_delay;
    ptr->next->delay = remain_delay - cap[id].delay;
    // 3. Insert
    cap[id].next = ptr->next;
    ptr->next = &cap[id];
}

void deleteTask(unsigned int id) {
    // 0. Is head
    if (first_idle > id) first_idle = id;
    if (head->id == id) {
        head->next->delay += head->delay;
        head->idle = 1;
        head = head->next;
        return;
    }
    // 1. Search task
    execution_chain_t *ptr = head, *prev;
    while (ptr->id != id && ptr != null) {
        ptr = ptr->next;
        prev = ptr;
    }
    if (ptr->null) return;
    // 2. Delete
    ptr->next->delay += ptr->delay;
    prev->next = ptr->next;
    ptr->idle = 1;
}

unsigned int createTimerEvent(timer_event_t event, unsigned int time) {
    unsigned int id = createTask();
    cap[id].event = event;
    cap[id].time = time;
    cap[id].loop = 0;
    insertTask(id);
    return id;
}

unsigned int createTickEvent(timer_event_t event, unsigned int time) {
    unsigned int id = createTask();
    cap[id].event = event;
    cap[id].time = time;
    cap[id].loop = 1;
    insertTask(id);
    return id;
}

void removeTimerEvent(int event) {
    deleteTask(event);
}

void removeTivkEvent(int event) {
    deleteTask(event);
}

unsigned int millis() {
    return (isr_time * T1_PR * T1_prescaler / PB_FREQ) / 1000;
}

unsigned int micros() {
    return isr_time * T1_PR * T1_prescaler / PB_FREQ + TMR1 * T1_prescaler / PB_FREQ;
}

void delay(unsigned int ms) {
    int curMillis = millis();
    while (millis() < curMillis + ms);
}

void delayMicroSecond(unsigned int us) {
    int curMicros = micros();
    while (micros() < curMicros + us);
}

unsigned int createSotwarePWM(volatile unsigned *pin, int frequency, int duty) {
    unsigned int cur = pwm_pin_cnt++;
    pwm_list[cur].pin = pin;
    pwm_list[cur].freq_cycle = T1_US(1000000 / frequency);
    pwm_list[cur].duty_cycle = duty * pwm_list[cur].freq_cycle / 255;
    pwm_list[cur].target_duty = pwm_list[cur].duty_cycle;
    pwm_list[cur].cur_cycle = 0;
    return cur;
}

void modifySoftwarePWM(unsigned int id, int duty) {
    pwm_list[id].target_duty = duty * pwm_list[id].freq_cycle / 255;
}

void T1_init() {
    head = null;
    first_idle = 0;
    isr_time = 0;
    
    __asm("di");
    T1CON = 0x0;
    IFS0CLR = 0x10;
    IEC0SET = 0x10;
    IPC1SET = 0x5;
    PR1 = T1_PR;
    T1CON = 0x80;
    __asm("ei");
}

#pragma interrupt T1_ISR ipl1 vector 4
void T1_ISR() {
    // 0. Clear IF
    IFS0CLR = 0x10;
    // 1. Inc isr time
    isr_time++;
    // 2. Progress event chain
    head->delay--;
    // 3. Execute event if time matches
    while (head->delay == 0) {
        execution_chain_t *cur = head;
        // 3.1 Move head
        head = head->next;
        if (cur->loop) {
        // 3.2 Loop: insert again
            insertTask(cur->id);
        } else {
        // 3.3 Idle
            if (cur->id < first_idle) first_idle = cur->id;
            cur->idle = 1;
        }
        // 3.4 Execute
        cur->event();
    }
    // 4. Software PWM
    int pin;
    for (pin = 0; pin < pwm_pin_cnt; pin++) {
        pwm_list[pin].cur_cycle++;
        if (pwm_list[pin].cur_cycle == pwm_list[pin].freq_cycle) {
            pwm_list[pin].cur_cycle = 0;
            pwm_list[pin].duty_cycle = pwm_list[pin].target_duty;
        }
        if (pwm_list[pin].cur_cycle > pwm_list[pin].duty_cycle) *pwm_list[pin].pin = 0;
        else *pwm_list[pin].pin = 1;
    }
}