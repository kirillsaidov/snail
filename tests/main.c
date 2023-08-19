#include <stdio.h>
#include <stdlib.h>

#include "../inc/snail/snail.h"
#include "vita/core/version.h"

int main(void) {
    const vt_version_t vita_version = vt_version_get();
    const vt_version_t snail_version = snl_version_get();
    
    printf("*** hello, snail! ***\n");
    printf("- Vita  version: %s\n", vita_version.str);
    printf("- Snail version: %s\n", snail_version.str);

    snl_canvas_t *canvas = snl_canvas_create(1024, 640);
    snl_canvas_preallocate(canvas, 100);
    printf("[%s]\n", vt_str_z(canvas->surface));

    // render
    snl_canvas_render_line(canvas, SNL_POINT(0, 0), SNL_POINT(50, 50), SNL_APPEARANCE_DEFAULT);
    printf("[%s]\n", vt_str_z(canvas->surface));

    snl_canvas_save(canvas, "test.svg");
    snl_canvas_destroy(canvas);
    
    return 0;
}

