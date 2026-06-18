param(
    [switch]$Testar,
    [switch]$Executar
)

$ErrorActionPreference = "Stop"

$fontes = Get-ChildItem -Path "src" -Filter "*.cpp" |
    Where-Object { -not ($Testar -and $_.Name -eq "main.cpp") } |
    ForEach-Object { $_.FullName }

if ($Testar) {
    $fontes += (Resolve-Path "tests/testes_regras.cpp").Path
    $saida = "termu_testes.exe"
}
else {
    $saida = "termu.exe"
}

& g++ -std=c++17 -Wall -Wextra -Wpedantic `
    -finput-charset=UTF-8 -fexec-charset=UTF-8 `
    -Iinclude @fontes -o $saida

if ($LASTEXITCODE -ne 0) {
    throw "A compilacao falhou."
}

Write-Host "Compilacao concluida: $saida"

if ($Testar -or $Executar) {
    & ".\$saida"

    if ($LASTEXITCODE -ne 0) {
        throw "A execucao terminou com erro."
    }
}
