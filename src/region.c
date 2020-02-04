#include "region.h"

PTImg pt_new_from_subregion(PTImg src, pt_rectangle r){
  pt_vec2 size = pt_rect_size(r);
  PTImg new_img = pt_new(size.w, size.h, src->mode);
  uint j = 0;
  switch(src->mode){
    case RGB:
      for(uint y = r.y0; y < r.y1; y++){
        for(uint x = r.x0; x < r.x1; x++)
          new_img->rgb[j++] = src->rgb[xy_to_i(src, x, y)];
      }
      break;
    case RGBA:
      for(uint y = r.y0; y < r.y1; y++){
        for(uint x = r.x0; x < r.x1; x++)
          new_img->rgba[j++] = src->rgba[xy_to_i(src, x, y)];
      }
      break;
    case INDEXED:
      new_img->palette = pt_copy_palette(src->palette);
      __attribute__((fallthrough));
    case GRAYSCALE:
      for(uint y = r.y0; y < r.y1; y++){
        for(uint x = r.x0; x < r.x1; x++)
          new_img->raw[j++] = src->raw[xy_to_i(src, x, y)];
      }
      break;
  }
  return new_img;
}
