#ifndef SNAIL_CANVAS_H
#define SNAIL_CANVAS_H

/** CANVAS MODULE
 *  - snl_canvas_create
 *  - snl_canvas_destroy
 *  - snl_canvas_render_line
 *  - snl_canvas_render_circle
 *  - snl_canvas_render_ellipse
 *  - snl_canvas_render_rectangle
 *  - snl_canvas_render_polygon_begin
 *  - snl_canvas_render_polygon_point
 *  - snl_canvas_render_polygon_end
 *  - snl_canvas_render_polyline_begin
 *  - snl_canvas_render_polyline_point
 *  - snl_canvas_render_polyline_end
 *  - snl_canvas_render_curve
 *  - snl_canvas_render_curve_custom
 *  - snl_canvas_render_path_begin
 *  - snl_canvas_render_path_line_to
 *  - snl_canvas_render_path_move_by
 *  - snl_canvas_render_path_end
 *  - snl_canvas_render_text
 *  - snl_canvas_render_text_styled
 *  - snl_canvas_undo
 *  - snl_canvas_clear
 *  - snl_canvas_translate
 *  - snl_canvas_reset_translation
 *  - snl_canvas_fill
 *  - snl_canvas_save
*/

#include <stdint.h>
#include "vita/container/str.h"
#include "vita/system/fileio.h"

// RGBA color structure
struct SnailColor {
    uint8_t r, g, b, a;
};

// RGBA color code
#define SNL_COLOR(r, g, b, a) ((struct SnailColor) {r, g, b, a})
#define SNL_COLOR_BLACK         SNL_COLOR(  0,   0,   0, 255)
#define SNL_COLOR_WHITE         SNL_COLOR(255, 255, 255, 255)
#define SNL_COLOR_PALEWHITE     SNL_COLOR(251, 251, 253, 255)
#define SNL_COLOR_RED           SNL_COLOR(255,   0,   0, 255)
#define SNL_COLOR_DARKRED       SNL_COLOR(139,   0,   0, 255)
#define SNL_COLOR_LIME          SNL_COLOR(  0, 255,   0, 255)
#define SNL_COLOR_BLUE          SNL_COLOR(  0,   0, 255, 255)
#define SNL_COLOR_DARKBLUE      SNL_COLOR(  0,   0, 139, 255)
#define SNL_COLOR_GOLD          SNL_COLOR(255, 215,   0, 255)
#define SNL_COLOR_YELLOW        SNL_COLOR(255, 255,   0, 255)
#define SNL_COLOR_DARKYELLOW    SNL_COLOR(246, 190,   0, 255)
#define SNL_COLOR_ORANGE        SNL_COLOR(255, 165,   0, 255)
#define SNL_COLOR_DARKORANGE    SNL_COLOR(255, 140,   0, 255)
#define SNL_COLOR_CORAL         SNL_COLOR(255, 127,  80, 255)
#define SNL_COLOR_TOMATO        SNL_COLOR(255,  99,  71, 255)
#define SNL_COLOR_CYAN          SNL_COLOR(  0, 255, 255, 255)
#define SNL_COLOR_MAGENTA       SNL_COLOR(255,   0, 255, 255)
#define SNL_COLOR_SILVER        SNL_COLOR(192, 192, 192, 255)
#define SNL_COLOR_GRAY          SNL_COLOR(128, 128, 128, 255)
#define SNL_COLOR_MAROON        SNL_COLOR(128,   0,   0, 255)
#define SNL_COLOR_OLIVE         SNL_COLOR(128, 128,   0, 255)
#define SNL_COLOR_GREEN         SNL_COLOR(  0, 128,   0, 255)
#define SNL_COLOR_DARKGREEN     SNL_COLOR(  1,  50,  32, 255)
#define SNL_COLOR_PURPLE        SNL_COLOR(128,   0, 128, 255)
#define SNL_COLOR_TEAL          SNL_COLOR(  0, 128, 128, 255)
#define SNL_COLOR_NAVY          SNL_COLOR(  0,   0, 128, 255)
#define SNL_COLOR_BROWN         SNL_COLOR(150,  75,   0, 255)
#define SNL_COLOR_DARKBROWN     SNL_COLOR( 89,  60,  30, 255)
#define SNL_COLOR_HARRISA       SNL_COLOR( 92,  44,  38, 255)
#define SNL_COLOR_CAFE          SNL_COLOR( 84,  56,  28, 255)
#define SNL_COLOR_BISTRE        SNL_COLOR( 42,  28,  14, 255)
#define SNL_COLOR_CHAMBRAY      SNL_COLOR( 71,  88, 119, 255)
#define SNL_COLOR_NONE          SNL_COLOR(  0,   0,   0,   0)

