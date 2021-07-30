#ifndef _BUTTON_SCAN_H    /* Guard against multiple inclusion */
#define _BUTTON_SCAN_H

#include "tick_timer.h"

#define BUTTON_COLS 4
#define BUTTON_ROWS 4

unsigned int *cols[BUTTON_COLS];
unsigned int *rows[BUTTON_ROWS];

typedef void (*scan_callback_t)(unsigned int col, unsigned int row);

#define SCAN_KEYDOWN 0
#define SCAN_KEYUP 1

typedef struct {
    scan_callback_t cb;
    unsigned trigger:1;
} scan_event_t;

#define SCAN_MAX_CALLBACK 10

scan_event_t scan_events[BUTTON_COLS][BUTTON_ROWS][SCAN_MAX_CALLBACK];
unsigned int scan_event_num[BUTTON_COLS][BUTTON_ROWS];
unsigned char scan_values[BUTTON_COLS * BUTTON_ROWS / 8];

void scan_init();
void scan_registerCallback(unsigned int col, unsigned int row, unsigned int trigger, scan_callback_t cb);
unsigned char scan_getValue(unsigned int col, unsigned int row);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
