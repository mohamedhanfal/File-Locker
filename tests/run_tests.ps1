$ErrorActionPreference = "Stop"

# Build
mingw32-make clean
mingw32-make

# Paths
$exe = ".\build\filelocker.exe"
$in  = ".\tests\inputs\sample.txt"
$packed = ".\tests\out\sample.packed"
$out = ".\tests\out\sample.out.txt"

# Ensure output dir exists
if (!(Test-Path ".\tests\out")) { New-Item -ItemType Directory ".\tests\out" | Out-Null }

# Run
& $exe pack $in $packed
& $exe unpack $packed $out

# Compare hashes (content)
$h1 = (Get-FileHash -Algorithm SHA256 $in).Hash
$h2 = (Get-FileHash -Algorithm SHA256 $out).Hash

if ($h1 -ne $h2) {
  Write-Host "FAIL: Output file differs from input!"
  Write-Host "IN : $h1"
  Write-Host "OUT: $h2"
  exit 1
}

Write-Host "PASS: pack/unpack produced identical output."
