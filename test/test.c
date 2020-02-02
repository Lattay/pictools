#include <stdio.h>
#include "../src/pictools.h"

int main(){
  PTImg img = pt_load("./res/red_circles_64.png");
  pt_color cl = pt_get_color(img, 10, 12);
  printf("%d, %d, %d\n", cl.r, cl.g, cl.b);
  return 0;
}
