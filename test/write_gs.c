#include <stdio.h>
#include "../src/pictools.h"

static uint mapFF(float f, float min, float max){
  return (uint) 255 * ((f - min)/(max - min)) ;
}

int main(){
  PTImg gs = pt_new(256, 256, GRAYSCALE);
  for(uint x = 1; x < gs->w-1; x++){
    for(uint y = 1; y < gs->h-1; y++){
      if((x > 192 || x < 64) || (y > 192 || y < 64)){
        pt_set_color_grayscale(gs, x, y, x);
      } else {
        pt_set_color_grayscale(gs, x, y, y);
      }
    }
  }
  pt_write("./image.png", gs);
  return 0;
}
