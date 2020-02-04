#include <stdio.h>
#include "../src/pictools.h"

int main(){
  PTImg img = pt_load("./res/red_circles_64.png", RGB);
  PTImg gs = pt_copy_to_grayscale(img);
  printf("%d %d %d %d\n", img->w, img->h, img->channels, img->mode);
  printf("%d %d %d %d\n", gs->w, gs->h, gs->channels, gs->mode);
  pt_write("./image.png", gs);
  return 0;
}
