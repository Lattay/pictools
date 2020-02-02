#ifndef PT_STRUCTURES_H
#define PT_STRUCTURES_H
#include "base.h"

typedef struct pt_quadtree_s {
  bool is_node;
  union {
    struct pt_quadtree_s* children; // point to a 4 elements array or NULL
    union {
      pt_color leaf;
      pt_color_a leaf_a;
      pt_uc leaf_c;
    };
  };
} pt_quadtree;
typedef pt_quadtree* PTQuadtree;

PTQuadtree pt_qt_new();
PTQuadtree pt_qt_new_with_nodes(PTQuadtree n0, PTQuadtree n1, PTQuadtree n2, PTQuadtree n3);
void pt_qt_add_nodes(PTQuadtree tree);

void pt_qt_free(PTQuadtree tree);


#endif
