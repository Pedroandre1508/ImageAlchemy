# Projeto de Processamento Digital de Imagens - PDI 2025

Este é um projeto-base em C++ que desenvolve algoritmos de processamento de imagens no contexto acadêmico.

## ⚙️ Execução

### 🐧 Linux (Rápido)
```bash
cd pdi_code/build
cmake ..
make
./pdi_m2
```

### 🪟 Windows
```powershell
cd pdi_code
mkdir build
cd build
cmake ..
cmake --build . --config Release
.\Release\pdi_m2.exe
```

### ⚠️ Problemas ao clonar do GitHub?
Se você clonou o repositório e está tendo erros de compilação, **delete a pasta `build/` e recrie**:

```bash
# Linux/macOS
rm -rf pdi_code/build
cd pdi_code && mkdir build && cd build && cmake .. && make

# Windows PowerShell
Remove-Item -Recurse -Force pdi_code\build
cd pdi_code; mkdir build; cd build; cmake ..; cmake --build . --config Release
```

📖 **[Guia Completo de Compilação Multiplataforma](COMPILACAO_MULTIPLATAFORMA.md)**