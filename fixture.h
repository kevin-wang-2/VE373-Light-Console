#ifndef _FIXTURE_H    /* Guard against multiple inclusion */
#define _FIXTURE_H

#include "package_structure.h"
#include "dmx.h"

typedef struct {
    unsigned fixture_id;
    unsigned model_id;
    unsigned char parameters[8];
    unsigned address :9;
    unsigned selected:1;
} fixture_t;

typedef struct {
    unsigned model_id;
    struct {
        unsigned param_id;
        unsigned offset;
        unsigned valid: 1;
    } params[8];
} model_t;

model_t models[16];
fixture_t fixtures[256];

void addModel(model_package_t pack);
void addFixture(patch_package_t pack);
void setParameter(unsigned int fixture_id, param_t param_id, unsigned char value);

#endif /* _FIXTURE_H */

/* *****************************************************************************
 End of File
 */
