$BUILD_DIR = "build"

if (-Not (Test-Path -Path $BUILD_DIR)) {
    New-Item -Path $BUILD_DIR -ItemType Directory
}

# Change to the build directory
Set-Location -Path $BUILD_DIR

# Run CMake to configure the project
cmake ..


cmake --build . --config Debug

$exePath = ".\Tests\Debug\my_tests.exe"

if (Test-Path $exePath) {
    & $exePath
} else {
    Write-Host "Executable not found!"
}

Write-Host "Build completed successfully!"

Set-Location ..