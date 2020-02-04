#include <stdlib.h>
#include <limits.h>
#include "base.h"

#define debug printf


/* File IO */
PTImg pt_load(char* file_name, pt_color_mode mode){
  PTImg ptimg = (PTImg) malloc(sizeof(pt_img));
  if(!ptimg){
    return NULL;
  }
  int w, h, channels, expected;
  switch(mode){
    case RGB:
      expected = 3; break;
    case RGBA:
      expected = 4; break;
    case GRAYSCALE:
      expected = 1; break;
    default:
      expected = 1; break;
  }
  ptimg->raw = stbi_load(file_name, &w, &h, &channels, expected);
  ptimg->h = h;
  ptimg->w = w;
  ptimg->channels = channels;

  switch(ptimg->channels){
    case 1:
      ptimg->mode = GRAYSCALE;
      break;
    case 3:
      ptimg->mode = RGB;
      break;
    case 4:
      ptimg->mode = RGBA;
      break;
  }

  return ptimg;
}

bool pt_write(char* file_name, const PTImg img){
  FILE* f = fopen(file_name, "w");
  if (!f){
    return false;
  } else {
    fclose(f);
  }

  if(img->mode == INDEXED){
    PTImg cimg = pt_copy_to_rgba(img);
    stbi_write_png(file_name, cimg->w, cimg->h, cimg->channels, cimg->raw, 0);
    pt_free(cimg);
  } else {
    stbi_write_png(file_name, img->w, img->h, img->channels, img->raw, 0);
  }

  return true;
}

void pt_clear(PTImg img){
  uint max = img->w * img->h;
  
  switch(img->mode){
    case RGB:
      max *= 3;
      break;
    case RGBA:
      max *= 4;
      break;
    case INDEXED:
      for(uint i = 0; i < img->palette->length; i++){
        img->palette->colors[i] = (pt_color) {0, 0, 0};
      }
      break;
    default:
      ;
  }
  for(uint i = 0; i < max; i++){
    img->raw[i] = 0;
  }
}

/* Memory allocation */
PTImg pt_new(uint w, uint h, pt_color_mode mode){
  PTImg img = (PTImg) malloc(sizeof(pt_img));
  if(!img)
    return NULL;
  img->mode = mode;
  img->w = w;
  img->h = h;
  switch(mode){
    case INDEXED:
      img->palette = (PTPalette) NULL;
      img->raw = malloc(sizeof(pt_uc) * w * h);
      break;
    case RGB:
      img->channels = 3;
      img->raw = (pt_uc*) malloc(sizeof(pt_uc) * w * h * 3);
      break;
    case RGBA:
      img->channels = 4;
      img->raw = (pt_uc*) malloc(sizeof(pt_uc) * w * h * 4);
      break;
    case GRAYSCALE:
      img->channels = 1;
      img->raw = (pt_uc*) malloc(sizeof(pt_uc) * w * h);
      break;
  }

  return img;
}

PTImg pt_new_indexed(uint w, uint h, size_t length){
  PTPalette palette = pt_new_palette(length);
  if(!palette)
    return NULL;
  PTImg img = pt_new(w, h, INDEXED);
  img->palette = palette;
  return img;
}

PTBinBuf pt_new_bb(uint w, uint h){
  size_t size = w * h / CHAR_BIT + 1;
  PTBinBuf bb = (PTBinBuf) malloc(sizeof(uint) * 2 + sizeof(pt_uc) * size);
  if(!bb)
    return NULL;
  bb->w = w; bb->h = h;
  return bb;
}

void pt_free(PTImg img){
  if(!img)
    return;
  if(img->raw)
    free(img->raw);
  free(img);
}

void pt_free_indexed(PTImg img){
  if(!img)
    return;
  if(img->mode == INDEXED)
    pt_free_palette(img->palette);

  pt_free(img);
}

void pt_free_bb(PTBinBuf bb){
  if(bb)
    free(bb);
}

PTPalette pt_new_palette(size_t length){
  if(length > UCHAR_MAX)
    return NULL;
  PTPalette palette = (PTPalette) malloc(sizeof(size_t)
      + sizeof(pt_color) * length);
  if(!palette)
    return NULL;
  palette->length = length;
  return palette;
}

void pt_free_palette(PTPalette palette){
  if(palette)
    free(palette);
}

