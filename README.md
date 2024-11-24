
# Proyecto Paralela FINAL

Este repositorio contiene un proyecto de programación paralela para resolver Sudokus de diferentes tamaños, utilizando métodos secuenciales y paralelos. El proyecto incluye múltiples componentes, archivos fuente, y ejemplos de datos.

## Estructura del Proyecto

- **Archivos Fuente**:
  - `main_secuencial.cpp`: Implementación principal del solucionador de Sudoku en modo secuencial.
  - `main_paralelo.cpp`: Implementación principal del solucionador de Sudoku en modo paralelo.
  - `secuencial.cpp`, `funciones_sudokuS.cpp`, `funciones_sudoku.cpp`: Implementación de funciones auxiliares para la solución del Sudoku.
  - `declaraciones_sudoku.h`, `declaraciones_sudokuS.h`: Archivos de cabecera con definiciones y declaraciones comunes.
  - `json.hpp`: Biblioteca para manejo de archivos JSON (nlohmann-json).

- **Datos de Entrada**:
  - `sudoku9.json`: Archivo JSON con datos de un Sudoku de tamaño 9x9.
  - `sudoku16.json`: Archivo JSON con datos de un Sudoku de tamaño 16x16.
  - `sudoku25.json`: Archivo JSON con datos de un Sudoku de tamaño 25x25.

- **Ejecutables**:
  - `main.exe`, `main_secuencial.exe`, `main_paralelo.exe`, `sudoku_solver_parallel.exe`, `sudoku_solver.exe`, `funciones_sudoku.exe`: Ejecutables generados.

- **Configuración**:
  - `.vscode/settings.json`: Configuración para Visual Studio Code.

## Requisitos

Para compilar y ejecutar este proyecto, necesitas:

1. Un compilador compatible con C++17 o superior.
2. La biblioteca [nlohmann-json](https://github.com/nlohmann/json) para manejar JSON.
3. Un entorno de desarrollo, preferiblemente [Visual Studio](https://visualstudio.microsoft.com/) o [Visual Studio Code](https://code.visualstudio.com/).

## Compilación

1. Clona este repositorio:
   ```bash
   git clone <URL-del-repositorio>
   cd <directorio-del-repositorio>
   ```

2. Compila el código:
   ```bash
   g++ -o main_secuencial main_secuencial.cpp funciones_sudokuS.cpp -std=c++17
   g++ -o main_paralelo main_paralelo.cpp funciones_sudoku.cpp -std=c++17 -fopenmp
   ```

3. Asegúrate de que los archivos `.json` estén en el mismo directorio que los ejecutables.

## Uso

Ejecuta los binarios generados:

- Para el solucionador secuencial:
  ```bash
  ./main_secuencial
  ```

- Para el solucionador paralelo:
  ```bash
  ./main_paralelo
  ```

## Notas

- El proyecto utiliza paralelismo (OpenMP) para optimizar la resolución del Sudoku en `main_paralelo.cpp`.
- Los archivos `.json` contienen configuraciones iniciales para Sudokus de distintos tamaños.
