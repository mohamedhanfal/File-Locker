# File Locker

Simple C CLI tool to lock and unlock files.

Pipeline:

- lock = compress -> encrypt -> container
- unlock = container -> decrypt -> decompress

## Requirements

- C compiler (GCC/Clang)
- GNU Make
- zlib (`-lz`)

## Quick Start (Copy-Paste)

### Windows (MSYS2 + UCRT64)

Install MSYS2 (PowerShell):

```powershell
winget install -e --id MSYS2.MSYS2
```

Open **MSYS2 UCRT64** terminal, then run:

```bash
pacman -Syu --noconfirm
pacman -S --needed --noconfirm mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-zlib make git
git clone https://github.com/mohamedhanfal/File-Locker.git
cd File-Locker
make clean
make
./build/filelocker.exe --help
```

### Ubuntu/Debian Linux

```bash
sudo apt update
sudo apt install -y build-essential make zlib1g-dev git
git clone https://github.com/mohamedhanfal/File-Locker.git
cd File-Locker
make clean
make
./build/filelocker --help
```

### macOS (Homebrew)

```bash
xcode-select --install
brew install make gcc zlib git
git clone https://github.com/mohamedhanfal/File-Locker.git
cd File-Locker
make clean
make
./build/filelocker --help
```

## Build

```bash
make clean
make
```

Output:

```text
build/filelocker.exe (Windows)
build/filelocker (Linux/macOS)
```

## Usage

Windows:

```powershell
.\build\filelocker.exe --help
.\build\filelocker.exe lock <input_file> <locker_file>
.\build\filelocker.exe unlock <locker_file> <output_file>
```

Linux/macOS:

```bash
./build/filelocker --help
./build/filelocker lock <input_file> <locker_file>
./build/filelocker unlock <locker_file> <output_file>
```

## OS Portability (Windows)

This project is intended to be portable across Windows systems by sharing source code, not compiled binaries.

What is portable:

- `src/`, `include/`, `Makefile`, and project config

What is not portable:

- Files in `build/` (machine/toolchain-specific artifacts)

Also portable to Linux and macOS with GCC/Clang, Make, and zlib installed.

## Notes

- Output `.lock` files are marked hidden on Windows.
- Current encryption is XOR and is not secure for production use.

Repository: https://github.com/mohamedhanfal/File-Locker.git