/* Safe access */
pt_color pt_get_color(PTImg src, uint x, uint y){
  if(x >= src->w || y >= src->h)
    pt_error("Out of bound get_color.");
  uint i;
  switch(src->mode){
    case RGB:
      return src->rgb[xy_to_i(src, x, y)];
    case RGBA:
      return *((pt_color*) src->rgba + xy_to_i(src, x, y));
    case INDEXED:
      i = xy_to_i(src, x, y);
      if(i >= src->palette->length)
        pt_error("Out of palette color in indexed picture.");
      return src->palette->colors[i];
    case GRAYSCALE:
      i = xy_to_i(src, x, y);
      return (pt_color) {i, i, i};
    default:
      pt_error("Illegal mode");
  }
  return (pt_color) {0, 0, 0}; // not reached
}

pt_color_a pt_get_color_a(PTImg src, uint x, uint y){
  if(x >= src->w || y >= src->h)
    pt_error("Out of bound get_color.");
  uint i;
  pt_color_a color = {0, 0, 0, 255};
  pt_color* cl;
  switch(src->mode){
    case RGB:
      cl = src->rgb + xy_to_i(src, x, y);
      color.r = cl->r;
      color.g = cl->g;
      color.b = cl->b;
      return color;
    case RGBA:
      return src->rgba[xy_to_i(src, x, y)];
    case INDEXED:
      i = xy_to_i(src, x, y);
      if(i >= src->palette->length)
        pt_error("Out of palette color in indexed picture.");
      color.r = src->palette->colors[i].r;
      color.g = src->palette->colors[i].g;
      color.b = src->palette->colors[i].b;
      return color;
    case GRAYSCALE:
      i = src->raw[xy_to_i(src, x, y)];
      return (pt_color_a) {i, i, i, 255};
    default:
      pt_error("Illegal mode");
  }
  return (pt_color_a) {0, 0, 0, 0}; // not reached
}

pt_uc pt_get_color_grayscale(PTImg src, uint x, uint y){
  if(src->mode != GRAYSCALE)
    pt_error("Illegal get_color_grayscale");
  if(x >= src->w || y >= src->h)
    pt_error("Out of bound get_color.");

  return src->raw[xy_to_i(src, x, y)];
}

pt_uc pt_get_color_index(PTImg src, uint x, uint y){
  if(src->mode != INDEXED)
    pt_error("Illegal get_color_index");
  if(x >= src->w || y >= src->h)
    pt_error("Out of bound get_color.");

  return src->raw[xy_to_i(src, x, y)];
}


bool pt_bb_get(PTBinBuf src, uint x, uint y){
  if(x >= src->w || y >= src->h)
    pt_error("Out of bound bb_get");

  uint i = xy_to_i(src, x, y);
  return !!((1 << (i%CHAR_BIT)) & src->data[i/CHAR_BIT]);
}

void pt_set_color(PTImg src, uint x, uint y, pt_color color){
  if(src->mode == INDEXED || src->mode == GRAYSCALE)
    pt_error("Illegal set_color");
  if(x >= src->w || y >= src->h)
    pt_error("Out of bound set_color");

  if(src->mode == RGB){
    src->rgb[xy_to_i(src, x, y)] = color;
  } else {
    src->rgba[xy_to_i(src, x, y)].r = color.r;
    src->rgba[xy_to_i(src, x, y)].g = color.g;
    src->rgba[xy_to_i(src, x, y)].b = color.b;
  }
}

void pt_set_color_a(PTImg src, uint x, uint y, pt_color_a color){
  if(x >= src->w || y >= src->h)
    pt_error("Out of bound set_color");
  if(src->mode != RGBA)
    pt_error("Illegal set_color_a");

  src->rgba[xy_to_i(src, x, y)] = color;
}

void pt_set_color_grayscale(PTImg src, uint x, uint y, pt_uc level){
  if(src->mode != GRAYSCALE)
    pt_error("Illegal set_color_grayscale");
  if(x >= src->w || y >= src->h)
    pt_error("Out of bound set_color");

  src->raw[xy_to_i(src, x, y)] = level;
}

void pt_set_color_index(PTImg src, uint x, uint y, pt_uc i){
  if(src->mode != INDEXED)
    pt_error("Illegal set_color_index");
  if(x >= src->w || y >= src->h)
    pt_error("Out of bound set_color");
  if(i >= src->palette->length)
    pt_error("Out of palette set_color");

  src->raw[xy_to_i(src, x, y)] = i;
}

