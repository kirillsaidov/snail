#include "snail/canvas.h"

#include <math.h>

// expand color
#define SNL_COLOR_EXPAND(color) color.r, color.g, color.b, color.a

static bool snl_can_continue();
static void snl_rotate(const float angle, float *x1, float *y1, float *x2, float *y2);

snl_canvas_t snl_canvas_create(const float width, const float height) {
    snl_canvas_t canvas = (snl_canvas_t) {
        .width = width, 
        .height = height,
        .surface = vt_str_create_capacity(512, NULL)
    };

    // initialize the canvas
    vt_str_appendf(
        canvas.surface,
        "<svg width='%.2f' height='%.2f' viewBox='0 0 %.2f %.2f' xmlns='%s' version='%s' xmlns:xlink='%s'>\n",
        width, height, width, height, "http://www.w3.org/2000/svg", "1.1", "http://www.w3.org/1999/xlink"
    );

    // add the __default__ filter
    snl_canvas_add_filter_blur(&canvas, "__default__", 0, 0);

    return canvas;
}

void snl_canvas_destroy(snl_canvas_t *canvas) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // free string
    vt_str_destroy(canvas->surface);
}

void snl_canvas_preallocate(snl_canvas_t *const canvas, float bytes) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(bytes > 0, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // reserve
    vt_str_reserve(canvas->surface, bytes);
}

void snl_canvas_add_filter_blur(snl_canvas_t *const canvas, const char *const id, const int32_t blurnessHorizontal, const int32_t blurnessVertical) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(id != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // add filter
    vt_str_appendf(
        canvas->surface,
        "<defs><filter id='%s'><feGaussianBlur stdDeviation='%d %d'/></filter></defs>\n",
        id, blurnessHorizontal, blurnessVertical
    );
}

void snl_canvas_add_filter_blur_hard_edge(snl_canvas_t *const canvas, const char *const id, const int32_t blurnessHorizontal, const int32_t blurnessVertical) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(id != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // add filter
    vt_str_appendf(
        canvas->surface,
        "<defs><filter id='%s'>"
        "<feGaussianBlur stdDeviation='%d %d'/>"
        "<feComponentTransfer><feFuncA type='table' tableValues='1 1'/></feComponentTransfer>"
        "</filter></defs>\n",
        id, blurnessHorizontal, blurnessVertical
    );
}

