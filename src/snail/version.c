#include "snail/version.h"

vt_version_t snl_version_get(void) {
    vt_version_t v = {
        .major = SNL_SNAIL_VERSION_MAJOR,
        .minor = SNL_SNAIL_VERSION_MINOR,
        .patch = SNL_SNAIL_VERSION_PATCH,
        .str = SNL_SNAIL_VERSION
    };

    return v;
}

