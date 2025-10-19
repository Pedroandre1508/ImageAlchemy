@echo off
REM ============================================
REM Script de Compilacao para Windows
REM ImageAlchemy - PDI 2025
REM ============================================

echo.
echo ========================================
echo   COMPILACAO PDI - ImageAlchemy
echo ========================================
echo.

REM Verificar se esta na pasta correta
if not exist "CMakeLists.txt" (
    echo [ERRO] CMakeLists.txt nao encontrado!
    echo Por favor, execute este script na pasta pdi_code/
    pause
    exit /b 1
)

echo [1/4] Removendo build antigo...
if exist build (
    rmdir /s /q build
    echo       Build antigo removido!
) else (
    echo       Nenhum build anterior encontrado
)

echo.
echo [2/4] Criando pasta build...
mkdir build
cd build

echo.
echo [3/4] Configurando CMake...
cmake ..
if errorlevel 1 (
    echo.
    echo [ERRO] Falha na configuracao do CMake!
    echo.
    echo Possiveis causas:
    echo - OpenCV nao instalado
    echo - CMake nao encontrado no PATH
    echo.
    echo Consulte: COMPILACAO_MULTIPLATAFORMA.md
    pause
    exit /b 1
)

echo.
echo [4/4] Compilando projeto...
cmake --build . --config Release
if errorlevel 1 (
    echo.
    echo [ERRO] Falha na compilacao!
    pause
    exit /b 1
)

echo.
echo ========================================
echo   COMPILACAO CONCLUIDA COM SUCESSO!
echo ========================================
echo.
echo Executaveis gerados:
echo   - Release\pdi_m2.exe  (Apenas M2.1)
echo   - Release\pdi_code.exe (Completo)
echo.
echo Para executar:
echo   cd Release
echo   pdi_m2.exe
echo.
pause
