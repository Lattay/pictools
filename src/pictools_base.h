#ifndef PICTOOLS_BASE_H
#define PICTOOLS_BASE_H

#include <stdbool.h>

/* #ifdef PICTOOLS_IMPLEMENTATION */
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
/* #endif */

#include "../vendor/stb_image.h"
#include "../vendor/stb_image_write.h"

#define pt_error perror

typedef unsigned int uint;

typedef unsigned int pt_ui;
typedef unsigned char pt_uc;

/* Types */
typedef enum { RGB, RGBA, GRAYSCALE, INDEXED } pt_color_mode;
typedef struct { pt_uc r, g, b; } pt_color;
typedef struct { pt_uc r, g, b, a; } pt_color_a;

typedef struct {
  size_t length;
  pt_color colors[];
} pt_palette;
typedef pt_palette* PTPalette;

typedef struct {
  pt_color_mode mode;
  union {
    pt_uc* raw;
    pt_color* rgb;
    pt_color_a* rgba;
  };
  uint w, h;
  union { /* palette and channels cannot be used together */
    uint channels;
    PTPalette palette;
  };
} pt_img;
typedef pt_img* PTImg;

typedef struct {
  uint w, h;
  pt_uc data[];
} pt_bin_buf;
typedef pt_bin_buf* PTBinBuf;

/* Color conversion   */
inline pt_color_a pt_rgb_to_rgba(const pt_color* src){ return (pt_color_a) {src->r, src->g, src->b, 255}; }
inline pt_uc pt_rgb_to_grayscale(const pt_color* src){ return (src->r + src->g + src->b)/3; }
inline pt_color pt_rgba_to_rgb(const pt_color_a* src){ return (pt_color) {src->r, src->g, src->b}; }
inline pt_uc pt_rgba_to_grayscale(const pt_color_a* src){ return (src->r + src->g + src->b)/3; }
/*--------------------*/

/* File IO          */
PTImg pt_load(char* file_name);
bool pt_write(char* file_name, PTImg img);
/*------------------*/

/* Memory allocation */
PTImg pt_new_img(uint w, uint h, pt_color_mode mode);
PTImg pt_new_indexed(uint w, uint h, size_t length);
PTBinBuf pt_new_bb(uint w, uint h);

void pt_free_img(PTImg img);
void pt_free_indexed(PTImg img);
void pt_free_bb(PTBinBuf img);

PTPalette pt_new_palette(size_t n);

void pt_free_palette(PTPalette palette);
/*-------------------*/

/* Unsafe internals (use at your own risk)
 * inline pt_color* pt_get_pixel(PTImg src, uint x, uint y);
 * inline pt_color_a* pt_get_pixel_a(PTImg src, uint x, uint y);
 * inline pt_uc* pt_get_pixel_raw(PTImg src, uint x, uint y);
 *-----------------------------------------*/

/* Low level access */
pt_color pt_get_color(PTImg src, uint x, uint y);
pt_color_a pt_get_color_a(PTImg src, uint x, uint y);
pt_uc pt_get_color_grayscale(PTImg src, uint x, uint y);
pt_uc pt_get_color_index(PTImg src, uint x, uint y);

bool pt_bb_get(PTBinBuf src, uint x, uint y);

void pt_set_color(PTImg src, uint x, uint y, pt_color color);
void pt_set_color_a(PTImg src, uint x, uint y, pt_color_a color);
void pt_set_color_grayscale(PTImg src, uint x, uint y, pt_uc level);
void pt_set_color_index(PTImg src, uint x, uint y, pt_uc i);

void pt_bb_set(PTBinBuf src, uint x, uint y, bool val);
void pt_bb_toggle(PTBinBuf src, uint x, uint y);
/*------------------*/

/* Copy w/ conversion */
PTImg pt_copy_to_rgb(PTImg src);
PTImg pt_copy_to_rgba(PTImg src);
PTImg pt_copy_to_grayscale(PTImg src);
PTImg pt_copy_to_indexed(PTImg src);
PTPalette pt_copy_palette(PTPalette src);
/*--------------------*/

#endif
