#ifndef _PACKAGE_STRUCTURE_H    /* Guard against multiple inclusion */
#define _PACKAGE_STRUCTURE_H

typedef union {
    struct {
        unsigned start:1;
        unsigned data :7;
    };
    
    char u;
} frame_t;

typedef enum {
    HI, FADER, BUTTON, MODEL, PATCH, SELECT, ERROR
} package_command_t;

typedef struct {
    unsigned fader_id:3;
    unsigned         :5;
    unsigned value   :8;
} fader_package_t;

typedef enum {
    FADER_0, FADER_1, FADER_2, FADER_3, FADER_4, FADER_5, FADER_6, FADER_7, FADER_8,
            DMX, FUNCTION, CUE, PG_DOWN, PG_UP, STORE, CLEAR, CONFIRM 
} button_t;

typedef struct {
    button_t button_id:6;
    unsigned function :1;
    unsigned          :1;
} button_package_t;

typedef enum {
    DIMMER, R, G, B, W, SHUTTER, PAN, TILT
} param_t;

typedef struct {
    param_t  param_id:3;
    unsigned model_id:4;
    unsigned offset  :8;
} model_package_t;

typedef struct {
    unsigned model_id  :4;
    unsigned address   :9;
    unsigned           :3;
    unsigned fixture_id:8;
} patch_package_t;

typedef struct {
    unsigned fixture_id:8;
} select_package_t;

typedef struct {
    unsigned value: 8;
    unsigned address: 9;
} DMX_package_t;


// Send
void packageToFrame(void *pack, void *frame, package_command_t cmd);

unsigned generateHeartbeat(void *frame);
unsigned generateFaderPackage(fader_package_t *package, void *frame);
unsigned generateButtonPackage(button_package_t *package, void *frame);
unsigned generateModelPackage(model_package_t *package, void *frame);
unsigned generatePatchPackage(patch_package_t *package, void *frame);
unsigned generateSelectPackage(select_package_t *package, void *frame);
void generateDMXPackage(DMX_package_t *package, void *frame);

// Receive
package_command_t extract_package(void *frame, void *pack);

void extractFaderPackage(fader_package_t *package, void *frame);
void extractButtonPackage(button_package_t *package, void *frame);
void extractModelPackage(model_package_t *package, void *frame);
void extractPatchPackage(patch_package_t *package, void *frame);
void extractSelectPackage(select_package_t *package, void *frame);



#endif /* _PACKAGE_STRUCTURE_H */

/* *****************************************************************************
 End of File
 */
