#include <stdio.h>
#include <stdlib.h>

#include "../inc/snail/snail.h"
#include "../third_party/vita/vita.h"

int main(void) {
    const vt_version_t vita_version = vt_version_get();
    const vt_version_t snail_version = snl_version_get();
    
    printf("*** hello, snail! ***\n");
    printf("- Vita  version: %s\n", vita_version.str);
    printf("- Snail version: %s\n", snail_version.str);
    
    return 0;
}