void snl_canvas_add_filter_shadow(
    snl_canvas_t *const canvas, 
    const char *const id, 
    const int32_t offsetX, 
    const int32_t offsetY, 
    const int32_t blurness, 
    const bool color_blend
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(id != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // add filter
    vt_str_appendf(
        canvas->surface,
        "<defs><filter id='%s' x='0' y='0' width='200%%' height='200%%'>"
        "<feOffset result='offOut' in='%s' dx='%d' dy='%d'/>"
        "<feGaussianBlur result='blurOut' in='offOut' stdDeviation='%d'/>"
        "<feBlend in='SourceGraphic' in2='blurOut' mode='normal'/>"
        "</filter></defs>\n",
        id, color_blend ? "SourceGraphic" : "SourceAlpha", offsetX, offsetY, blurness
    );
}

void snl_canvas_add_gradient_linear(
    snl_canvas_t *const canvas, 
    const char *const id, 
    const struct SnailColor colorA, 
    const struct SnailColor colorB,
    const int32_t offsetA,
    const int32_t offsetB,
    const float opacityA,
    const float opacityB,
    const float angle
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(id != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // rotate 
    float x1 = 0, y1 = 0, x2 = 100, y2 = 0;
    snl_rotate(angle, &x1, &y1, &x2, &y2);

    // add filter
    vt_str_appendf(
        canvas->surface,
        "<defs><linearGradient id='%s' x1='%f%%' y1='%f%%' x2='%f%%' y2='%f%%'>"
        "<stop offset='%d%%' style='stop-color:rgba(%u, %u, %u, %u);stop-opacity:%f'/>"
        "<stop offset='%d%%' style='stop-color:rgba(%u, %u, %u, %u);stop-opacity:%f'/>"
        "</linearGradient></defs>\n",
        id, x1, y1, x2, y2,
        offsetA, colorA.r, colorA.g, colorA.b, colorA.a, opacityA,
        offsetB, colorB.r, colorB.g, colorB.b, colorB.a, opacityB
    );
}

void snl_canvas_add_gradient_linear_tricolor(
    snl_canvas_t *const canvas, 
    const char *const id, 
    const struct SnailColor colorA, 
    const struct SnailColor colorB,
    const struct SnailColor colorC,
    const int32_t offsetA,
    const int32_t offsetB,
    const int32_t offsetC,
    const float opacityA,
    const float opacityB,
    const float opacityC,
    const float angle
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(id != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // rotate 
    float x1 = 0, y1 = 0, x2 = 100, y2 = 0;
    snl_rotate(angle, &x1, &y1, &x2, &y2);

    // add filter
    vt_str_appendf(
        canvas->surface,
        "<defs><linearGradient id='%s' x1='%f%%' y1='%f%%' x2='%f%%' y2='%f%%'>"
        "<stop offset='%d%%' style='stop-color:rgba(%u, %u, %u, %u);stop-opacity:%f'/>"
        "<stop offset='%d%%' style='stop-color:rgba(%u, %u, %u, %u);stop-opacity:%f'/>"
        "<stop offset='%d%%' style='stop-color:rgba(%u, %u, %u, %u);stop-opacity:%f'/>"
        "</linearGradient></defs>\n",
        id, x1, y1, x2, y2,
        offsetA, colorA.r, colorA.g, colorA.b, colorA.a, opacityA,
        offsetB, colorB.r, colorB.g, colorB.b, colorB.a, opacityB,
        offsetC, colorC.r, colorC.g, colorC.b, colorC.a, opacityC
    );
}

void snl_canvas_add_gradient_radial(
    snl_canvas_t *const canvas, 
    const char *const id,
    const struct SnailColor colorA, 
    const struct SnailColor colorB,
    const int32_t offsetA,
    const int32_t offsetB,
    const float opacityA,
    const float opacityB
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(id != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // add filter
    vt_str_appendf(
        canvas->surface,
        "<defs><radialGradient id='%s' x1='50%%' y1='50%%' x2='50%%' y2='50%%'>"
        "<stop offset='%d%%' style='stop-color:rgba(%u, %u, %u, %u);stop-opacity:%f'/>"
        "<stop offset='%d%%' style='stop-color:rgba(%u, %u, %u, %u);stop-opacity:%f'/>"
        "</radialGradient></defs>\n",
        id,
        offsetA, colorA.r, colorA.g, colorA.b, colorA.a, opacityA,
        offsetB, colorB.r, colorB.g, colorB.b, colorB.a, opacityB
    );
}

void snl_canvas_add_gradient_radial_tricolor(
    snl_canvas_t *const canvas, 
    const char *const id,
    const struct SnailColor colorA, 
    const struct SnailColor colorB,
    const struct SnailColor colorC,
    const int32_t offsetA,
    const int32_t offsetB,
    const int32_t offsetC,
    const float opacityA,
    const float opacityB,
    const float opacityC
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(id != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));

    // add filter
    vt_str_appendf(
        canvas->surface,
        "<defs><radialGradient id='%s' x1='50%%' y1='50%%' x2='50%%' y2='50%%'>"
        "<stop offset='%d%%' style='stop-color:rgba(%u, %u, %u, %u);stop-opacity:%f'/>"
        "<stop offset='%d%%' style='stop-color:rgba(%u, %u, %u, %u);stop-opacity:%f'/>"
        "<stop offset='%d%%' style='stop-color:rgba(%u, %u, %u, %u);stop-opacity:%f'/>"
        "</radialGradient></defs>\n",
        id,
        offsetA, colorA.r, colorA.g, colorA.b, colorA.a, opacityA,
        offsetB, colorB.r, colorB.g, colorB.b, colorB.a, opacityB,
        offsetC, colorC.r, colorC.g, colorC.b, colorC.a, opacityC
    );
}

void snl_canvas_render_line(
    snl_canvas_t *const canvas, 
    snl_point_t start, snl_point_t end, 
    const snl_appearance_t appearance
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    // adjust for translation
    start = SNL_POINT_ADJUST(start, canvas->translateX, canvas->translateY);
    end = SNL_POINT_ADJUST(end, canvas->translateX, canvas->translateY);

    // render
    vt_str_appendf(
        canvas->surface, 
        "<line x1='%.2f' y1='%.2f' x2='%.2f' y2='%.2f' "
        "style='stroke:rgba(%u, %u, %u, %u);stroke-width:%.2f;stroke-opacity:%.2f;filter:url(#%s)'/>\n", 
        start.x, start.y, end.x, end.y,
        appearance.stroke_color.r, appearance.stroke_color.g, appearance.stroke_color.b, appearance.stroke_color.a,
        appearance.stroke_width, appearance.stroke_opacity, appearance.filter == NULL ? "__default__" : appearance.filter
    );
}

void snl_canvas_render_circle(
    snl_canvas_t *const canvas, 
    struct SnailPoint origin, const float radius, 
    const struct SnailAppearance appearance
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    // adjust for translation
    origin = SNL_POINT_ADJUST(origin, canvas->translateX, canvas->translateY);

    vt_str_appendf(
        canvas->surface,
        "<circle cx='%.2f' cy='%.2f' r='%.2f' stroke-width='%.2f' "
        "stroke='rgba(%u, %u, %u, %u)' "
        "fill-opacity='%.2f' stroke-opacity='%.2f' filter='url(#%s)' ",
        origin.x, origin.y, radius, appearance.stroke_width,
        SNL_COLOR_EXPAND(appearance.stroke_color),
        appearance.fill_opacity, appearance.stroke_opacity, appearance.filter == NULL ? "__default__" : appearance.filter
    );

    // fill color or gradient
    if(appearance.gradient) {
        vt_str_appendf(canvas->surface, "fill='url(#%s)'/>\n", appearance.gradient);
    } else {
        vt_str_appendf(canvas->surface, "fill='rgba(%u, %u, %u, %u)'/>\n", SNL_COLOR_EXPAND(appearance.fill_color));
    }
}

void snl_canvas_render_ellipse(
    snl_canvas_t *const canvas, 
    struct SnailPoint origin, const struct SnailPoint radius,
    const struct SnailAppearance appearance
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    // adjust for translation
    origin = SNL_POINT_ADJUST(origin, canvas->translateX, canvas->translateY);

    // render
    vt_str_appendf(
        canvas->surface,
        "<ellipse cx='%.2f' cy='%.2f' rx='%.2f' ry='%.2f' stroke-width='%.2f' "
        "stroke='rgba(%u, %u, %u, %u)' " 
        "fill-opacity='%.2f' stroke-opacity='%.2f' filter='url(#%s)' ",
        origin.x, origin.y, radius.x, radius.y, appearance.stroke_width,
        SNL_COLOR_EXPAND(appearance.stroke_color),
        appearance.fill_opacity, appearance.stroke_opacity, appearance.filter == NULL ? "__default__" : appearance.filter
    );
    
    // fill color or gradient
    if(appearance.gradient) {
        vt_str_appendf(canvas->surface, "fill='url(#%s)'/>\n", appearance.gradient);
    } else {
        vt_str_appendf(canvas->surface, "fill='rgba(%u, %u, %u, %u)'/>\n", SNL_COLOR_EXPAND(appearance.fill_color));
    }
}

void snl_canvas_render_rectangle(
    snl_canvas_t *const canvas, 
    snl_point_t pos, const snl_point_t size, const float radius, 
    const snl_appearance_t appearance
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    // adjust for translation
    pos = SNL_POINT_ADJUST(pos, canvas->translateX, canvas->translateY);

    // render
    vt_str_appendf(
        canvas->surface,
        "<rect x='%.2f' y='%.2f' width='%.2f' height='%.2f' rx='%.2f' ry='%.2f' stroke-width='%.2f' "
        "stroke='rgba(%u, %u, %u, %u)' fill-opacity='%.2f' stroke-opacity='%.2f' filter='url(#%s)' ",
        pos.x, pos.y, size.x, size.y, radius, radius, appearance.stroke_width,
        SNL_COLOR_EXPAND(appearance.stroke_color),
        appearance.fill_opacity, appearance.stroke_opacity, appearance.filter == NULL ? "__default__" : appearance.filter
    );

    // fill color or gradient
    if(appearance.gradient) {
        vt_str_appendf(canvas->surface, "fill='url(#%s)'/>\n", appearance.gradient);
    } else {
        vt_str_appendf(canvas->surface, "fill='rgba(%u, %u, %u, %u)'/>\n", SNL_COLOR_EXPAND(appearance.fill_color));
    }
}

void snl_canvas_render_polygon_begin(snl_canvas_t *const canvas) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget to call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    // render
    vt_str_appendf(canvas->surface, "<polygon points='");
}

void snl_canvas_render_polygon_point(snl_canvas_t *const canvas, snl_point_t point) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(!snl_can_continue(canvas), "Error: you need to 'snl_render_polygon_begin()' before using 'snl_render_polygon_point()'.\n");

    // adjust for translation
    point = SNL_POINT_ADJUST(point, canvas->translateX, canvas->translateY);

    // render
    vt_str_appendf(canvas->surface, "%.2f, %.2f ", point.x, point.y);
}

void snl_canvas_render_polygon_end(snl_canvas_t *const canvas, const snl_appearance_t appearance, const char *const fill_rule) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(!snl_can_continue(canvas), "Error: you need to 'snl_render_polygon_begin()' before using 'snl_render_polygon_end()'.\n");

    // render
    vt_str_appendf(
        canvas->surface,
        "' style='stroke:rgba(%u, %u, %u, %u);stroke-width:%.2f;fill-opacity:%.2f;stroke-opacity:%.2f;fill-rule:%s;filter:url(#%s)' ",
        SNL_COLOR_EXPAND(appearance.stroke_color),
        appearance.stroke_width, appearance.fill_opacity, appearance.stroke_opacity, fill_rule, appearance.filter == NULL ? "__default__" : appearance.filter
    );

    // fill color or gradient
    if(appearance.gradient) {
        vt_str_appendf(canvas->surface, "fill='url(#%s)'/>\n", appearance.gradient);
    } else {
        vt_str_appendf(canvas->surface, "fill='rgba(%u, %u, %u, %u)'/>\n", SNL_COLOR_EXPAND(appearance.fill_color));
    }
}

