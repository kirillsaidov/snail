#include "snail/canvas.h"

snl_canvas_t snl_canvas_create(const uint32_t width, const uint32_t height) {
    snl_canvas_t canvas = (snl_canvas_t) {
        .width = width, 
        .height = height,
        .surface = vt_str_create_capacity(VT_ARRAY_DEFAULT_INIT_ELEMENTS, NULL)
    };

    // initialize the canvas
    vt_str_appendf(
        canvas.surface,
        "<svg width='%u' height='%u' viewBox='0 0 %u %u' xmlns='%s' version='%s' xmlns:xlink='%s'>\n",
        width, height, width, height, "http://www.w3.org/2000/svg", "1.1", "http://www.w3.org/1999/xlink"
    );

    return canvas;
}

void snl_canvas_destroy(snl_canvas_t *canvas) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // free string
    vt_str_destroy(canvas->surface);
}

void snl_canvas_preallocate(snl_canvas_t *const canvas, uint32_t bytes) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(bytes > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // reserve
    vt_str_reserve(canvas->surface, bytes);
}

void snl_canvas_render_line(
    snl_canvas_t *const canvas, 
    const snl_point_t start, const snl_point_t end, 
    const snl_appearance_t appearance
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // render
    vt_str_appendf(
        canvas->surface, 
        "<line x1='%d' y1='%d' x2='%d' y2='%d' style='stroke:rgba(%u, %u, %u, %u);stroke-width:%u;stroke-opacity:%.2f'/>\n", 
        start.x, start.y, end.x, end.y,
        appearance.stroke_color.r, appearance.stroke_color.g, appearance.stroke_color.b, appearance.stroke_color.a,
        appearance.stroke_width, appearance.stroke_opacity
    );
}

void snl_canvas_render_circle(
    snl_canvas_t *const canvas, 
    const struct SnailPoint origin, const uint32_t radius, 
    const struct SnailAppearance appearance
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // render
    vt_str_appendf(
        canvas->surface,
        "<circle cx='%u' cy='%u' r='%u' stroke-width='%u' "
        "stroke='rgba(%u, %u, %u, %u)' fill='rgba(%u, %u, %u, %u)' " 
        "fill-opacity='%.2f' stroke-opacity='%.2f'/>\n",
        origin.x, origin.y, radius, appearance.stroke_width,
        appearance.stroke_color.r, appearance.stroke_color.g, appearance.stroke_color.b, appearance.stroke_color.a,
        appearance.fill_color.r, appearance.fill_color.g, appearance.fill_color.b, appearance.fill_color.a,
        appearance.fill_opacity, appearance.stroke_opacity
    );
}

void snl_canvas_render_ellipse(
    snl_canvas_t *const canvas, 
    const struct SnailPoint origin, const struct SnailPoint radius,
    const struct SnailAppearance appearance
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // render
    vt_str_appendf(
        canvas->surface,
        "<ellipse cx='%u' cy='%u' rx='%u' ry='%u' stroke-width='%u' "
        "stroke='rgba(%u, %u, %u, %u)' fill='rgba(%u, %u, %u, %u)' " 
        "fill-opacity='%.2f' stroke-opacity='%.2f'/>\n",
        origin.x, origin.y, radius.x, radius.y, appearance.stroke_width,
        appearance.stroke_color.r, appearance.stroke_color.g, appearance.stroke_color.b, appearance.stroke_color.a,
        appearance.fill_color.r, appearance.fill_color.g, appearance.fill_color.b, appearance.fill_color.a,
        appearance.fill_opacity, appearance.stroke_opacity
    );
}

void snl_canvas_render_rectangle(
    snl_canvas_t *const canvas, 
    const snl_point_t pos, const snl_point_t size, const uint32_t radius, 
    const snl_appearance_t appearance
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // render
    vt_str_appendf(
        canvas->surface,
        "<rect x='%u' y='%u' width='%u' height='%u' rx='%u' ry='%u' stroke-width='%u' "
        "stroke='rgba(%u, %u, %u, %u)' fill='rgba(%u, %u, %u, %u)' fill-opacity='%.2f' stroke-opacity='%.2f'/>\n",
        pos.x, pos.y, size.x, size.y, radius, radius, appearance.stroke_width,
        appearance.stroke_color.r, appearance.stroke_color.g, appearance.stroke_color.b, appearance.stroke_color.a,
        appearance.fill_color.r, appearance.fill_color.g, appearance.fill_color.b, appearance.fill_color.a,
        appearance.fill_opacity, appearance.stroke_opacity
    );
}

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

void snl_canvas_fill(snl_canvas_t *const canvas, struct SnailColor color) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // render
    snl_canvas_render_rectangle(canvas, SNL_POINT(0, 0), SNL_POINT(canvas->width, canvas->height), 0, SNL_APPEARANCE(0, 1, SNL_COLOR_NONE, 1, color));
}

void snl_canvas_save(const snl_canvas_t *const canvas, const char *const filename) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(filename != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // finalize the canvas
    vt_str_append(canvas->surface, "</svg>");

    // save
    vt_file_writeln(filename, vt_str_z(canvas->surface));
}


