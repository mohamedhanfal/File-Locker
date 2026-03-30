# Makefile for File Locker (Windows/MinGW)
.RECIPEPREFIX := >

ifeq ($(OS),Windows_NT)
NULLDEV = nul
EXE_EXT = .exe
else
NULLDEV = /dev/null
EXE_EXT =
endif

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
DEBUG_FLAGS = -g -O0 -DDEBUG
LDFLAGS = -lz

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
TARGET = $(BIN_DIR)/filelocker$(EXE_EXT)

# Phony targets
.PHONY: all clean rebuild debug help

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
> @echo [LINK] Linking $(TARGET)
> $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
> @echo [SUCCESS] Build complete: $(TARGET)

# Create build directory
$(BIN_DIR):
> @mkdir -p $(BIN_DIR) 2>$(NULLDEV) || true

# Compile object files
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
> @echo [CC] Compiling $<
> @mkdir -p $(dir $@) 2>$(NULLDEV) || true
> $(CC) $(CFLAGS) -c $< -o $@

# Debug build (with symbols)
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Clean build outputs
clean:
> @echo [CLEAN] Removing build artifacts
> @rm -rf $(OBJ_DIR)/src $(OBJ_DIR)/lib $(TARGET) 2>$(NULLDEV) || true
> @echo [CLEAN] Done

# Full rebuild
rebuild: clean all

# Help
help:
> @echo "File Locker Build System"
> @echo "  make          - Build filelocker$(EXE_EXT)"
> @echo "  make debug    - Build with debug symbols"
> @echo "  make clean    - Remove build artifacts"
> @echo "  make rebuild  - Clean then build"
> @echo "  make help     - Show this message"

.SILENT: