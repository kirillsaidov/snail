#ifndef SNAIL_VERSION_H
#define SNAIL_VERSION_H

/** VERSION MODULE
    - snl_version_get
*/

#include "vita/core/version.h"

// defines
#define SNL_SNAIL_VERSION_MAJOR 0
#define SNL_SNAIL_VERSION_MINOR 0
#define SNL_SNAIL_VERSION_PATCH 2
#define SNL_SNAIL_VERSION VT_STRING_OF(VT_PCAT(VT_PCAT(VT_PCAT(VT_PCAT(SNL_SNAIL_VERSION_MAJOR, .), SNL_SNAIL_VERSION_MINOR), .), SNL_SNAIL_VERSION_PATCH))

/** Query Snail version
    @returns vt_version_t struct containing major, minor, patch and full version str
*/
extern vt_version_t snl_version_get(void);

#endif // SNAIL_VERSION_H
