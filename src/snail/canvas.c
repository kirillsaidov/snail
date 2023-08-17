#include "snail/canvas.h"

snl_canvas_t snl_canvas_create(const uint32_t width, const uint32_t height);

void snl_canvas_destroy(snl_canvas_t *canvas);

void snl_cavas_preallocate(snl_canvas_t *const canvas, uint32_t bytes);

void snl_canvas_render_line(
    snl_canvas_t *const canvas, 
    const snl_point_t start, const snl_point_t end, 
    const snl_appearance_t appearance
);
void snl_canvas_render_circle(
    snl_canvas_t *const canvas, 
    const struct SnailPoint origin, const uint32_t radius, 
    const struct SnailAppearance appearance
);
void snl_canvas_render_ellipse(
    snl_canvas_t *const canvas, 
    const struct SnailPoint origin, const struct SnailPoint radius,
    const struct SnailAppearance appearance
);
void snl_canvas_render_rectangle(
    snl_canvas_t *const canvas, 
    const snl_point_t pos, const snl_point_t size, const uint32_t radius, 
    const snl_appearance_t appearance
);
void snl_canvas_render_polygon_begin(snl_canvas_t *const canvas, const snl_point_t point);
void snl_canvas_render_polygon_point(snl_canvas_t *const canvas, const snl_point_t point);
void snl_canvas_render_polygon_end(snl_canvas_t *const canvas, const snl_point_t point, const snl_appearance_t appearance);
void snl_canvas_render_polyline_begin(snl_canvas_t *const canvas, const snl_point_t point);
void snl_canvas_render_polyline_point(snl_canvas_t *const canvas, const snl_point_t point);
void snl_canvas_render_polyline_end(snl_canvas_t *const canvas, const snl_point_t point, const snl_appearance_t appearance);
void snl_canvas_render_curve(
    snl_canvas_t *const canvas, 
    const snl_point_t start, const snl_point_t end, 
    const snl_appearance_t appearance
);
void snl_canvas_render_curve2(
    snl_canvas_t *const canvas, 
    const snl_point_t start, const snl_point_t end, 
    const int32_t curve_height, const int32_t curvature, 
    const snl_appearance_t appearance
);

void snl_canvas_render_path_begin(snl_canvas_t *const canvas, const snl_point_t point);
void snl_canvas_render_path_line_to(snl_canvas_t *const canvas, const snl_point_t point);

// TODO: use static previous point to save information
void snl_canvas_render_path_move_by(snl_canvas_t *const canvas, const snl_point_t amount);
void snl_canvas_render_path_end(snl_canvas_t *const canvas, const snl_point_t point, const snl_appearance_t appearance);
void snl_canvas_render_text(snl_canvas_t *const canvas, const snl_point_t pos, const char* const text, const uint32_t font_size);
void snl_canvas_render_text2(snl_canvas_t *const canvas, const snl_point_t pos, const char* const text, const uint32_t font_size, const char *const font_family);
void snl_canvas_render_text3(snl_canvas_t *const canvas, const snl_point_t pos, const char* const text, const snl_text_decoration_t td);
void snl_canvas_undo(snl_canvas_t *const canvas);
void snl_canvas_translate(snl_canvas_t *const canvas, const int32_t x, const int32_t y);
void snl_canvas_reset_translation(snl_canvas_t *const canvas);
void snl_canvas_fill(snl_canvas_t *const canvas, struct SnailColor color);
void snl_canvas_save(const snl_canvas_t *const canvas, const char *const filename);


