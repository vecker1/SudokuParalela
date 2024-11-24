#ifndef DECLARACIONES_SUDOKU_H
#define DECLARACIONES_SUDOKU_H

#include <vector>
#include <string>

// Verifica si un número es válido en una posición específica del tablero
bool esValido(std::vector<std::vector<int>>& board, int fila, int columna, int num, int gridSize, int boxSize);

// Encuentra la celda con menos opciones (MRV - Minimum Remaining Values)
std::pair<int, int> EncontrarCelda(std::vector<std::vector<int>>& board, int gridSize, int boxSize);

// Función recursiva para resolver el Sudoku
bool solveSudoku(std::vector<std::vector<int>>& board, int gridSize, int boxSize);

// Función paralela para resolver el Sudoku
bool solucionparalela(std::vector<std::vector<int>>& board, int gridSize, int boxSize);

#endif // DECLARACIONES_SUDOKU_H
