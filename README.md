# Sudoku Solver: Secuencial y Paralelo

Este proyecto implementa un solucionador de Sudokus utilizando dos enfoques: un algoritmo secuencial basado en backtracking y un enfoque paralelo utilizando OpenMP. Ambos programas están diseñados para resolver Sudokus de tamaños 9x9, 16x16 y 25x25 a partir de archivos en formato JSON.

---

## Características principales

- **Secuencial (`PosibleSecuencialDef.cpp`)**:
  - Resuelve el Sudoku utilizando un algoritmo de backtracking.
  - Procesa múltiples archivos JSON desde un directorio.
  - Genera un archivo JSON de salida con el Sudoku resuelto.
  - Diseñado para optimizar la resolución de Sudokus pequeños y medianos.

- **Paralelo (`FinalP.cpp`)**:
  - Resuelve Sudokus utilizando un enfoque paralelo con OpenMP.
  - Divide el espacio de búsqueda entre múltiples hilos, optimizando la resolución para tableros grandes.
  - Genera un archivo JSON con el resultado.
  - Diseñado para aprovechar procesadores multinúcleo.

---

## Requisitos del sistema

- **Compilador**:
  - Compatible con C++17 o superior.
  - OpenMP habilitado para la versión paralela.

- **Bibliotecas externas**:
  - [nlohmann/json](https://github.com/nlohmann/json) para leer y escribir archivos JSON.
  - `<filesystem>` para manejar directorios.

---

## Compilación

1. **Secuencial**:

   -g++ -o Secuencial PosibleSecuencialDef.cpp -std=c++17
   
    -LUEGO:
   
   -./PosibleSecuencialDef "C:\Users\rutacarpetasuya"

3. **Paralelo**:
   g++ -o Paralelo FinalP.cpp -fopenmp -std=c++17

   -LUEGO:
   
   -./FinalP.exe "C:\Users\rutacarpetasuya"
