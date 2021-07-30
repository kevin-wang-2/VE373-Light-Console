#include "button_scan.h"

unsigned int current_col;

void scan_next() {
    // 1. Reset previous col
    *cols[current_col] = 0;
    
    // 2. Next col
    current_col++;
    if (current_col == BUTTON_COLS) {
        current_col = 0;
    }
        
    // 3. Put up current col
    *cols[current_col] = 1;
    
    // 4. Read data, update and generate events
    unsigned int row;
    unsigned event_flag[BUTTON_ROWS];
    unsigned event_type[BUTTON_ROWS];
    for (row = 0; row < BUTTON_ROWS; row++) {
        unsigned num = current_col * BUTTON_ROWS + row;
        if (!*rows[row]) {
            if (!(scan_values[num / 8] >> (num - num / 8) & 0x1)) {
                event_flag[row] = 1;
                event_type[row] = 0;
            }
            scan_values[num / 8] |= 1 << (num - num / 8);
        } else {
            if (scan_values[num / 8] >> (num - num / 8)) {
                event_flag[row] = 1;
                event_type[row] = 1;
            }
            scan_values[num / 8] &= ~(1 << (num - num / 8));
        }
    }
    
    for (row = 0; row < BUTTON_ROWS; row++) {
        if (event_flag[row]) {
            for (unsigned num = 0; num < scan_event_num[current_col][row]; num++) {
                if (scan_events[current_col][row][num].trigger == event_type[row])
                    scan_events[current_col][row][num].cb(current_col, row);
            }
        }
    }
}

/**
 * Should register pins beforehand
 * COL: OUTPUT
 * ROW: INPUT without pullup
 */
void scan_init() {
    current_col = 0;
    createTickEvent(scan_next, 50);
}

void scan_registerCallback(unsigned int col, unsigned int row, unsigned int trigger, scan_callback_t cb) {
    scan_events[col][row][scan_event_num[current_col][row]].cb = cb;
    scan_events[col][row][scan_event_num[current_col][row]].trigger = trigger;
    scan_event_num[current_col][row]++;
}

unsigned char scan_getValue(unsigned int col, unsigned int row) {
    unsigned num = current_col * BUTTON_ROWS + row;
    return (scan_values[num / 8] >> (num % 8)) & 0x1;
}