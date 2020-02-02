#ifndef PT_BASE_H
#define PT_BASE_H

#include "../vendor/stb_image.h"
#include "../vendor/stb_image_write.h"

#include "common.h"
#include "geometry.h"

/* Color types */
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

#define xy_to_i(src, x, y) (src->w * y + x)
#define v2_to_i(src, pos) (src->w * pos.y + pos.x)

/* Color conversion   */
inline pt_color_a pt_rgb_to_rgba(const pt_color src){ return (pt_color_a) {src.r, src.g, src.b, 255}; }
inline pt_uc pt_rgb_to_grayscale(const pt_color src){ return (src.r + src.g + src.b)/3; }
inline pt_color pt_rgba_to_rgb(const pt_color_a src){ return (pt_color) {src.r, src.g, src.b}; }
inline pt_uc pt_rgba_to_grayscale(const pt_color_a src){ return (src.r + src.g + src.b)/3; }
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

/* Low level access */
pt_color pt_get_color(const PTImg src, uint x, uint y);
pt_color_a pt_get_color_a(const PTImg src, uint x, uint y);
pt_uc pt_get_color_grayscale(const PTImg src, uint x, uint y);
pt_uc pt_get_color_index(const PTImg src, uint x, uint y);
inline pt_color pt_vget_color(const PTImg src, const pt_vec2 pos){ return pt_get_color(src, pos.x, pos.y); }
inline pt_color_a pt_vget_color_a(const PTImg src, const pt_vec2 pos){ return pt_get_color_a(src, pos.x, pos.y); }
inline pt_uc pt_vget_color_grayscale(const PTImg src,  const pt_vec2 pos){ return pt_get_color_grayscale(src, pos.x, pos.y); }
inline pt_uc pt_vget_color_index(const PTImg src,  const pt_vec2 pos){ return pt_get_color_index(src, pos.x, pos.y); }

bool pt_bb_get(const PTBinBuf src, uint x, uint y);
inline bool pt_bb_vget(const PTBinBuf src, const pt_vec2 pos){ return pt_bb_get(src, pos.x, pos.y); }

void pt_set_color(PTImg src, uint x, uint y, pt_color color);
void pt_set_color_a(PTImg src, uint x, uint y, pt_color_a color);
void pt_set_color_grayscale(PTImg src, uint x, uint y, pt_uc level);
void pt_set_color_index(PTImg src, uint x, uint y, pt_uc i);
inline void pt_vset_color(PTImg src, const pt_vec2 pos, pt_color color){ pt_set_color(src, pos.x, pos.y, color); }
inline void pt_vset_color_a(PTImg src, const pt_vec2 pos, pt_color_a color){ pt_set_color_a(src, pos.x, pos.y, color); }
inline void pt_vset_color_grayscale(PTImg src, const pt_vec2 pos, pt_uc level){ pt_set_color_grayscale(src, pos.x, pos.y, level); }
inline void pt_vset_color_index(PTImg src, const pt_vec2 pos, pt_uc i){ pt_set_color_index(src, pos.x, pos.y, i); }

void pt_bb_set(PTBinBuf src, uint x, uint y, bool val);
void pt_bb_toggle(PTBinBuf src, uint x, uint y);
inline void pt_bb_vset(PTBinBuf src, pt_vec2 pos, bool val){ pt_bb_set(src, pos.x, pos.y, val); }
inline void pt_bb_vtoggle(PTBinBuf src, pt_vec2 pos){ pt_bb_toggle(src, pos.x, pos.y); }
/*------------------*/

/* Copy w/ conversion */
PTImg pt_copy_to_rgb(PTImg src);
PTImg pt_copy_to_rgba(PTImg src);
PTImg pt_copy_to_grayscale(PTImg src);
PTImg pt_copy_to_indexed(PTImg src);
PTPalette pt_copy_palette(PTPalette src);
/*--------------------*/

#endif
