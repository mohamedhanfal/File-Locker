# Makefile for File Locker (Windows/MinGW)
.RECIPEPREFIX := >

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
DEBUG_FLAGS = -g -O0 -DDEBUG

ifeq ($(OS),Windows_NT)
SHELL := cmd
LDFLAGS = -Wl,-Bstatic -lz -Wl,-Bdynamic -static-libgcc
else
LDFLAGS = -lz
endif

# Directories
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = build
BIN_DIR = build
CFLAGS += -I./include -I./lib

# Source files
SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/cli.c \
          $(SRC_DIR)/file_io.c \
          $(SRC_DIR)/container.c \
          $(SRC_DIR)/compression.c \
          $(SRC_DIR)/encryption.c

OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/filelocker.exe

# Phony targets
.PHONY: all clean rebuild debug help

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
> @echo [LINK] Linking $(TARGET)
> $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
> @echo [SUCCESS] Build complete: $(TARGET)

ifeq ($(OS),Windows_NT)
# Create build directory (cmd.exe compatible)
$(BIN_DIR):
> @if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"

# Compile object files (cmd.exe compatible)
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
> @echo [CC] Compiling $<
> @if not exist "$(dir $@)" mkdir "$(dir $@)"
> $(CC) $(CFLAGS) -c $< -o $@
else
# Create build directory
$(BIN_DIR):
> @mkdir -p $(BIN_DIR)

# Compile object files
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
> @echo [CC] Compiling $<
> @mkdir -p $(dir $@)
> $(CC) $(CFLAGS) -c $< -o $@
endif

# Debug build (with symbols)
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Clean build outputs
ifeq ($(OS),Windows_NT)
clean:
> @echo [CLEAN] Removing build artifacts
> @if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
> @if exist "$(TARGET)" del /q "$(TARGET)"
> @echo [CLEAN] Done
else
clean:
> @echo [CLEAN] Removing build artifacts
> @rm -rf $(OBJ_DIR) $(TARGET)
> @echo [CLEAN] Done
endif

# Full rebuild
rebuild: clean all

# Help
help:
> @echo "File Locker Build System"
> @echo "  make          - Build filelocker.exe"
> @echo "  make debug    - Build with debug symbols"
> @echo "  make clean    - Remove build artifacts"
> @echo "  make rebuild  - Clean then build"
> @echo "  make help     - Show this message"

.SILENT: