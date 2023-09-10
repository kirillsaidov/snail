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
    snl_canvas_t canvas = snl_canvas_create(512, 512);
    snl_canvas_preallocate(&canvas, 3072);

    // render
    snl_canvas_fill(&canvas, SNL_COLOR_SILVER);
    snl_canvas_render_line(&canvas, SNL_POINT(0, 0), SNL_POINT(50, 50), SNL_APPEARANCE_DEFAULT);
    snl_canvas_render_circle(&canvas, SNL_POINT(80, 30), 25, SNL_APPEARANCE_DEFAULT);
    snl_canvas_render_circle(&canvas, SNL_POINT(140, 30), 25, SNL_APPEARANCE(3, 1, SNL_COLOR_TEAL, 1, SNL_COLOR_CORAL));
    snl_canvas_render_ellipse(&canvas, SNL_POINT(220, 30), SNL_POINT(40, 25), SNL_APPEARANCE(5, 1, SNL_COLOR_PURPLE, 0.5, SNL_COLOR_DARKORANGE));
    snl_canvas_render_rectangle(&canvas, SNL_POINT(270, 10), SNL_POINT(40, 40), 0, SNL_APPEARANCE(7, 1, SNL_COLOR_GOLD, 1, SNL_COLOR_YELLOW));
    
    // polygon
    snl_canvas_render_polygon_begin(&canvas);
    snl_canvas_render_polygon_point(&canvas, SNL_POINT(320, 10));
    snl_canvas_render_polygon_point(&canvas, SNL_POINT(380, 10));
    snl_canvas_render_polygon_point(&canvas, SNL_POINT(400, 30));
    snl_canvas_render_polygon_point(&canvas, SNL_POINT(390, 50));
    snl_canvas_render_polygon_point(&canvas, SNL_POINT(340, 40));
    snl_canvas_render_polygon_end(&canvas, SNL_APPEARANCE(3, 1, SNL_COLOR_BLUE, 1, SNL_COLOR_CYAN), SNL_FILL_RULE_DEFAULT);

    // polyline
    snl_canvas_render_polyline_begin(&canvas);
    snl_canvas_render_polyline_point(&canvas, SNL_POINT(410, 10));
    snl_canvas_render_polyline_point(&canvas, SNL_POINT(470, 10));
    snl_canvas_render_polyline_point(&canvas, SNL_POINT(490, 30));
    snl_canvas_render_polyline_point(&canvas, SNL_POINT(480, 50));
    snl_canvas_render_polyline_point(&canvas, SNL_POINT(430, 40));
    snl_canvas_render_polyline_end(&canvas, SNL_APPEARANCE(7, 1, SNL_COLOR_ORANGE, 1, SNL_COLOR_NONE));

    // curve
    snl_canvas_render_curve(&canvas, SNL_POINT(10, 80), SNL_POINT(60, 80), SNL_APPEARANCE_DEFAULT);
    snl_canvas_render_curve(&canvas, SNL_POINT(40, 120), SNL_POINT(40, 160), SNL_APPEARANCE(3, 1, SNL_COLOR_GREEN, 1, SNL_COLOR_BLUE));
    snl_canvas_render_curve2(&canvas, SNL_POINT(70, 80), SNL_POINT(140, 80), 80, -40, SNL_APPEARANCE(3, 1, SNL_COLOR_TEAL, 1, SNL_COLOR_NONE));
    snl_canvas_render_curve2(&canvas, SNL_POINT(120, 120), SNL_POINT(120, 160), 80, 50, SNL_APPEARANCE(5, 1, SNL_COLOR_BROWN, 1, SNL_COLOR_NONE));

    // path
    snl_canvas_render_path_begin(&canvas);
    snl_canvas_render_path_line_to(&canvas, SNL_POINT(160, 120));
    snl_canvas_render_path_line_to(&canvas, SNL_POINT(210, 130));
    snl_canvas_render_path_move_by(&canvas, SNL_POINT(10, -20));
    snl_canvas_render_path_move_by(&canvas, SNL_POINT(30, 40));
    snl_canvas_render_path_end(&canvas, SNL_APPEARANCE_DEFAULT);

    // text
    snl_canvas_render_text(&canvas, SNL_POINT(10, 180), "hello, world!", 14);
    snl_canvas_render_text2(&canvas, SNL_POINT(10, 195), "hello, world!", 14, SNL_FONT_LUMINARI, SNL_COLOR_CORAL);
    snl_canvas_render_text3(&canvas, SNL_POINT(10, 215), "hello, world!", 
        SNL_APPEARANCE(1, 1, SNL_COLOR_BISTRE, 1, SNL_COLOR_LIME), 
        SNL_TEXT_DECORATION(18, 0, SNL_FONT_TREBUCHET_MS, SNL_FONT_WEIGHT_NORMAL, SNL_FONT_STYLE_ITALIC, SNL_TEXT_UNDERLINE)
    );

    // undo
    snl_canvas_render_text(&canvas, SNL_POINT(10, 250), "UNDO THIS!", 18);
    snl_canvas_undo(&canvas);

    // translate
    snl_canvas_translate(&canvas, canvas.width / 2, canvas.height / 2);
    snl_canvas_render_rectangle(&canvas, SNL_POINT(-50, -50), SNL_POINT(100, 100), 0, SNL_APPEARANCE_DEFAULT);
    snl_canvas_render_rectangle(&canvas, SNL_POINT(-40, -40), SNL_POINT(80, 80), 0, SNL_APPEARANCE_DEFAULT);
    snl_canvas_render_rectangle(&canvas, SNL_POINT(-30, -30), SNL_POINT(60, 60), 0, SNL_APPEARANCE_DEFAULT);
    snl_canvas_translate(&canvas, canvas.width / 1.5, canvas.height / 1.5);
    snl_canvas_render_rectangle(&canvas, SNL_POINT(-30, -30), SNL_POINT(60, 60), 0, SNL_APPEARANCE_DEFAULT);

    // reset translation
    snl_canvas_reset_translation(&canvas);
    snl_canvas_render_rectangle(&canvas, SNL_POINT(-30, -30), SNL_POINT(60, 60), 0, SNL_APPEARANCE_DEFAULT);


    // for reference
    printf("- Canvas   length: %zu\n", vt_str_len(canvas.surface));
    printf("- Canvas capacity: %zu\n", vt_str_capacity(canvas.surface));

    // save and destroy canvas
    snl_canvas_save(&canvas, "test.svg");
    snl_canvas_destroy(&canvas);
    
    return 0;
}