void pt_bb_set(PTBinBuf src, uint x, uint y, bool val){
  if(x >= src->w || y >= src->h)
    pt_error("Out of bound bb_set");
  uint i = xy_to_i(src, x, y);
  uint i_eff = i / CHAR_BIT;
  pt_uc mask = 1 << (i%CHAR_BIT);
  src->data[i_eff] &= ~mask;  // clear
  if(val)  // set
    src->data[i_eff] |= mask;
}

void pt_bb_toggle(PTBinBuf src, uint x, uint y){
  if(x >= src->w || y >= src->h)
    pt_error("Out of bound bb_toggle");
  uint i = xy_to_i(src, x, y);
  uint i_eff = i / CHAR_BIT;
  pt_uc mask = 1 << (i%CHAR_BIT);
  src->data[i_eff] ^= mask;  // A xor 1 = not A
}

/* Copy with conversion */
PTImg pt_copy_to_rgb(PTImg src){
  PTImg new_img = pt_new(src->w, src->h, RGB);
  uint max = src->w * src->h;

  switch(src->mode){
    case RGB:
      for(uint i = 0; i < max; i++){
        new_img->rgb[i] = src->rgb[i];
      }
      break;
    case RGBA:
      for(uint i = 0, j = 0; i < max; i++, j++){
        new_img->raw[i] = src->raw[j];
        if(i%3 == 2) j++;
      }
      break;
    case GRAYSCALE:
      for(uint i = 0; i < max; i++){
        new_img->rgb[i].r = src->raw[i];
        new_img->rgb[i].g = src->raw[i];
        new_img->rgb[i].b = src->raw[i];
      }
      break;
    case INDEXED:
      for(uint i = 0; i < max; i++){
        new_img->rgb[i] = src->palette->colors[src->raw[i]];
      }
      break;
    default:
      pt_error("Illegal mode");
  }
  return new_img;

}

PTImg pt_copy_to_rgba(PTImg src){
  PTImg new_img = pt_new(src->w, src->h, RGBA);
  uint max = src->w * src->h;
  
  switch(src->mode){
    case RGB:
      for(uint i = 0, j = 0; i < max; i++, j++){
        new_img->raw[j] = src->raw[i];
        if(i%3 == 2) j++;
      }
      break;
    case RGBA:
      for(uint i = 0; i < max; i++){
        new_img->rgba[i] = src->rgba[i];
      }
      break;
    case GRAYSCALE:
      for(uint i = 0; i < max; i++){
        new_img->rgba[i].r = src->raw[i];
        new_img->rgba[i].g = src->raw[i];
        new_img->rgba[i].b = src->raw[i];
        new_img->rgba[i].a = 255;
      }
      break;
    case INDEXED:
      for(uint i = 0; i < max; i++){
        new_img->rgba[i] = pt_rgb_to_rgba(src->palette->colors[src->raw[i]]);
      }
      break;
  }
  return new_img;
}

PTImg pt_copy_to_grayscale(PTImg src){
  PTImg new_img = pt_new(src->w, src->h, GRAYSCALE);
  uint max = src->w * src->h;
  
  switch(src->mode){
    case RGB:
      for(uint i = 0; i < max; i++){
        new_img->raw[i] = pt_rgb_to_grayscale(src->rgb[i]);
      }
      break;
    case RGBA:
      for(uint i = 0; i < max; i++){
        new_img->raw[i] = pt_rgba_to_grayscale(src->rgba[i]);
      }
      break;
    case GRAYSCALE:
      for(uint i = 0; i < max; i++){
        new_img->raw[i] = src->raw[i];
      }
      break;
    case INDEXED:
      for(uint i = 0; i < max; i++){
        new_img->raw[i] = pt_rgb_to_grayscale(src->palette->colors[src->raw[i]]);
      }
      break;
  }
  return new_img;
}

PTImg pt_copy_to_indexed(PTImg src){
  PTImg new_img = pt_new(src->w, src->h, RGB);
  uint max = src->w * src->h;
  
  switch(src->mode){
    case INDEXED:
      for(uint i = 0; i < max; i++){
        new_img->raw[i] = src->raw[i];
      }
      break;
    default:
      pt_error("Copying any non indexed image to an indexed image is not implemented yet.");
  }
  return new_img;
}

PTPalette pt_copy_palette(PTPalette src){
  PTPalette palette = pt_new_palette(src->length);
  for(uint i = 0; i < src->length; i++)
    palette->colors[i] = src->colors[i];
  return palette;
}
