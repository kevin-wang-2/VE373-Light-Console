#include "fixture.h"

void addModel(model_package_t pack) {
    models[pack.model_id].model_id = pack.model_id;
    models[pack.model_id].params[pack.param_id].param_id = pack.param_id;
    models[pack.model_id].params[pack.param_id].offset = pack.offset;
    models[pack.model_id].params[pack.param_id].valid = 1;
}

void onDMXChange(unsigned int address, unsigned char value, dmx_bind_t *bind) {
    fixtures[bind->fixture_id].parameters[bind->param_id] = value;
}

void addFixture(patch_package_t pack) {
    fixtures[pack.fixture_id].fixture_id = pack.fixture_id;
    fixtures[pack.fixture_id].model_id= pack.model_id;
    fixtures[pack.fixture_id].address = pack.address;
    unsigned int i;
    for (i = 0; i < 7; i++) {
        if (models[pack.model_id].params[i].valid) {
            CREATE_DMX_BIND(pack.address + models[pack.model_id].params[i].offset,
                    onDMXChange,
                    pack.fixture_id,
                    i
                    );
        }
    }
}

void setParameter(unsigned int fixture_id, param_t param_id, unsigned char value) {
    setDMXValue(fixtures[fixture_id].address + models[fixtures[fixture_id].model_id].params[param_id].offset,
            value);
}

