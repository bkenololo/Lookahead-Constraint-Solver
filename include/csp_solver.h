#ifndef CSP_SOLVER_H
#define CSP_SOLVER_H

#include <stdbool.h>
#include "graph.h"

// Fungsi Constraints / DRC (Design Rule Check)
bool is_safe_placement(Grid* grid, Component* comp, int target_x, int target_y);

// Fungsi Action (Do & Undo)
void place_component(Grid* grid, Component* comp, int target_x, int target_y);
void remove_component(Grid* grid, Component* comp);

// Fungsi Pre-processing (Sanity Check)
bool is_area_sufficient(Grid* grid, Component components[], int num_components);

// Fungsi Heuristik: Forward Checking (Mencegah Domain Wipeout)
bool forward_check(Grid* grid, Component components[], int num_components, int current_index);

// Fungsi Master: Otak Rekursif Auto-Placement
bool solve_placement(Grid* grid, Component components[], int num_components, int current_index);

#endif // CSP_SOLVER_H