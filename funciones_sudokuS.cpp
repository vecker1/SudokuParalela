#include "declaraciones_sudokuS.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

bool es_valido(const vector<vector<int>>& board, int fila, int columna, int num) {
    int size = board.size();
    int box_size = static_cast<int>(sqrt(size));

    for (int i = 0; i < size; i++) {
        if (board[fila][i] == num || board[i][columna] == num) {
            return false;
        }
    }

    int iniciofila = fila - fila % box_size, iniciocolumna = columna - columna % box_size;
    for (int i = iniciofila; i < iniciofila + box_size; i++) {
        for (int j = iniciocolumna; j < iniciocolumna + box_size; j++) {
            if (board[i][j] == num) {
                return false;
            }
        }
    }

    return true;
}

bool solved_sudoku(vector<vector<int>>& board) {
    int size = board.size();
    for (int fila = 0; fila < size; fila++) {
        for (int columna = 0; columna < size; columna++) {
            if (board[fila][columna] == 0) {
                for (int num = 1; num <= size; num++) {
                    if (es_valido(board, fila, columna, num)) {
                        board[fila][columna] = num;
                        if (solved_sudoku(board)) {
                            return true;
                        }
                        board[fila][columna] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

vector<vector<int>> load_sudoku(const string& nombre, int& size) {
    ifstream file(nombre);
    if (!file.is_open()) {
        throw runtime_error("Error: No se pudo abrir el archivo " + nombre);
    }
    json j;
    file >> j;

    vector<vector<int>> board = j["board"].get<vector<vector<int>>>();
    size = board.size();

    if (size != 9 && size != 16 && size != 25) {
        throw runtime_error("Error: Tamaño de Sudoku no válido. Debe ser 9x9, 16x16 o 25x25.");
    }

    return board;
}

void save_sudoku_visual(const vector<vector<int>>& board, const string& nombre) {
    ofstream file(nombre);
    if (!file.is_open()) {
        throw runtime_error("Error: No se pudo crear el archivo " + nombre);
    }

    int gridSize = board.size();
    int boxSize = static_cast<int>(sqrt(gridSize));

    file << "{\n  \"board\": [\n";
    for (int i = 0; i < gridSize; ++i) {
        file << "    [";
        for (int j = 0; j < gridSize; ++j) {
            file << board[i][j];
            if (j < gridSize - 1) file << ", ";

            // Agregar separador vertical entre subcuadrículas
            if ((j + 1) % boxSize == 0 && j + 1 < gridSize) file << "  ";
        }
        file << "]";
        if (i < gridSize - 1) file << ",";

        file << "\n";

        // Agregar separadores horizontales entre subcuadrículas
        if ((i + 1) % boxSize == 0 && i + 1 < gridSize) {
            file << "    " << "\n";
        }
    }
    file << "  ]\n}\n";

    file.close();
    cout << "\nEl Sudoku resuelto se ha guardado en: " << nombre << "\n";
}
