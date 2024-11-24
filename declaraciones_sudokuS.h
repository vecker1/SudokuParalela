#ifndef DECLARACIONES_SUDOKUS_H
#define DECLARACIONES_SUDOKUS_H

#include <vector>
#include <string>

// Verifica si un número es válido en una posición específica del tablero
bool es_valido(const std::vector<std::vector<int>>& board, int fila, int columna, int num);

// Resuelve el Sudoku recursivamente
bool solved_sudoku(std::vector<std::vector<int>>& board);

// Carga un Sudoku desde un archivo JSON
std::vector<std::vector<int>> load_sudoku(const std::string& nombre, int& size);

// Guarda un Sudoku resuelto en formato visual JSON
void save_sudoku_visual(const std::vector<std::vector<int>>& board, const std::string& nombre);

#endif // DECLARACIONES_SUDOKUS_H
