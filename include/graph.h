#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h> // Biar bisa pake tipe data boolean (true/false)

// ---------------------------------------------------------
// 1. VARIABLE: Komponen (Standard Cell / Macro)
// ---------------------------------------------------------
typedef struct {
    int id;             // ID unik (1, 2, 3...) biar gampang dibedain pas nge-print grid
    int width;          // Lebar kotak komponen (Sumbu X)
    int height;         // Tinggi kotak komponen (Sumbu Y)
    
    bool is_placed;     // Status apakah komponen ini udah dapet tempat
    int pos_x;          // Koordinat X (Kiri-Atas) kalau udah ditaruh
    int pos_y;          // Koordinat Y (Kiri-Atas) kalau udah ditaruh
} Component;

// ---------------------------------------------------------
// 2. DOMAIN & CONSTRAINT MAP: Silikon / Wafer Grid
// ---------------------------------------------------------
typedef struct {
    int width;          // Batas maksimal X
    int height;         // Batas maksimal Y
    int **matrix;       // Array 2D dinamis. Isi: 0 (Kosong), >0 (ID Komponen yg ngisi)
} Grid;

// ---------------------------------------------------------
// Deklarasi Fungsi Memori (Nanti diimplementasi di src/graph.c)
// ---------------------------------------------------------
Grid* create_grid(int width, int height);
void free_grid(Grid* grid);
void print_grid(Grid* grid);

// Fungsi File I/O Parser
Component* load_components(const char* filename, int* out_num_components);
Grid* load_grid(const char* filename);

#endif // GRAPH_H