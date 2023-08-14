#ifndef SNAIL_CANVAS_H
#define SNAIL_CANVAS_H

#include <stdint.h>
#include "vita/container/str.h"

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

void snl_canvas_render_shape(snl_canvas_t *const canvas, SHAPE);
void snl_canvas_undo(snl_canvas_t *const canvas);
void snl_canvas_translate(snl_canvas_t *const canvas, const int32_t x, const int32_t y);
void snl_canvas_reset_translation(snl_canvas_t *const canvas);
void snl_canvas_fill(snl_canvas_t *const canvas, COLOR);
void snl_canvas_save(const snl_canvas_t *const canvas, const char *const filename);

#endif // SNAIL_CANVAS_H