// appearance configuration
// @note {stroke, fill} color will always be prioritized over gradient, unless `SNL_COLOR_NONE` is specified 
typedef struct SnailAppearance {
    // stroke
    float stroke_width;
    float stroke_opacity;
    struct SnailColor stroke_color;

    // fill
    float fill_opacity;
    struct SnailColor fill_color;

    // filter, gradient
    const char *filter;
    const char *gradient; 
} snl_appearance_t;

// stroke_width, stroke_opacity, stroke_color, fill_opacity, fill_color, filter_id_str, gradient_id_str
#define SNL_APPEARANCE(sw, so, sc, fo, fc, fis, gis) ((snl_appearance_t) {sw, so, sc, fo, fc, fis, gis})
#define SNL_APPEARANCE_DEFAULT SNL_APPEARANCE(1, 1, SNL_COLOR_BLACK, 1, SNL_COLOR_NONE, NULL, NULL)

// websafe fonts
#define SNL_FONT_ARIAL           "arial" 
#define SNL_FONT_ARIAL_BLACK     "arial black" 
#define SNL_FONT_HEVETICA        "hevetica"
#define SNL_FONT_VERDANA         "verdana"
#define SNL_FONT_TAHOMA          "tahoma"
#define SNL_FONT_TREBUCHET_MS    "trebuchet ms"
#define SNL_FONT_IMPACT          "impact"
#define SNL_FONT_GILL_SANS       "gill sans"
#define SNL_FONT_TIMES_NEW_ROMAN "times new roman"
#define SNL_FONT_GEORGIA         "georgia"
#define SNL_FONT_PALATINO        "palatino"
#define SNL_FONT_BASKERVILLE     "baskerville"
#define SNL_FONT_COURIER         "courier"
#define SNL_FONT_MONACO          "monaco"
#define SNL_FONT_LUMINARI        "luminari"
#define SNL_FONT_COMIC_SANS_MS   "comic sans ms"

// available font weight
#define SNL_FONT_WEIGHT_NORMAL "normal"
#define SNL_FONT_WEIGHT_BOLD   "bold"

// available font styles
#define SNL_FONT_STYLE_NORMAL "normal"
#define SNL_FONT_STYLE_ITALIC "italic"

// available text decoration styles
#define SNL_TEXT_UNDERLINE "underline"
#define SNL_TEXT_OVERLINE "overline"
#define SNL_TEXT_LINE_THROUGH "line-through"
#define SNL_TEXT_NONE ""

// fill rule for polygon
#define SNL_FILL_RULE_NONZERO "nonzero"
#define SNL_FILL_RULE_EVENODD "evenodd"
#define SNL_FILL_RULE_DEFAULT SNL_FILL_RULE_NONZERO

// font decoration
typedef struct SnailTextStyle {
    float font_size;
    float text_rotation;
    const char *font_family;
    const char *font_weight;
    const char *font_style;
    const char *text_decoration;
} snl_text_style_t;

// font_size, text_rotation, font_family, font_weight, font_style, text_decoration
#define SNL_TEXT_STYLE(fsz, tr, ff, fw, fs, td) ((snl_text_style_t) {fsz, tr, ff, fw, fs, td})
#define SNL_TEXT_STYLE_DEFAULT(fsz, tr, ff, fw, fs, td) SNL_TEXT_STYLE(10, 0, SNL_FONT_ARIAL, SNL_FONT_WEIGHT_NORMAL, SNL_FONT_STYLE_NORMAL, SNL_TEXT_NONE)

// point(x, y)
typedef struct SnailPoint {
    float x, y;
} snl_point_t;

// set point easily
#define SNL_POINT(x, y) ((snl_point_t) {x, y})

// adjust point by value
#define SNL_POINT_ADJUST(point, adjust_x, adjust_y) ((snl_point_t) {point.x + adjust_x, point.y + adjust_y})

