# File Locker

A lightweight C command-line tool to lock and unlock files using compression, XOR encryption, and a custom FLK1 container format.

## Features

- `lock` command: compress -> encrypt -> containerize
- `unlock` command: container -> decrypt -> decompress
- Hidden password input in terminal
- Binary-safe file I/O
- Windows hidden-file support for generated `.lock` files
- zlib-based compression

## Project Structure

```text
File Locker/
|-- src/
|   |-- main.c
|   |-- cli.c
|   |-- file_io.c
|   |-- compression.c
|   |-- encryption.c
|   |-- container.c
|-- include/
|   |-- cli.h
|   |-- file_io.h
|   |-- compression.h
|   |-- encryption.h
|   |-- container.h
|-- tests/
|-- build/
|-- Makefile
|-- README.md
```

## Requirements

- GCC or MinGW
- Make
- zlib development library

## Build

```powershell
make clean
make
```

Output executable:

```text
build/filelocker.exe
```

## Usage

Show help:

```powershell
.\build\filelocker.exe --help
```

Lock a file:

```powershell
.\build\filelocker.exe lock <input_file> <locker_file>
```

Example:

```powershell
.\build\filelocker.exe lock tests\test.txt tests\test.lock
```

Unlock a file:

```powershell
.\build\filelocker.exe unlock <locker_file> <output_file>
```

Example:

```powershell
.\build\filelocker.exe unlock tests\test.lock tests\test.out.txt
```

## Integrity Verification (Round Trip)

After locking and unlocking, verify the files are identical:

```powershell
$h1 = (Get-FileHash tests\test.txt -Algorithm SHA256).Hash
$h2 = (Get-FileHash tests\test.out.txt -Algorithm SHA256).Hash
$h1
$h2
$h1 -eq $h2
```

Expected result:

```text
True
```

## How It Works

### Lock Pipeline

1. Read input file into memory
2. Compress with zlib
3. Encrypt compressed bytes using XOR password
4. Write FLK1 header + payload to output `.lock` file
5. On Windows, set output as hidden file

### Unlock Pipeline

1. Read and validate FLK1 container
2. Decrypt payload using XOR password
3. Decompress to original size
4. Write restored output file

## FLK1 Container Header

The file format stores:

- Magic: `FLK1`
- Version: `1`
- Flags: reserved (`0`)
- Payload size: encrypted/compressed data size
- Original size: source file size before processing

## Security Note

Current encryption uses XOR, which is suitable for learning and prototypes but **not secure for production use**.

For production-grade security, upgrade to:

- AES-256 encryption
- Password-based key derivation (PBKDF2, Argon2, or scrypt)
- Authenticated encryption mode (AES-GCM or ChaCha20-Poly1305)

## Known Limitations

- Small files may become larger after locking (header + compression overhead)
- Whole-file memory buffering is not ideal for very large files
- No recursive folder mode in current CLI

## Future Improvements

- Strong cryptography (AES + KDF)
- Better CLI UX (batch modes, force overwrite, verbose mode)
- Stream-based processing for large files
- Automated test suite and CI integration

## Troubleshooting

- `Compression failed`:
  - Check zlib linkage in build configuration.
- `Failed to read lock container`:
  - Wrong file path or invalid/corrupted `.lock` file.
- `Decompression failed`:
  - Possible wrong password or corrupted container.
- Empty output:
  - Ensure correct password and valid input file.

## My github repository link

https://github.com/mohamedhanfal/File-Locker.git
