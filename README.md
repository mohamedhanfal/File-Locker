# File Locker

Simple C command-line tool to lock and unlock files.

Pipeline:

- lock = compress -> encrypt -> container
- unlock = container -> decrypt -> decompress

## Windows Setup (MSYS2 -> PowerShell)

This is the recommended workflow:

1. Install dependencies in MSYS2 UCRT64.
2. Build and run from PowerShell.
3. Add File Locker build path to PATH.

### 1) Install dependencies in MSYS2 UCRT64

Install MSYS2 (PowerShell):

```powershell
winget install -e --id MSYS2.MSYS2
```

Open MSYS2 UCRT64 terminal and run:

```bash
pacman -Syu --noconfirm
pacman -S --needed --noconfirm mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-zlib git
```

### 2) Build from PowerShell

Clone and enter project:

```powershell
git clone https://github.com/mohamedhanfal/File-Locker.git
cd File-Locker
```

Add UCRT64 tools to PATH (user PATH + current shell):

```powershell
$msysBin = "C:\msys64\ucrt64\bin"
$userPath = [Environment]::GetEnvironmentVariable("Path", "User")
if ($userPath -notlike "*$msysBin*") {
    [Environment]::SetEnvironmentVariable("Path", "$userPath;$msysBin", "User")
}
$env:Path += ";$msysBin"
```

Verify tools:

```powershell
gcc --version
mingw32-make --version
```

Build:

```powershell
mingw32-make clean
mingw32-make
```

Output:

```text
build/filelocker.exe
```

### 3) Add build path to PATH (PowerShell)

From project root:

```powershell
$buildPath = (Resolve-Path .\build).Path
$userPath = [Environment]::GetEnvironmentVariable("Path", "User")
if ($userPath -notlike "*$buildPath*") {
    [Environment]::SetEnvironmentVariable("Path", "$userPath;$buildPath", "User")
}
$env:Path += ";$buildPath"
filelocker.exe --help
```

## Usage

```powershell
.\build\filelocker.exe --help
.\build\filelocker.exe lock <input_file> <locker_file>
.\build\filelocker.exe unlock <locker_file> <output_file>
```

## Portability Notes

- Source code is portable. Build artifacts are machine-specific.
- Windows zlib is linked statically in current Makefile to avoid missing zlib.dll.
- If another PC still fails to run, update Microsoft Universal C Runtime (UCRT) via Windows Update.

## Other OS

Yes, this project can be built on Linux and macOS with GCC/Clang, Make, and zlib installed.

## Security Note

Current encryption is XOR. This is suitable for learning, not production security.

Repository: https://github.com/mohamedhanfal/File-Locker.git
