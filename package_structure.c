#include "package_structure.h"

// Send
unsigned packageToFrame(void *pack, void *frame, package_command_t cmd) {
    switch (cmd) {
        case HI:
            return generateHeartbeat(frame);
        case FADER:
            return generateFaderPackage(pack, frame);
        case BUTTON:
            return generateButtonPackage(pack, frame);
        case MODEL:
            return generateModelPackage(pack, frame);
        case PATCH:
            return generatePatchPackage(pack, frame);
        case SELECT:
            return generateSelectPackage(pack, frame);
    }
}

unsigned generateHeartbeat(void *frame) {
    frame_t *ptr = frame;
    ptr->start = 1;
    ptr->data = 0;
    
    return 1;
}

unsigned generateFaderPackage(fader_package_t *package, void *frame) {
    frame_t *ptr = frame;
    
    // Header
    ptr->start = 1;
    ptr->data = 1;
    
    // Content
    (ptr + 1)->start = 0;
    (ptr + 1)->data = (package->fader_id << 4) | (package->value >> 4);
    (ptr + 2)->start = 0;
    (ptr + 2)->data = (package->value & 0xf);
    
    return 3;
}

unsigned generateButtonPackage(button_package_t *package, void *frame) {
    frame_t *ptr = frame;
    
    // Header
    ptr->start = 1;
    ptr->data = 2;
    
    // Content
    (ptr + 1)->start = 0;
    (ptr + 1)->data = (package->button_id << 1) | package->function;
    
    return 2;
}

unsigned generateModelPackage(model_package_t *package, void *frame) {
    frame_t *ptr = frame;
    
    // Header
    ptr->start = 1;
    ptr->data = 3;
    
    // Content
    (ptr + 1)->start = 0;
    (ptr + 1)->data = (package->param_id << 4) | package->model_id;
    (ptr + 2)->start = 0;
    (ptr + 2)->data = package->offset;
    
    return 3;
}

unsigned generatePatchPackage(patch_package_t *package, void *frame) {
    frame_t *ptr = frame;
    
    // Header
    ptr->start = 1;
    ptr->data = 4;
    
    // Content
    (ptr + 1)->start = 0;
    (ptr + 1)->data = (package->model_id << 3) | (package->address >> 6);
    (ptr + 2)->start = 0;
    (ptr + 2)->data = ((package->address & 0x3f) << 1) | (package->fixture_id >> 7);
    (ptr + 3)->start = 0;
    (ptr + 3)->data = (package->fixture_id & 0x7f);
    
    return 4;
}

unsigned generateSelectPackage(select_package_t *package, void *frame) {
    frame_t *ptr = frame;
    
    // Header
    ptr->start = 1;
    ptr->data = 5;
    
    // Content
    (ptr + 1)->start = 0;
    (ptr + 1)->data = package->fixture_id >> 1;
    (ptr + 2)->start = 0;
    (ptr + 2)->data = package->fixture_id & 0x1;
    
    return 3;
    
}

unsigned generateDMXPackage(DMX_package_t *package, void *frame) {
    frame_t *ptr = frame;
    
    ptr->start = 0;
    ptr->data = (package->address >> 6) & 0x07;
    (ptr + 1)->start = 0;
    (ptr + 1)->data = ((package->address & 0x3f) << 1) | ((package->value >> 7) & 0x01);
    (ptr + 2)->start = 1;
    (ptr + 2)->data = package->value & 0x7f;
    
    return 3;
}

// Receive
package_command_t extractPackage(void *frame, void *pack) {
    frame_t *ptr = frame;
    
    switch ((package_command_t)ptr->data) {
        case HI:
            break;
        case FADER:
            extractFaderPackage(ptr, ptr + 1);
            break;
        case BUTTON:
            extractButtonPackage(pack, ptr + 1);
            break;
        case MODEL:
            extractModelPackage(pack, ptr + 1);
            break;
        case PATCH:
            extractPatchPackage(pack, ptr + 1);
            break;
        case SELECT:
            extractSelectPackage(pack, ptr + 1);
            break;
    }
    
    return ptr->data;
}

unsigned int getPackageLength(unsigned char header) {
    switch((package_command_t)((frame_t *)(&header))->data) {
        case HI:
            return 1;
        case FADER:
            return 3;
        case BUTTON:
            return 2;
        case MODEL:
            return 3;
        case PATCH:
            return 4;
        case SELECT:
            return 3;
    }
}

void extractFaderPackage(fader_package_t *package, void *frame) {
    frame_t *ptr = frame;
    
    package->fader_id = ptr->data >> 4;
    package->value = ((ptr->data & 0xf) << 4) | package.value;
}

void extractButtonPackage(button_package_t *package, void *frame) {
    frame_t *ptr = frame;
    
    package->button_id = ptr->data >> 1;
    package->function = ptr->data & 0x1;
}

void extractModelPackage(model_package_t *package, void *frame) {
    frame_t *ptr = frame;
    
    package->param_id = ptr->data >> 4;
    package->model_id = ptr->data & 0xf;
    package->offset = (ptr + 1)->data;
}

void extractPatchPackage(patch_package_t *package, void *frame) {
    frame_t *ptr = frame;
    
    package->model_id = ptr->data >> 3;
    package->address = ((ptr->data & 0xf) << 6) | ((ptr + 1)->data >> 1);
    package->fixture_id = (((ptr + 1)->data & 0xf) << 7) | ((ptr + 2)->data);
}

void extractSelectPackage(select_package_t *package, void *frame) {
    frame_t *ptr = frame;
    
    package->fixture_id = ptr->data << 1 | (ptr + 1)->data;
}