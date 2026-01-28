$ErrorActionPreference = "Stop"

# Build
mingw32-make clean
mingw32-make

$exe = ".\build\filelocker.exe"
$inputDir = ".\tests\inputs"
$outDir = ".\tests\out"

# Create out dir if missing
if (!(Test-Path $outDir)) {
  New-Item -ItemType Directory -Path $outDir | Out-Null
}

# Pick all files in tests\inputs (non-recursive)
$files = Get-ChildItem -Path $inputDir -File
if ($files.Count -eq 0) {
  Write-Host "No input files found in $inputDir"
  exit 1
}

$failed = 0

foreach ($f in $files) {
  $inPath = $f.FullName

  # Keep per-file outputs separate
  $packedPath = Join-Path $outDir ($f.Name + ".bin")
  $outPath    = Join-Path $outDir ($f.Name)

  # Run pack/unpack
  & $exe pack $inPath $packedPath
  & $exe unpack $packedPath $outPath

  # Compare hashes (SHA256 default)
  $h1 = (Get-FileHash $inPath).Hash
  $h2 = (Get-FileHash $outPath).Hash

  if ($h1 -ne $h2) {
    Write-Host "FAIL: $($f.Name)"
    Write-Host "  IN : $h1"
    Write-Host "  OUT: $h2"
    $failed++
  } else {
    Write-Host "PASS: $($f.Name)"
  }
}

if ($failed -gt 0) {
  Write-Host "`nSome tests FAILED: $failed"
  exit 1
}

Write-Host "`nAll tests PASSED."
exit 0
