#include "structures.h"

PTQuadtree pt_qt_new(){
  PTQuadtree tree = (PTQuadtree) malloc(sizeof(pt_quadtree));
  tree->is_node = false;
  tree->children = NULL;
}

PTQuadtree pt_qt_new_with_nodes(PTQuadtree n0, PTQuadtree n1, PTQuadtree n2, PTQuadtree n3){
  PTQuadtree tree = pt_qt_new();
  PTQuadtree nodes = (PTQuadtree) malloc(4*sizeof(pt_quadtree));
  nodes[0] = *n0; nodes[1] = *n1;
  nodes[2] = *n2; nodes[3] = *n3;
  tree->is_node = true;
  tree->children = nodes;
  return tree;
}

bool pt_qt_add_nodes(PTQuadtree tree){
  if(tree->is_node)
    return false;
  tree->is_node = true;
  tree->children = (PTQuadtree) malloc(4*sizeof(pt_quadtree));
  tree->children[0].is_node = false;
  tree->children[1].is_node = false;
  tree->children[2].is_node = false;
  tree->children[3].is_node = false;
  tree->children[0].children = NULL;
  tree->children[1].children = NULL;
  tree->children[2].children = NULL;
  tree->children[3].children = NULL;
  return true;
}

static void pt_free_qt_content(PTQuadtree tree){
  if(tree->is_node){
    pt_free_qt_content(tree->children[0]);
    pt_free_qt_content(tree->children[1]);
    pt_free_qt_content(tree->children[2]);
    pt_free_qt_content(tree->children[3]);
    free(tree->children);
  }
}

void pt_free_qt(PTQuadtree tree){
  pt_free_qt_content(tree);
  free(tree);
}
