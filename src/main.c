#include <stdio.h>
#include <stdlib.h>
#include "../include/graph.h"
#include "../include/csp_solver.h"

int main() {
    printf("\n>> Initializing EDA Auto-Placement Engine...\n");

    // ==========================================
    // DATA EXTRACTION (Dari File .txt ke RAM)
    // ==========================================
    Grid* silicon_grid = load_grid("data/test_grid_1.txt");
    if (silicon_grid == NULL) {
        return 1; // Abort kalau file grid gak ketemu
    }

    int num_components = 0;
    Component* my_netlist = load_components("data/test_netlist.txt", &num_components);
    
    if (my_netlist == NULL) {
        free_grid(silicon_grid);
        return 1; // Abort kalau file netlist gak ketemu
    }

    printf(">> Berhasil memuat Wafer Grid ukuran %dx%d\n", silicon_grid->width, silicon_grid->height);
    printf(">> Berhasil memuat %d komponen dari netlist\n", num_components);

    // ==========================================
    // SANITY CHECK & ENGINE EXECUTION
    // ==========================================
    if (!is_area_sufficient(silicon_grid, my_netlist, num_components)) {
        printf(">> [FATAL ERROR] Total area komponen melebihi Wafer!\n");
    } else {
        printf(">> [OK] Sanity Check Lulus. Menjalankan Solver...\n");

        if (solve_placement(silicon_grid, my_netlist, num_components, 0)) {
            printf("\n>> [SUCCESS] Konfigurasi Placement Sempurna Ditemukan!\n");
            print_grid(silicon_grid);
        } else {
            printf("\n>> [FAILED] Tidak ada konfigurasi yang memungkinkan (Unroutable)!\n");
        }
    }

    // ==========================================
    // CLEANUP MEMORY
    // ==========================================
    free(my_netlist); 
    free_grid(silicon_grid); 
    
    printf(">> Engine shutdown gracefully.\n");
    return 0;
}