void snl_canvas_render_polyline_begin(snl_canvas_t *const canvas) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget to call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    // render
    vt_str_appendf(canvas->surface, "<polyline points='");
}

void snl_canvas_render_polyline_point(snl_canvas_t *const canvas, snl_point_t point) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(!snl_can_continue(canvas), "Error: you need to 'snl_render_polyline_begin()' before using 'snl_render_polyline_point()'.\n");

    // adjust for translation
    point = SNL_POINT_ADJUST(point, canvas->translateX, canvas->translateY);

    // render
    vt_str_appendf(canvas->surface, "%.2f, %.2f ", point.x, point.y);
}

void snl_canvas_render_polyline_end(snl_canvas_t *const canvas, const snl_appearance_t appearance) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(!snl_can_continue(canvas), "Error: you need to 'snl_render_polyline_begin()' before using 'snl_render_polyline_end()'.\n");

    // render
    vt_str_appendf(
        canvas->surface,
        "' style='stroke:rgba(%u, %u, %u, %u);stroke-width:%.2f;fill-opacity:%.2f;stroke-opacity:%.2f;filter:url(#%s)' ",
        SNL_COLOR_EXPAND(appearance.stroke_color),
        appearance.stroke_width, appearance.fill_opacity, appearance.stroke_opacity, appearance.filter == NULL ? "__default__" : appearance.filter
    );

    // fill color or gradient
    if(appearance.gradient) {
        vt_str_appendf(canvas->surface, "fill='url(#%s)'/>\n", appearance.gradient);
    } else {
        vt_str_appendf(canvas->surface, "fill='rgba(%u, %u, %u, %u)'/>\n", SNL_COLOR_EXPAND(appearance.fill_color));
    }
}

