#ifndef PT_SELECTION_H
#define PT_SELECTION_H
#include "base.h"

typedef struct { } pt_selection;
typedef pt_selection* PTSelection;

PTSelection pt_new_selection();

void pt_free_selection(PTSelection select);

#endif
