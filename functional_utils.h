#ifndef _FUNCTIONAL_UTILS_H    /* Guard against multiple inclusion */
#define _FUNCTIONAL_UTILS_H

typedef enum {
    CHANNEL, FUNCTION, CUE
} mode_t;

mode_t global_mode;
unsigned int page;

unsigned int property_button;

unsigned int fixture_selected[256];
unsigned int selection_length;

void faderChange();

#define MIN(a, b) ((a < b) ? a : b)
#define PAGE_CAP(page) (global_mode == CHANNEL) ? MIN(page, 63) : \
    (global_mode == FUNCTION) ? 0 : page

#define PAGE_UP page = PAGE_CAP(page + 1); \
faderChange();

#define PAGE_DOWN page = (page == 0) ? page : page - 1; \ 
faderChange();

#define PAGE_SKIP(new_page) page = PAGE_CAP(new_page); \
faderChange();

#define MODE_CHANGE(new_mode) mode = new_mode; \
page = (new_mode == FUNCTION) ? property_button : 0; \
faderChange();

#endif /* _FUNCTIONAL_UTILS_H */

/* *****************************************************************************
 End of File
 */