void snl_canvas_render_curve(
    snl_canvas_t *const canvas, 
    snl_point_t start, snl_point_t end, 
    const snl_appearance_t appearance
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    // adjust for translation
    start = SNL_POINT_ADJUST(start, canvas->translateX, canvas->translateY);
    end = SNL_POINT_ADJUST(end, canvas->translateX, canvas->translateY);

    // calculte curve_height and curvature
    const snl_point_t delta_end = SNL_POINT(end.x - start.x, end.y - start.y);
    const float curve_height = (delta_end.x + delta_end.y) / 2; 
    const float curvature = (delta_end.x + delta_end.y) / 2; 

    // rendering
    vt_str_appendf(
        canvas->surface,
        "<path d='M %.2f %.2f q %.2f %.2f %.2f %.2f' stroke-width='%.2f' "
        "stroke='rgba(%u, %u, %u, %u)' fill-opacity='%.2f' stroke-opacity='%.2f' filter='url(#%s)' ",
        start.x, start.y, curve_height, curvature, delta_end.x, delta_end.y, appearance.stroke_width,
        SNL_COLOR_EXPAND(appearance.stroke_color),
        appearance.fill_opacity, appearance.stroke_opacity, appearance.filter == NULL ? "__default__" : appearance.filter
    );

    // fill color or gradient
    if(appearance.gradient) {
        vt_str_appendf(canvas->surface, "fill='url(#%s)'/>\n", appearance.gradient);
    } else {
        vt_str_appendf(canvas->surface, "fill='rgba(%u, %u, %u, %u)'/>\n", SNL_COLOR_EXPAND(appearance.fill_color));
    }
}

