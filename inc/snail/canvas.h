#ifndef SNAIL_CANVAS_H
#define SNAIL_CANVAS_H

/** CANVAS MODULE
    - ...
*/

#include <stdint.h>
#include "vita/container/str.h"

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
struct SnailAppearance {
    // stroke
    uint8_t stroke_width;
    float stroke_opacity;
    struct SnailColor stroke_color;

    // fill
    float fill_opacity;
    struct SnailColor fill_color;
};

// svg draw canvas
typedef struct SnailCanvas {
    uint32_t width, height;
    int32_t translateX, translateY;
    vt_str_t *canvas;
} snl_canvas_t;

/**
 * @brief Creates a new canvas 
 * 
 * @param width canvas width
 * @param height canvas height
 * @return snl_canvas_t 
 */
snl_canvas_t snl_canvas_create(const uint32_t width, const uint32_t height);

/**
 * @brief Release canvas memory
 * 
 * @param canvas canvas instance
 */
void snl_canvas_destroy(snl_canvas_t *canvas);

/**
 * @brief Get canvas width
 * 
 * @param canvas canvas instance
 * @return uint32_t 
 */
uint32_t snl_canvas_get_width(const snl_canvas_t *const canvas);

/**
 * @brief Get canvas height
 * 
 * @param canvas canvas instance
 * @return uint32_t 
 */
uint32_t snl_canvas_get_height(const snl_canvas_t *const canvas);



void snl_canvas_render_line(snl_canvas_t *const canvas, SHAPE);
void snl_canvas_render_circle(snl_canvas_t *const canvas, SHAPE);
void snl_canvas_render_ellipse(snl_canvas_t *const canvas, SHAPE);
void snl_canvas_render_rectangle(snl_canvas_t *const canvas, SHAPE);

// todo: think
void snl_canvas_render_polygon(snl_canvas_t *const canvas, SHAPE);
void snl_canvas_render_polyline(snl_canvas_t *const canvas, SHAPE);

void snl_canvas_render_curve(snl_canvas_t *const canvas, SHAPE);
void snl_canvas_render_path_begin(snl_canvas_t *const canvas, SHAPE);
void snl_canvas_render_path_move_to(snl_canvas_t *const canvas, SHAPE);
void snl_canvas_render_path_end(snl_canvas_t *const canvas, SHAPE);
void snl_canvas_render_text(snl_canvas_t *const canvas, SHAPE);



void snl_canvas_undo(snl_canvas_t *const canvas);
void snl_canvas_translate(snl_canvas_t *const canvas, const int32_t x, const int32_t y);
void snl_canvas_reset_translation(snl_canvas_t *const canvas);
void snl_canvas_fill(snl_canvas_t *const canvas, struct SnailColor color);
void snl_canvas_save(const snl_canvas_t *const canvas, const char *const filename);

#endif // SNAIL_CANVAS_H


