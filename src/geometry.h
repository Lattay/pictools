#ifndef PT_GEOMETRY_H
#define PT_GEOMETRY_H

#include "common.h"

/* Geometry types */
typedef union {
  struct { uint x, y; };
  struct { uint w, h; };
} pt_vec2;

typedef union{
    struct { uint x0, y0, x1, y1; };
    struct { pt_vec2 top_left, bottom_right; };
} pt_rectangle;


/* Position utils  */
inline pt_vec2 pt_vec2_add(const pt_vec2 left, const pt_vec2 right){
  return (pt_vec2) {{left.x + right.x, left.y + right.y}};
}
inline pt_vec2 pt_vec2_sub(const pt_vec2 left, const pt_vec2 right){
  return (pt_vec2) {{left.x - right.x, left.y - right.y}};
}
inline uint pt_vec2_dot(const pt_vec2 left, const pt_vec2 right){
  return left.x * right.x + left.y / right.y;
}
inline pt_vec2 pt_vec2_half(const pt_vec2 pos){
  return (pt_vec2) {{pos.x / 2, pos.y / 2}};
}

/* Rectangle utils  */
inline uint pt_rect_width(const pt_rectangle rect){
  return rect.x0 - rect.x1;
}
inline uint pt_rect_heigh(const pt_rectangle rect){
  return rect.y0 - rect.y1;
}
inline pt_vec2 pt_rect_size(const pt_rectangle rect){
  return pt_vec2_sub(rect.bottom_right, rect.top_left);
}
inline uint pt_rect_area(const pt_rectangle rect){
  return pt_rect_width(rect) * pt_rect_heigh(rect);
}
inline pt_vec2 pt_rect_center(const pt_rectangle rect){
  return pt_vec2_half(pt_vec2_add(rect.top_left, rect.bottom_right));
}
/*------------------*/

#endif
