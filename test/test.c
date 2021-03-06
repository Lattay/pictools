#include <stdio.h>
#include "../src/pictools.h"

static uint mapFF(float f, float min, float max){
  return (uint) 255 * ((f - min)/(max - min)) ;
}

int main(){
  PTImg img = pt_load("./res/red_circles_64.png", RGB);
  PTImg gs = pt_copy_to_grayscale(img);
  PTImg gs_x = pt_copy_to_grayscale(img);
  PTImg gs_y = pt_copy_to_grayscale(img);
  PTImg gs_r = pt_copy_to_grayscale(img);
  pt_free(img);
  for(uint x = 1; x < gs->w-1; x++){
    for(uint y = 1; y < gs->h-1; y++){
      float dfx = (pt_get_color_grayscale(gs, x + 1, y)
          - pt_get_color_grayscale(gs, x - 1, y)) / 2.0;
      float dfy = (pt_get_color_grayscale(gs, x, y + 1)
          - pt_get_color_grayscale(gs, x, y - 1)) / 2.0;
      pt_set_color_grayscale(gs_x, x, y, mapFF(dfx, -125, 125));
      pt_set_color_grayscale(gs_y, x, y, mapFF(dfy, -125, 125));
      pt_set_color_grayscale(gs_r, x, y, mapFF(dfx*dfx + dfy*dfy, 0, 31250));
    }
  }
  pt_write("./dx.png", gs_x);
  pt_write("./dy.png", gs_y);
  pt_write("./image.png", gs_r);
  return 0;
}
