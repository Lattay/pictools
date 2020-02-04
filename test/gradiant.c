#include <stdio.h>
#include <math.h>
#include "../src/pictools.h"

static uint flatten(float f, float min, float max){
  if(f < min + 0.1*(max - min)){
    return 0;
  } else {
    return 255;
  }
}

int main(){
  PTImg img = pt_load("./res/red_circles_64.png", RGB);
  PTImg gs = pt_copy_to_grayscale(img);
  PTImg gs_r = pt_new(gs->w, gs->h, GRAYSCALE);
  pt_clear(gs_r);
  pt_free(img);
  for(uint x = 1; x < gs->w-1; x++){
    for(uint y = 1; y < gs->h-1; y++){
      float dfx = (pt_get_color_grayscale(gs, x + 1, y)
          - pt_get_color_grayscale(gs, x - 1, y)) / 2.0;
      float dfy = (pt_get_color_grayscale(gs, x, y + 1)
          - pt_get_color_grayscale(gs, x, y - 1)) / 2.0;
      pt_set_color_grayscale(gs_r, x, y, flatten(sqrt(dfx*dfx + dfy*dfy), 0, 361));
    }
  }
  pt_write("./image.png", gs_r);
  return 0;
}
