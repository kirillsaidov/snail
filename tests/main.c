#include <stdio.h>
#include <stdlib.h>

#include "snail/snail.h"
#include "vita/core/version.h"

void draw_logo(void);
void draw_test(void);

int main(void) {
    const vt_version_t vita_version = vt_version_get();
    const vt_version_t snail_version = snl_version_get();
    
    printf("*** hello, snail! ***\n");
    printf("- Vita  version: %s\n", vita_version.str);
    printf("- Snail version: %s\n", snail_version.str);

    draw_test();
    draw_logo();
    
    return 0;
}

void draw_logo(void) {
    // create canvas
    snl_canvas_t canvas = snl_canvas_create(512, 384);
    snl_canvas_preallocate(&canvas, 2048);

    // Add gradient
    snl_canvas_add_gradient_linear(
        &canvas, "lg0", 
        SNL_COLOR(32, 183, 157, 255),   // colorA
        SNL_COLOR(152, 250, 174, 255),  // colorB
        0, 100, 100, 100, 0             // offsetA, offsetB, opacityA, opacityB, angle
    );

    // Draw background
    snl_canvas_render_rectangle(
        &canvas, 
        SNL_POINT(0, 0), 
        SNL_POINT(canvas.width, canvas.height), 
        13, 
        SNL_APPEARANCE(1, 1, SNL_COLOR_PALEWHITE, 1, SNL_COLOR_PALEWHITE, NULL, NULL)
    );

    // Draw snail body
    snl_canvas_render_rectangle(
        &canvas, 
        SNL_POINT(0.03*canvas.width, 2.055*canvas.height/3), 
        SNL_POINT(canvas.width*0.95, 0.9*canvas.height/3.2),
        81, 
        SNL_APPEARANCE(21, 1, SNL_COLOR_NONE, 0, SNL_COLOR_NONE, NULL, "lg0")
    );

    // Draw left eye
    snl_canvas_render_rectangle(
        &canvas, 
        SNL_POINT(3.5*canvas.width/5, 1.32*canvas.height/3), 
        SNL_POINT(0.07*canvas.width, 0.2*canvas.height),
        81, 
        SNL_APPEARANCE(0, 1, SNL_COLOR_NONE, 1, SNL_COLOR_NONE, NULL, "lg0")
    );

    // Draw right eye
    snl_canvas_render_rectangle(
        &canvas, 
        SNL_POINT(4*canvas.width/5, 1.32*canvas.height/3), 
        SNL_POINT(0.07*canvas.width, 0.2*canvas.height),
        81, 
        SNL_APPEARANCE(0, 1, SNL_COLOR_NONE, 1, SNL_COLOR_NONE, NULL, "lg0")
    );

    // Draw text 'made with'
    snl_canvas_render_text_styled(
        &canvas,
        SNL_POINT(0.1*canvas.width, 4.3*canvas.height/5), 
        "made with", 
        SNL_APPEARANCE(0, 1, SNL_COLOR_NONE, 1, SNL_COLOR_NONE, NULL, "lg0"),
        SNL_TEXT_STYLE(45, 0, SNL_FONT_GILL_SANS, SNL_FONT_WEIGHT_BOLD, SNL_FONT_STYLE_ITALIC, SNL_TEXT_NONE)
    );

    // Draw text 'Snail'
    snl_canvas_render_text_styled(
        &canvas,
        SNL_POINT(1.17*canvas.width/2, 4.35*canvas.height/5), 
        "Snail", 
        SNL_APPEARANCE(1, 1, SNL_COLOR_BLACK, 1, SNL_COLOR_NONE, NULL, "lg0"),
        SNL_TEXT_STYLE(60, 0, SNL_FONT_LUMINARI, SNL_FONT_WEIGHT_BOLD, SNL_FONT_STYLE_NORMAL, SNL_TEXT_NONE)
    );

    // Draw snail shell
    const float circle_radius[3] = { 2.2*canvas.height/7, 1.4*canvas.height/7, 0.8*canvas.height/7 };
    for (size_t i = 0; i < 3; i++) { 
        snl_canvas_render_circle(
            &canvas, SNL_POINT(canvas.width/3, 2.5*canvas.height/7), circle_radius[i], 
            SNL_APPEARANCE(32, i == 2 ? 0 : 1, SNL_COLOR_NONE, i == 2 ? 1 : 0, SNL_COLOR_NONE, NULL, "lg0")
        );
    }

    // Save and destroy canvas
    snl_canvas_save(&canvas, "../imgs/logo.svg");
    snl_canvas_destroy(&canvas);
}

