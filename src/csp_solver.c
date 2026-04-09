#include <stdio.h>
#include "../include/csp_solver.h"

// Mengecek apakah komponen bisa ditaruh di kordinat (target_x, target_y)
bool is_safe_placement(Grid* grid, Component* comp, int target_x, int target_y) {
    
    // 1. BOUNDARY CHECK (Cek batas Wafer)
    if (target_x < 0 || target_y < 0) return false;
    
    // Kalau kordinat target ditambah lebar komponen ternyata melebihi batas ujung silikon
    if (target_x + comp->width > grid->width) return false;
    if (target_y + comp->height > grid->height) return false;

    // 2. OVERLAP CHECK (Cek tabrakan dengan komponen lain)
    // Scan matriks seluas area komponen yang mau ditaruh
    for (int y = target_y; y < target_y + comp->height; y++) {
        for (int x = target_x; x < target_x + comp->width; x++) {
            
            if (grid->matrix[y][x] != 0) {
                // Ada komponen lain di sini! (Tumpang tindih / Short circuit)
                return false; 
            }
            
        }
    }

    // Kalau lolos semua ujian di atas, berarti posisinya AMAN (Valid)
    return true;
}

// ==========================================
// FUNGSI DO: Menaruh komponen di atas Wafer
// ==========================================
void place_component(Grid* grid, Component* comp, int target_x, int target_y) {
    // 1. Update state dari komponen itu sendiri
    comp->pos_x = target_x;
    comp->pos_y = target_y;
    comp->is_placed = true;

    // 2. Update peta silikon (Tandai matriks pakai ID komponen)
    for (int y = target_y; y < target_y + comp->height; y++) {
        for (int x = target_x; x < target_x + comp->width; x++) {
            grid->matrix[y][x] = comp->id;
        }
    }
}

// ==========================================
// FUNGSI UNDO: Mencabut komponen dari Wafer (Sistem Backtrack)
// ==========================================
void remove_component(Grid* grid, Component* comp) {
    // 1. Bersihkan peta silikon (Kembalikan area jadi 0 / kosong)
    for (int y = comp->pos_y; y < comp->pos_y + comp->height; y++) {
        for (int x = comp->pos_x; x < comp->pos_x + comp->width; x++) {
            grid->matrix[y][x] = 0;
        }
    }

    // 2. Reset state dari komponen
    comp->pos_x = -1; // -1 artinya tidak ada di dalam grid
    comp->pos_y = -1;
    comp->is_placed = false;
}

// ==========================================
// FUNGSI PRE-PROCESSING: Feasibility Check
// ==========================================
bool is_area_sufficient(Grid* grid, Component components[], int num_components) {
    int total_wafer_area = grid->width * grid->height;
    int total_components_area = 0;

    for (int i = 0; i < num_components; i++) {
        total_components_area += (components[i].width * components[i].height);
    }

    // Jika total area komponen lebih besar dari area wafer, kembalikan false (Mustahil)
    return total_components_area <= total_wafer_area;
}

// ==========================================
// FUNGSI HEURISTIK: Forward Checking
// ==========================================
bool forward_check(Grid* grid, Component components[], int num_components, int current_index) {
    // Scan semua komponen sisa yang BELUM dipasang
    for (int i = current_index + 1; i < num_components; i++) {
        bool has_valid_spot = false;
        
        // Cari minimal 1 spot kosong buat komponen ini
        for (int y = 0; y < grid->height; y++) {
            for (int x = 0; x < grid->width; x++) {
                if (is_safe_placement(grid, &components[i], x, y)) {
                    has_valid_spot = true;
                    break; // Ketemu 1 spot aman aja udah cukup, stop cari buat komponen ini
                }
            }
            if (has_valid_spot) break;
        }
        
        // Kalau ada SATU aja komponen sisa yang sama sekali gak punya spot...
        if (!has_valid_spot) {
            return false; // DOMAIN WIPEOUT! Potong jalur komputasi ini sekarang juga.
        }
    }
    return true; // Aman, semua komponen sisa minimal punya 1 spot harapan.
}

// ==========================================
// FUNGSI MASTER: Rekursif Auto-Placement
// ==========================================
bool solve_placement(Grid* grid, Component components[], int num_components, int current_index) {
    // 1. BASE CASE: Kalau semua komponen udah dapet kordinat (Sukses!)
    if (current_index >= num_components) {
        return true; 
    }

    Component* current_comp = &components[current_index];

    // 2. LOOP DOMAIN: Coba taruh di semua titik (x,y) yang ada di grid
    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            
            // 3. CEK DRC (Constraints)
            if (is_safe_placement(grid, current_comp, x, y)) {
                
                // 4. DO: Taruh komponen sementara
                place_component(grid, current_comp, x, y);

                // 5. FORWARD CHECKING (Senjata rahasia lu)
                if (forward_check(grid, components, num_components, current_index)) {
                    
                    // 6. REKURSI: Panggil fungsi ini lagi buat komponen selanjutnya (index + 1)
                    if (solve_placement(grid, components, num_components, current_index + 1)) {
                        return true; // Kalau anak-anaknya sukses, lapor ke atas: SUKSES!
                    }
                }

                // 7. UNDO: Kalau ternyata jalan buntu (Dead-End), cabut lagi komponennya (Backtrack)
                remove_component(grid, current_comp);
            }
        }
    }

    // Kalau loop domain udah habis dan gak ada satupun yang return true
    return false; 
}