void snl_canvas_render_curve2(
    snl_canvas_t *const canvas, 
    snl_point_t start, snl_point_t end, 
    const float curve_height, const float curvature, 
    const snl_appearance_t appearance
) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    // adjust for translation
    start = SNL_POINT_ADJUST(start, canvas->translateX, canvas->translateY);
    end = SNL_POINT_ADJUST(end, canvas->translateX, canvas->translateY);

    // calculte curve_height and curvature
    const snl_point_t delta_end = SNL_POINT(end.x - start.x, end.y - start.y);

    // rendering
    vt_str_appendf(
        canvas->surface,
        "<path d='M %.2f %.2f q %.2f %.2f %.2f %.2f' stroke-width='%.2f' "
        "stroke='rgba(%u, %u, %u, %u)' fill-opacity='%.2f' stroke-opacity='%.2f' filter='url(#%s)'/>\n",
        start.x, start.y, curve_height, curvature, delta_end.x, delta_end.y, appearance.stroke_width,
        SNL_COLOR_EXPAND(appearance.stroke_color),
        appearance.fill_opacity, appearance.stroke_opacity, appearance.filter == NULL ? "__default__" : appearance.filter
    );

    // fill color or gradient
    if(appearance.gradient) {
        vt_str_appendf(canvas->surface, "fill='url(#%s)'/>\n", appearance.gradient);
    } else {
        vt_str_appendf(canvas->surface, "fill='rgba(%u, %u, %u, %u)'/>\n", SNL_COLOR_EXPAND(appearance.fill_color));
    }
}

void snl_canvas_render_path_begin(snl_canvas_t *const canvas) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget to call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    // render
    vt_str_appendf(canvas->surface, "<polyline points='");
}

static snl_point_t gi_path_prev_point = SNL_POINT(0, 0);
void snl_canvas_render_path_line_to(snl_canvas_t *const canvas, snl_point_t point) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(!snl_can_continue(canvas), "Error: you need to 'snl_render_path_begin()' before using 'snl_render_path_line_to()'.\n");

    // adjust for translation
    point = SNL_POINT_ADJUST(point, canvas->translateX, canvas->translateY);

    // render
    vt_str_appendf(canvas->surface, "%.2f, %.2f ", point.x, point.y);

    // update previous point
    gi_path_prev_point = point;
}

void snl_canvas_render_path_move_by(snl_canvas_t *const canvas, const snl_point_t amount) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(!snl_can_continue(canvas), "Error: you need to 'snl_render_path_begin()' before using 'snl_render_path_move_by()'.\n");

    // calculate the new point which will later become our previous point
    gi_path_prev_point = SNL_POINT(gi_path_prev_point.x + amount.x, gi_path_prev_point.y + amount.y);

    // render
    vt_str_appendf(canvas->surface, "%.2f, %.2f ", gi_path_prev_point.x, gi_path_prev_point.y);
}

void snl_canvas_render_path_end(snl_canvas_t *const canvas, const snl_appearance_t appearance) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(!snl_can_continue(canvas), "Error: you need to 'snl_render_path_begin()' before using 'snl_render_path_end()'.\n");

    // render
    vt_str_appendf(
        canvas->surface,
        "' style='stroke:rgba(%u, %u, %u, %u);stroke-width:%.2f;fill-opacity:%.2f;stroke-opacity:%.2f;filter:url(#%s)' ",
        SNL_COLOR_EXPAND(appearance.stroke_color),
        appearance.stroke_width, appearance.fill_opacity, appearance.stroke_opacity, appearance.filter == NULL ? "__default__" : appearance.filter
    );

    // fill color or gradient
    if(appearance.gradient) {
        vt_str_appendf(canvas->surface, "fill='url(#%s)'/>\n", appearance.gradient);
    } else {
        vt_str_appendf(canvas->surface, "fill='rgba(%u, %u, %u, %u)'/>\n", SNL_COLOR_EXPAND(appearance.fill_color));
    }
}

