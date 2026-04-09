# ==========================================
# Makefile for EDA Algorithm Explorer
# ==========================================

# 1. Konfigurasi Compiler
CC = gcc
# Flags: -Wall -Wextra (nyalain semua warning), -g (biar bisa di-debug), -Iinclude (kasih tau lokasi file .h)
CFLAGS = -Wall -Wextra -g -Iinclude

# 2. Direktori
SRC_DIR = src
BIN_DIR = bin

# 3. File yang mau di-compile
# Ini otomatis nyari semua file .c yang ada di dalam folder src/
SRCS = $(wildcard $(SRC_DIR)/*.c)

# 4. Nama file output (Executable)
# Kalau lu pakai Windows, outputnya otomatis jadi eda_solver.exe
TARGET = $(BIN_DIR)/eda_solver

# ==========================================
# Rules / Perintah Utama
# ==========================================

# 'all' adalah target default. Kalau lu cuma ngetik 'make', dia bakal ngejalanin ini.
all: $(TARGET)

# Cara ngebangun executable-nya
$(TARGET): $(SRCS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^
	@echo ">> Build successful! Executable is in $(TARGET)"

# Perintah 'make run' buat langsung compile & jalanin programnya
run: all
	@echo ">> Running EDA Solver..."
	@./$(TARGET)

# Perintah 'make clean' buat nghapus file hasil compile biar bersih
clean:
	@echo ">> Cleaning build files..."
	rm -f $(TARGET)
	rm -f $(TARGET).exe

# Biar Make gak bingung kalau kebetulan ada folder namanya 'all', 'run', atau 'clean'
.PHONY: all run clean