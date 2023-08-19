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

    // create canvas
    snl_canvas_t canvas = snl_canvas_create(1024, 640);
    snl_canvas_preallocate(&canvas, 2048);

    // render
    snl_canvas_fill(&canvas, SNL_COLOR_SILVER);
    snl_canvas_render_line(&canvas, SNL_POINT(0, 0), SNL_POINT(50, 50), SNL_APPEARANCE_DEFAULT);
    snl_canvas_render_circle(&canvas, SNL_POINT(80, 30), 25, SNL_APPEARANCE_DEFAULT);
    snl_canvas_render_circle(&canvas, SNL_POINT(140, 30), 25, SNL_APPEARANCE(3, 1, SNL_COLOR_TEAL, 1, SNL_COLOR_CORAL));
    snl_canvas_render_ellipse(&canvas, SNL_POINT(220, 30), SNL_POINT(40, 25), SNL_APPEARANCE(5, 1, SNL_COLOR_PURPLE, 0.5, SNL_COLOR_DARKORANGE));
    snl_canvas_render_rectangle(&canvas, SNL_POINT(270, 10), SNL_POINT(40, 40), 0, SNL_APPEARANCE(7, 1, SNL_COLOR_GOLD, 1, SNL_COLOR_YELLOW));
    
    // for reference
    printf("- Canvas   length: %zu\n", vt_str_len(canvas.surface));
    printf("- Canvas capacity: %zu\n", vt_str_capacity(canvas.surface));

    // save and destroy canvas
    snl_canvas_save(&canvas, "test.svg");
    snl_canvas_destroy(&canvas);
    
    return 0;
}

