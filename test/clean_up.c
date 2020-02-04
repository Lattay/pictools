#include <stdio.h>
#include <math.h>
#include "../src/pictools.h"

int main(){
  PTImg img = pt_load("./res/red_circles_64.png", RGB);
  PTImg gs = pt_copy_to_grayscale(img);

  // clean side inconditionally
  for(uint x = 0; x < img->w; x++){
    pt_set_color(img, x, 0, pt_white);
    pt_set_color(img, x, img->h-1, pt_white);
  }
  for(uint y = 0; y < img->h; y++){
    pt_set_color(img, 0, y, pt_white);
    pt_set_color(img, img->w-1, y, pt_white);
  }

  // clean low gradient pixel
  for(uint x = 1; x < gs->w-1; x++){
    for(uint y = 1; y < gs->h-1; y++){
      float dfx = (pt_get_color_grayscale(gs, x + 1, y)
          - pt_get_color_grayscale(gs, x - 1, y)) / 2.0;
      float dfy = (pt_get_color_grayscale(gs, x, y + 1)
          - pt_get_color_grayscale(gs, x, y - 1)) / 2.0;
      if(sqrt(dfx*dfx + dfy*dfy) < 40 && pt_get_color_grayscale(gs, x, y) > 210){
        pt_set_color(img, x, y, pt_white);
      }
    }
  }

  pt_write("./image.png", img);
  return 0;
}