// svg draw canvas
typedef struct SnailCanvas {
    const float width, height;
    float translateX, translateY;
    vt_str_t *surface;
} snl_canvas_t;

/**
 * @brief Creates a new canvas 
 * 
 * @param width canvas width
 * @param height canvas height
 * @return snl_canvas_t
 */
extern snl_canvas_t snl_canvas_create(const float width, const float height);

/**
 * @brief Release canvas memory
 * 
 * @param canvas canvas instance
 */
extern void snl_canvas_destroy(snl_canvas_t *canvas);

/**
 * @brief Preallocates memory for fast rendering 
 * 
 * @param canvas canvas instance
 * @param bytes amount
 * @return None 
 */
extern void snl_canvas_preallocate(snl_canvas_t *const canvas, float bytes);

/**
 * @brief Creates a blur filter 
 * 
 * @param canvas canvas instance
 * @param id filter name
 * @param blurnessHorizontal amount
 * @param blurnessVertical amount
 * @return None 
 */
extern void snl_canvas_add_filter_blur(snl_canvas_t *const canvas, const char *const id, const int32_t blurnessHorizontal, const int32_t blurnessVertical);

/**
 * @brief Creates a blur filter with hard edge 
 * 
 * @param canvas canvas instance
 * @param id filter name
 * @param blurnessHorizontal amount
 * @param blurnessVertical amount
 * @return None 
 */
extern void snl_canvas_add_filter_blur_hard_edge(snl_canvas_t *const canvas, const char *const id, const int32_t blurnessHorizontal, const int32_t blurnessVertical);

/**
 * @brief Creates a shadow filter 
 * 
 * @param canvas canvas instance
 * @param id filter name
 * @param offsetX amount
 * @param offsetY amount
 * @param blurness amount
 * @param color_blend blend colors or not
 * @return None 
 */
extern void snl_canvas_add_filter_shadow(
    snl_canvas_t *const canvas, 
    const char *const id, 
    const int32_t offsetX, 
    const int32_t offsetY, 
    const int32_t blurness, 
    const bool color_blend
);

/**
 * @brief Creates a linear gradient 
 * 
 * @param canvas canvas instance
 * @param id filter name
 * @param colorA color gradient
 * @param colorB color gradient
 * @param offsetA colorA offset ~[0; 100]
 * @param offsetB colorB offset ~[0; 100]
 * @param opacityA colorA opacity
 * @param opacityB colorB opacity
 * @param angle amount
 * @return None 
 */
extern void snl_canvas_add_gradient_linear(
    snl_canvas_t *const canvas, 
    const char *const id, 
    const struct SnailColor colorA, 
    const struct SnailColor colorB,
    const int32_t offsetA,
    const int32_t offsetB,
    const float opacityA,
    const float opacityB,
    const float angle
);

/**
 * @brief Creates a linear gradient with 3 color gradients 
 * 
 * @param canvas canvas instance
 * @param id filter name
 * @param colorA color gradient
 * @param colorB color gradient
 * @param colorC color gradient
 * @param offsetA colorA offset ~[0; 100]
 * @param offsetB colorB offset ~[0; 100]
 * @param offsetC colorC offset ~[0; 100]
 * @param opacityA colorA opacity
 * @param opacityB colorB opacity
 * @param opacityC colorC opacity
 * @param angle amount
 * @return None 
 */
extern void snl_canvas_add_gradient_linear_tricolor(
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
);

/**
 * @brief Creates a radial gradient
 * 
 * @param canvas canvas instance
 * @param id filter name
 * @param colorA color gradient
 * @param colorB color gradient
 * @param offsetA colorA offset
 * @param offsetB colorB offset
 * @param opacityA colorA opacity
 * @param opacityB colorB opacity
 * @return None 
 */
extern void snl_canvas_add_gradient_radial(
    snl_canvas_t *const canvas, 
    const char *const id,
    const struct SnailColor colorA, 
    const struct SnailColor colorB,
    const int32_t offsetA,
    const int32_t offsetB,
    const float opacityA,
    const float opacityB
);

/**
 * @brief Creates a radial gradient with 3 color gradients 
 * 
 * @param canvas canvas instance
 * @param id filter name
 * @param colorA color gradient
 * @param colorB color gradient
 * @param colorC color gradient
 * @param offsetA colorA offset
 * @param offsetB colorB offset
 * @param offsetC colorC offset
 * @param opacityA colorA opacity
 * @param opacityB colorB opacity
 * @param opacityC colorC opacity
 * @return None 
 */