void draw_test(void) {
    // create canvas
    snl_canvas_t canvas = snl_canvas_create(512, 512);
    snl_canvas_preallocate(&canvas, 4096);

    // add filters and gradients
    snl_canvas_add_filter_blur(&canvas, "b0", 1, 1);
    snl_canvas_add_filter_blur_hard_edge(&canvas, "b1", 1, 1);
    snl_canvas_add_filter_shadow(&canvas, "s0", 10, 10, 5, true);
    snl_canvas_add_gradient_linear(&canvas, "lg0", SNL_COLOR_BLUE, SNL_COLOR_RED, 0, 100, 1, 1, 0);
    snl_canvas_add_gradient_linear(&canvas, "lg1", SNL_COLOR_PURPLE, SNL_COLOR_ORANGE, 0, 100, 1, 1, 45);
    snl_canvas_add_gradient_linear_tricolor(&canvas, "lg2", SNL_COLOR_GOLD, SNL_COLOR_LIME, SNL_COLOR_BLUE, 0, 50, 100, 1, 1, 1, 0);
    snl_canvas_add_gradient_linear_tricolor(&canvas, "lg3", SNL_COLOR_GOLD, SNL_COLOR_LIME, SNL_COLOR_BLUE, 0, 50, 100, 1, 1, 1, 45);
    snl_canvas_add_gradient_radial(&canvas, "rg0", SNL_COLOR_CORAL, SNL_COLOR_PURPLE, 0, 100, 1, 1);
    snl_canvas_add_gradient_radial_tricolor(&canvas, "rg1", SNL_COLOR_CORAL, SNL_COLOR_PURPLE, SNL_COLOR_BLUE, 0, 50, 100, 1, 1, 1);
    

    // render
    snl_canvas_fill(&canvas, SNL_COLOR_SILVER);
    snl_canvas_render_line(&canvas, SNL_POINT(0, 0), SNL_POINT(50, 50), SNL_APPEARANCE_DEFAULT);
    snl_canvas_render_circle(&canvas, SNL_POINT(80, 30), 25, SNL_APPEARANCE_DEFAULT);
    snl_canvas_render_circle(&canvas, SNL_POINT(140, 30), 25, SNL_APPEARANCE(3, 1, SNL_COLOR_TEAL, 1, SNL_COLOR_CORAL, NULL, NULL));
    snl_canvas_render_ellipse(&canvas, SNL_POINT(220, 30), SNL_POINT(40, 25), SNL_APPEARANCE(5, 1, SNL_COLOR_PURPLE, 0.5, SNL_COLOR_DARKORANGE, NULL, NULL));
    snl_canvas_render_rectangle(&canvas, SNL_POINT(270, 10), SNL_POINT(40, 40), 0, SNL_APPEARANCE(7, 1, SNL_COLOR_GOLD, 1, SNL_COLOR_YELLOW, NULL, NULL));
    
    // polygon
    snl_canvas_render_polygon_begin(&canvas);
    snl_canvas_render_polygon_point(&canvas, SNL_POINT(320, 10));
    snl_canvas_render_polygon_point(&canvas, SNL_POINT(380, 10));
    snl_canvas_render_polygon_point(&canvas, SNL_POINT(400, 30));
    snl_canvas_render_polygon_point(&canvas, SNL_POINT(390, 50));
    snl_canvas_render_polygon_point(&canvas, SNL_POINT(340, 40));
    snl_canvas_render_polygon_end(&canvas, SNL_APPEARANCE(3, 1, SNL_COLOR_BLUE, 1, SNL_COLOR_CYAN, NULL, NULL), SNL_FILL_RULE_DEFAULT);

    // polyline
    snl_canvas_render_polyline_begin(&canvas);
    snl_canvas_render_polyline_point(&canvas, SNL_POINT(410, 10));
    snl_canvas_render_polyline_point(&canvas, SNL_POINT(470, 10));
    snl_canvas_render_polyline_point(&canvas, SNL_POINT(490, 30));
    snl_canvas_render_polyline_point(&canvas, SNL_POINT(480, 50));
    snl_canvas_render_polyline_point(&canvas, SNL_POINT(430, 40));
    snl_canvas_render_polyline_end(&canvas, SNL_APPEARANCE(7, 1, SNL_COLOR_ORANGE, 1, SNL_COLOR_NONE, NULL, "lg2"));

    // curve
    snl_canvas_render_curve(&canvas, SNL_POINT(10, 80), SNL_POINT(60, 80), SNL_APPEARANCE_DEFAULT);
    snl_canvas_render_curve(&canvas, SNL_POINT(40, 120), SNL_POINT(40, 160), SNL_APPEARANCE(3, 1, SNL_COLOR_GREEN, 1, SNL_COLOR_BLUE, NULL, NULL));
    snl_canvas_render_curve_custom(&canvas, SNL_POINT(70, 80), SNL_POINT(140, 80), 80, -40, SNL_APPEARANCE(3, 1, SNL_COLOR_TEAL, 1, SNL_COLOR_NONE, NULL, NULL));
    snl_canvas_render_curve_custom(&canvas, SNL_POINT(120, 120), SNL_POINT(120, 160), 80, 50, SNL_APPEARANCE(5, 1, SNL_COLOR_BROWN, 1, SNL_COLOR_BLACK, NULL, NULL));

    // path
    snl_canvas_render_path_begin(&canvas);
    snl_canvas_render_path_line_to(&canvas, SNL_POINT(160, 120));
    snl_canvas_render_path_line_to(&canvas, SNL_POINT(210, 130));
    snl_canvas_render_path_move_by(&canvas, SNL_POINT(10, -20));
    snl_canvas_render_path_move_by(&canvas, SNL_POINT(30, 40));
    snl_canvas_render_path_end(&canvas, SNL_APPEARANCE_DEFAULT);

    // text
    snl_canvas_render_text(&canvas, SNL_POINT(10, 195), "hello, world!", 14, SNL_FONT_LUMINARI, SNL_COLOR_CORAL);
    snl_canvas_render_text_styled(&canvas, SNL_POINT(10, 215), "hello, world!", 
        SNL_APPEARANCE(1, 1, SNL_COLOR_BISTRE, 1, SNL_COLOR_LIME, NULL, NULL), 
        SNL_TEXT_STYLE(18, 0, SNL_FONT_TREBUCHET_MS, SNL_FONT_WEIGHT_NORMAL, SNL_FONT_STYLE_ITALIC, SNL_TEXT_UNDERLINE)
    );

    // undo
    snl_canvas_render_text(&canvas, SNL_POINT(10, 250), "hello, world!", 14, SNL_FONT_LUMINARI, SNL_COLOR_CORAL);
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

    // filter: blur
    snl_canvas_render_rectangle(&canvas, SNL_POINT(10, 230), SNL_POINT(60, 60), 0, SNL_APPEARANCE(1, 1, SNL_COLOR_BLUE, 1, SNL_COLOR_LIME, "b0", NULL));
    snl_canvas_render_text_styled(&canvas, SNL_POINT(10, 310), "hello, world!", 
        SNL_APPEARANCE(1, 1, SNL_COLOR_BISTRE, 1, SNL_COLOR_LIME, "b0", NULL), 
        SNL_TEXT_STYLE(18, 0, SNL_FONT_TREBUCHET_MS, SNL_FONT_WEIGHT_NORMAL, SNL_FONT_STYLE_ITALIC, SNL_TEXT_UNDERLINE)
    );

    // filter: shadow
    snl_canvas_render_circle(&canvas, SNL_POINT(130, 250), 25, SNL_APPEARANCE(1, 1, SNL_COLOR_BISTRE, 1, SNL_COLOR_LIME, "s0", NULL));

    // gradients: linear
    snl_canvas_render_circle(&canvas, SNL_POINT(250, 250), 25, SNL_APPEARANCE(1, 1, SNL_COLOR_BISTRE, 1, SNL_COLOR_NONE, "s0", "lg0"));
    snl_canvas_render_rectangle(&canvas, SNL_POINT(10, 330), SNL_POINT(60, 60), 0, SNL_APPEARANCE(1, 1, SNL_COLOR_BLUE, 1, SNL_COLOR_NONE, NULL, "lg2"));
    snl_canvas_render_rectangle(&canvas, SNL_POINT(10, 420), SNL_POINT(60, 60), 0, SNL_APPEARANCE(1, 1, SNL_COLOR_BLUE, 1, SNL_COLOR_NONE, NULL, "rg0"));
    snl_canvas_render_circle(&canvas, SNL_POINT(350, 250), 25, SNL_APPEARANCE(1, 1, SNL_COLOR_BISTRE, 1, SNL_COLOR_NONE, "s0", "rg1"));
    snl_canvas_render_circle(&canvas, SNL_POINT(0.5*canvas.width, 0.8*canvas.height), 32, SNL_APPEARANCE(13, 1, SNL_COLOR_NONE, 0, SNL_COLOR_NONE, NULL, "lg2"));

    // for reference
    printf("- Canvas   length: %zu\n", vt_str_len(canvas.surface));
    printf("- Canvas capacity: %zu\n", vt_str_capacity(canvas.surface));

    // clear canvas
    // snl_canvas_clear(&canvas);

    // save and destroy canvas
    snl_canvas_save(&canvas, "test.svg");
    snl_canvas_destroy(&canvas);
}

