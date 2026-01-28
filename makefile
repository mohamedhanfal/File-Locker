CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2 -Iinclude
TARGET = build/filelocker.exe

SRCS = src/main.c src/cli.c src/file_io.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@if not exist build mkdir build
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-del /q $(OBJS) 2>nul
	-del /q $(TARGET) 2>nul

.PHONY: all clean
