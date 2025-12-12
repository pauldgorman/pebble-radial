$compileCommand = "docker run --rm -v ""${PWD}:/code"" -w /code gcc:latest gcc -I tests -o tests/runner tests/test_runner.c src/c/radial_geometry.c"
Invoke-Expression $compileCommand

if ($LASTEXITCODE -eq 0) {
    Write-Host "Compilation successful. Running tests..."
    $runCommand = "docker run --rm -v ""${PWD}:/code"" -w /code gcc:latest ./tests/runner"
    Invoke-Expression $runCommand
}
else {
    Write-Error "Compilation failed."
}