// void snl_canvas_render_text(snl_canvas_t *const canvas, snl_point_t pos, const char* const text, const float font_size) {
//     // check for invalid input
//     VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
//     VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
//     VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

//     // adjust for translation
//     pos = SNL_POINT_ADJUST(pos, canvas->translateX, canvas->translateY);

//     // rendering
//     const struct SnailAppearance appearance = SNL_APPEARANCE(0, 1, SNL_COLOR_NONE, 1, SNL_COLOR_BLACK, NULL, NULL);
//     vt_str_appendf(
//         canvas->surface,
//         "<text x='%.2f' y='%.2f' font-family='%s' font-size='%.2f' font-weight='%s' font-style='%s' text-decoration='%s' stroke-width='%.2f' "
//         "stroke='rgba(%u, %u, %u, %u)' "
//         "fill-opacity='%.2f' stroke-opacity='%.2f' transform='rotate(%.2f)' filter='url(#%s)' ",
//         pos.x, pos.y, SNL_FONT_ARIAL, font_size, SNL_FONT_WEIGHT_NORMAL, SNL_FONT_STYLE_NORMAL, SNL_TEXT_NONE, appearance.stroke_width,
//         SNL_COLOR_EXPAND(appearance.stroke_color),
//         appearance.fill_opacity, appearance.stroke_opacity, 0.0, appearance.filter == NULL ? "__default__" : appearance.filter
//     );

//     // fill color or gradient
//     if(appearance.gradient) {
//         vt_str_appendf(canvas->surface, "fill='url(#%s)'>%s</text>\n", appearance.gradient, text);
//     } else {
//         vt_str_appendf(canvas->surface, "fill='rgba(%u, %u, %u, %u)'>%s</text>\n", SNL_COLOR_EXPAND(appearance.fill_color), text);
//     }
// }

void snl_canvas_render_text(snl_canvas_t *const canvas, snl_point_t pos, const char* const text, const float font_size, const char *const font_family, const struct SnailColor color) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    // adjust for translation
    pos = SNL_POINT_ADJUST(pos, canvas->translateX, canvas->translateY);

    // rendering
    const struct SnailAppearance appearance = SNL_APPEARANCE(0, 1, SNL_COLOR_NONE, 1, color, NULL, NULL);
    vt_str_appendf(
        canvas->surface,
        "<text x='%.2f' y='%.2f' font-family='%s' font-size='%.2f' font-weight='%s' font-style='%s' text-decoration='%s' stroke-width='%.2f' "
        "stroke='rgba(%u, %u, %u, %u)' "
        "fill-opacity='%.2f' stroke-opacity='%.2f' transform='rotate(%.2f)' filter='url(#%s)' ",
        pos.x, pos.y, font_family, font_size, SNL_FONT_WEIGHT_NORMAL, SNL_FONT_STYLE_NORMAL, SNL_TEXT_NONE, appearance.stroke_width,
        SNL_COLOR_EXPAND(appearance.stroke_color),
        appearance.fill_opacity, appearance.stroke_opacity, 0.0, appearance.filter == NULL ? "__default__" : appearance.filter
    );

    // fill color or gradient
    if(appearance.gradient) {
        vt_str_appendf(canvas->surface, "fill='url(#%s)'>%s</text>\n", appearance.gradient, text);
    } else {
        vt_str_appendf(canvas->surface, "fill='rgba(%u, %u, %u, %u)'>%s</text>\n", SNL_COLOR_EXPAND(appearance.fill_color), text);
    }
}

