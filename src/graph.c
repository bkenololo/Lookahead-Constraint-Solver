#include <stdio.h>
#include <stdlib.h>
#include "../include/graph.h"

// Membangun grid silikon dinamis
Grid* create_grid(int width, int height) {
    // 1. Alokasi memori untuk struct Grid itu sendiri
    Grid* grid = (Grid*)malloc(sizeof(Grid));
    grid->width = width;
    grid->height = height;

    // 2. Alokasi array of pointers (untuk baris/Y)
    grid->matrix = (int**)malloc(height * sizeof(int*));
    
    // 3. Alokasi memori untuk tiap kolom (X) pakai calloc biar isinya 0 semua
    for (int y = 0; y < height; y++) {
        grid->matrix[y] = (int*)calloc(width, sizeof(int));
    }

    return grid;
}

// Menghancurkan grid dari memori (PENTING biar gak memory leak)
void free_grid(Grid* grid) {
    if (grid == NULL) return;
    
    for (int y = 0; y < grid->height; y++) {
        free(grid->matrix[y]); // Free tiap baris dulu
    }
    free(grid->matrix);        // Free array of pointers
    free(grid);                // Free struct utamanya
}

// Visualisasi grid ala terminal hacker
void print_grid(Grid* grid) {
    printf("\n=== SILICON WAFER GRID (%dx%d) ===\n", grid->width, grid->height);
    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (grid->matrix[y][x] == 0) {
                printf(" . "); // Titik artinya area kosong
            } else {
                printf("[%d]", grid->matrix[y][x]); // Angka artinya ID komponen
            }
        }
        printf("\n");
    }
    printf("==================================\n\n");
}

// ==========================================
// FUNGSI PARSER: Membaca Netlist dari File .txt
// ==========================================
Component* load_components(const char* filename, int* out_num_components) {
    // 1. Buka file dalam mode Read ("r")
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf(">> [FATAL ERROR] Gagal membuka file: %s\n", filename);
        *out_num_components = 0;
        return NULL;
    }

    // 2. Baca baris pertama (Total komponen)
    int count;
    fscanf(file, "%d", &count);
    *out_num_components = count; // Kirim nilai count ke luar fungsi lewat pointer

    // 3. Minta memori ke RAM sebesar (Jumlah Komponen x Ukuran 1 Komponen)
    Component* array = (Component*)malloc(count * sizeof(Component));

    // 4. Baca baris-baris selanjutnya (W dan L)
    for (int i = 0; i < count; i++) {
        int w, h;
        fscanf(file, "%d %d", &w, &h);
        
        array[i].id = i + 1; // Auto-generate ID: 1, 2, 3...
        array[i].width = w;
        array[i].height = h;
        
        // Default state (Belum ditaruh)
        array[i].is_placed = false;
        array[i].pos_x = -1;
        array[i].pos_y = -1;
    }

    // 5. Tutup file biar memori disk gak bocor
    fclose(file);
    
    return array; // Kembalikan pointer array-nya ke main.c
}

// ==========================================
// FUNGSI PARSER: Membaca Ukuran Grid dari File .txt
// ==========================================
Grid* load_grid(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf(">> [FATAL ERROR] Gagal membuka file grid: %s\n", filename);
        return NULL;
    }

    int width, height;
    fscanf(file, "%d %d", &width, &height);
    fclose(file);

    // Langsung passing angkanya ke fungsi alokasi memori yang udah ada
    return create_grid(width, height);
}