extern void snl_canvas_add_gradient_radial_tricolor(
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
);

/**
 * @brief Render a single line to canvas surface
 * 
 * @param canvas canvas instance
 * @param start starting point of the line
 * @param end ending point of the line
 * @param appearance outlook
 */
extern void snl_canvas_render_line(
    snl_canvas_t *const canvas, 
    snl_point_t start, snl_point_t end, 
    const snl_appearance_t appearance
);

/**
 * @brief Render a circle to canvas surface
 * 
 * @param canvas canvas instance
 * @param origin circle origin
 * @param radius circle radius
 * @param appearance outlook
 * @return None
 */
extern void snl_canvas_render_circle(
    snl_canvas_t *const canvas, 
    struct SnailPoint origin, const float radius, 
    const struct SnailAppearance appearance
);

/**
 * @brief Render an ellipse to canvas surface
 * 
 * @param canvas canvas instance
 * @param origin ellipse origin
 * @param radius along the x and y axis
 * @param appearance outlook
 * @return None
 */
extern void snl_canvas_render_ellipse(
    snl_canvas_t *const canvas, 
    struct SnailPoint origin, const struct SnailPoint radius,
    const struct SnailAppearance appearance
);

/**
 * @brief Render a rectangle to canvas surface
 * 
 * @param canvas canvas instance
 * @param pos rectangle position
 * @param size rectangle width and height
 * @param radius corner smoothness
 * @param appearance outlook
 * @return None
 */
extern void snl_canvas_render_rectangle(
    snl_canvas_t *const canvas, 
    snl_point_t pos, const snl_point_t size, const float radius, 
    const snl_appearance_t appearance
);

/**
 * @brief Start rendering a polygon to canvas surface
 * 
 * @param canvas canvas instance
 * @return None
 * 
 * @note called before <snl_canvas_render_polygon_point()> calls
 */
extern void snl_canvas_render_polygon_begin(snl_canvas_t *const canvas);

/**
 * @brief Continue rendering a polygon to canvas surface
 * 
 * @param canvas canvas instance
 * @param point next polygon point
 * @return None
 * 
 * @note called between <snl_canvas_render_polygon_begin()> and <snl_canvas_render_polygon_end()>
 */
extern void snl_canvas_render_polygon_point(snl_canvas_t *const canvas, snl_point_t point);

/**
 * @brief Finish rendering a polygon to canvas surface
 * 
 * @param canvas canvas instance
 * @param appearance outlook
 * @param fill_rule fill rule
* @return None
 * 
 * @note called after <snl_canvas_render_polygon_point()> calls
 */
extern void snl_canvas_render_polygon_end(snl_canvas_t *const canvas, const snl_appearance_t appearance, const char *const fill_rule);

/**
 * @brief Start rendering a polyline to canvas surface
 * 
 * @param canvas canvas instance
 * @return None
 * 
 * @note called before <snl_canvas_render_polyline_point()> calls
 */
extern void snl_canvas_render_polyline_begin(snl_canvas_t *const canvas);

/**
 * @brief Continue rendering a polyline to canvas surface
 * 
 * @param canvas canvas instance
 * @param point next polyline point
 * @return None
 * 
 * @note called between <snl_canvas_render_polyline_begin()> and <snl_canvas_render_polyline_end()>
 */
extern void snl_canvas_render_polyline_point(snl_canvas_t *const canvas, snl_point_t point);

/**
 * @brief Finish rendering a polyline to canvas surface
 * 
 * @param canvas canvas instance
 * @param appearance outlook
 * @return None
 * 
 * @note called after <snl_canvas_render_polyline_point()> calls
 */
extern void snl_canvas_render_polyline_end(snl_canvas_t *const canvas, const snl_appearance_t appearance);

/**
 * @brief Render a curve to canvas surface with equal curvature

 * @param canvas canvas instance
 * @param start starting point
 * @param end ending point
 * @param appearance outlook
 * @return None
 */
extern void snl_canvas_render_curve(
    snl_canvas_t *const canvas, 
    snl_point_t start, snl_point_t end, 
    const snl_appearance_t appearance
);