extern void snl_canvas_render_text_styled(snl_canvas_t *const canvas, snl_point_t pos, const char* const text, const snl_appearance_t appearance, snl_text_style_t text_style) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    // adjust for translation
    pos = SNL_POINT_ADJUST(pos, canvas->translateX, canvas->translateY);

    // rendering
    vt_str_appendf(
        canvas->surface,
        "<text x='%.2f' y='%.2f' font-family='%s' font-size='%.2f' font-weight='%s' font-style='%s' text-decoration='%s' stroke-width='%.2f' "
        "stroke='rgba(%u, %u, %u, %u)' "
        "fill-opacity='%.2f' stroke-opacity='%.2f' transform='rotate(%.2f)' filter='url(#%s)' ",
        pos.x, pos.y, text_style.font_family, text_style.font_size, text_style.font_weight, text_style.font_style, text_style.text_decoration, appearance.stroke_width,
        SNL_COLOR_EXPAND(appearance.stroke_color),
        appearance.fill_opacity, appearance.stroke_opacity, text_style.text_rotation, appearance.filter == NULL ? "__default__" : appearance.filter
    );

    // fill color or gradient
    if(appearance.gradient) {
        vt_str_appendf(canvas->surface, "fill='url(#%s)'>%s</text>\n", appearance.gradient, text);
    } else {
        vt_str_appendf(canvas->surface, "fill='rgba(%u, %u, %u, %u)'>%s</text>\n", SNL_COLOR_EXPAND(appearance.fill_color), text);
    }
}

void snl_canvas_undo(snl_canvas_t *const canvas) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    // undo the last operation (remove the last line)
    size_t surface_len = vt_str_len(canvas->surface) - 2;
    const char *const surface_ptr = vt_str_z(canvas->surface);
    while (surface_len-- > 0) {
        if (surface_ptr[surface_len] == '\n') {
            vt_str_remove(canvas->surface, surface_len + 1, vt_str_len(canvas->surface) - surface_len - 1);
            break;
        }
    }
}

void snl_canvas_translate(snl_canvas_t *const canvas, const float x, const float y) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    canvas->translateX = x;
    canvas->translateY = y;
}

void snl_canvas_reset_translation(snl_canvas_t *const canvas) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget call 'snl_render_xxx_end()' after 'snl_render_xxx_begin()'?\n");

    canvas->translateX = canvas->translateY = 0;
}

void snl_canvas_fill(snl_canvas_t *const canvas, struct SnailColor color) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget to call 'snl_render_xxx_end()'?\n");

    // render
    snl_canvas_render_rectangle(canvas, SNL_POINT(0, 0), SNL_POINT(canvas->width, canvas->height), 0, SNL_APPEARANCE(0, 1, SNL_COLOR_NONE, 1, color, NULL, NULL));
}

void snl_canvas_save(const snl_canvas_t *const canvas, const char *const filename) {
    // check for invalid input
    VT_DEBUG_ASSERT(canvas != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(canvas->surface != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_IS_NULL));
    VT_DEBUG_ASSERT(filename != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(snl_can_continue(canvas), "Error: did you forget to call 'snl_render_xxx_end()'?\n");

    // finalize the canvas
    vt_str_append(canvas->surface, "</svg>");

    // save
    vt_file_writeln(filename, vt_str_z(canvas->surface));
}

// ------------------------------- PRIVATE ------------------------------- //

/**
 * @brief This check is meant for situations where the user fogot to call <snl_canvas_render_xxx_end()> after <snl_canvas_render_xxx_begin()>
 * @param canvas canvas instance
 * @return bool 
 */
static bool snl_can_continue(const snl_canvas_t *const canvas) {
    const char *const surface = vt_str_z(canvas->surface);
    const size_t len = vt_str_len(canvas->surface);

    return surface[len - 1] == '\n';
}

/**
 * @brief Rotates a line around its center by angle
 * @param x1 line.startX
 * @param y1 line.startY
 * @param x2 line.endX
 * @param y2 line.endY
 * @return None 
 */
static void snl_rotate(const float angle, float *x1, float *y1, float *x2, float *y2) {
    const float rads = angle * M_PI / 180.0;
    
    // find center
    const float cx = fabs(*x2 - *x1) / 2;
    const float cy = fabs(*y2 - *y1) / 2;

    // translate coordinates (adjust by center)
    const float
        tx1 = *x1 - cx,
        ty1 = *y1 - cy,
        tx2 = *x2 - cx,
        ty2 = *y2 - cy;
    
    // rotate by angle
    const float
        nx1 = tx1 * cosf(rads) + ty1 * sinf(rads) + cx,
        ny1 = -tx1 * sinf(rads) + ty1 * cosf(rads) + cy,
        nx2 = tx2 * cosf(rads) + ty2 * sinf(rads) + cx,
        ny2 = -tx2 * sinf(rads) + ty2 * cosf(rads) + cy;

    // update values
    *x1 = nx1;
    *y1 = ny1;
    *x2 = nx2;
    *y2 = ny2;
}