/**
 * @brief Render a curve to canvas surface with custom curvature

 * @param canvas canvas instance
 * @param start starting point
 * @param end ending point
 * @param curve_height custom curve height
 * @param curvature custom curvature
 * @param appearance outlook
 * @return None
 */
extern void snl_canvas_render_curve_custom(
    snl_canvas_t *const canvas, 
    snl_point_t start, snl_point_t end, 
    const float curve_height, const float curvature, 
    const snl_appearance_t appearance
);

/**
 * @brief Start path rendering to canvas surface
 * 
 * @param canvas canvas instance
 * @return None
 * 
 * @note called before <snl_canvas_render_path_line_to()> and <snl_canvas_render_path_move_by()> calls
 */
extern void snl_canvas_render_path_begin(snl_canvas_t *const canvas);

/**
 * @brief Render path to canvas surface
 * 
 * @param canvas canvas instance
 * @param point next point
 * @return None
 * 
 * @note called between <snl_canvas_render_path_begin()> and <snl_canvas_render_path_end()> calls
 */
extern void snl_canvas_render_path_line_to(snl_canvas_t *const canvas, snl_point_t point);

/**
 * @brief Render path to canvas surface
 * 
 * @param canvas canvas instance
 * @param point amount to move by from previous point
 * @return None
 * 
 * @note called between <snl_canvas_render_path_begin()> and <snl_canvas_render_path_end()> calls
 */
extern void snl_canvas_render_path_move_by(snl_canvas_t *const canvas, const snl_point_t amount);

/**
 * @brief End path rendering to canvas surface
 * 
 * @param canvas canvas instance
 * @return None
 * 
 * @note called after <snl_canvas_render_path_line_to()> and <snl_canvas_render_path_move_by()> calls
 */
extern void snl_canvas_render_path_end(snl_canvas_t *const canvas, const snl_appearance_t appearance);

/**
 * @brief Render text to canvas surface with custom font and color
 * 
 * @param canvas canvas instance
 * @param pos text position
 * @param text text value
 * @param font_size font size
 * @param font_family font family
 * @param color text color
 * @return None
 * 
 * @note see also <snl_canvas_render_text()> and <snl_canvas_render_text_styled()>
 */
extern void snl_canvas_render_text(snl_canvas_t *const canvas, snl_point_t pos, const char* const text, const float font_size, const char *const font_family, const struct SnailColor color);

/**
 * @brief Render text to canvas surface with custom style
 * 
 * @param canvas canvas instance
 * @param pos text position
 * @param text text value
 * @param appearance text appearance (color, stroke, fill, etc.)
 * @param text_style text style settings (underline, rotation, font family, etc.)
 * @return None
 * 
 * @note see also <snl_canvas_render_text()> and <snl_canvas_render_text()>
 */
extern void snl_canvas_render_text_styled(snl_canvas_t *const canvas, snl_point_t pos, const char* const text, const snl_appearance_t appearance, snl_text_style_t text_style);

/**
 * @brief Undo the last rendering operation
 * 
 * @param canvas canvas instance
 * @return None
 */
extern void snl_canvas_undo(snl_canvas_t *const canvas);

/**
 * @brief Clear canvas surface
 * 
 * @param canvas canvas instance
 * @return None
 */
extern void snl_canvas_clear(snl_canvas_t *const canvas);

/**
 * @brief Translate canvas view
 * 
 * @param canvas canvas instance
 * @param x translate view along the horizontal axis
 * @param y translate view along the vertical axis
 * @return None
 * 
 * @note does not effect previosly rendered shapes
 */
extern void snl_canvas_translate(snl_canvas_t *const canvas, const float x, const float y);

/**
 * @brief Reset canvas translation
 * 
 * @param canvas canvas instance
 * @return None
 * 
 * @note does not effect previosly rendered shapes
 */
extern void snl_canvas_reset_translation(snl_canvas_t *const canvas);

/**
 * @brief Color fill the canvas surface
 * 
 * @param canvas canvas instance
 * @param color color
 * @return None
 */
extern void snl_canvas_fill(snl_canvas_t *const canvas, struct SnailColor color);

/**
 * @brief Save canvas
 * 
 * @param canvas canvas instance
 * @param filename name
 * @return None
 */
extern void snl_canvas_save(const snl_canvas_t *const canvas, const char *const filename);

#endif // SNAIL_CANVAS_H

