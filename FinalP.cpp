//    g++ -o FinalP FinalP.cpp -fopenmp -std=c++17 
//     ./FinalP.exe "C:\Users\micha\OneDrive\Escritorio\Codigos_Sudoku"

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <omp.h>
#include <filesystem>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

// Función para verificar si un número es válido en una posición específica
bool esValido(vector<vector<int>> &board, int fila, int columna, int num, int gridSize, int boxSize) {
    for (int x = 0; x < gridSize; ++x) {
        if (board[fila][x] == num || board[x][columna] == num)
            return false;
    }
    int iniciofila = (fila / boxSize) * boxSize;
    int iniciocolumna = (columna / boxSize) * boxSize;
    for (int i = 0; i < boxSize; ++i) {
        for (int j = 0; j < boxSize; ++j) {
            if (board[iniciofila + i][iniciocolumna + j] == num)
                return false;
        }
    }
    return true;
}

// Encuentra la celda con menos opciones (MRV - Minimum Remaining Values)
pair<int, int> EncontrarCelda(vector<vector<int>> &board, int gridSize, int boxSize) {
    int minOpcion = gridSize + 1;
    pair<int, int> bestCell = {-1, -1};

    for (int fila = 0; fila < gridSize; ++fila) {
        for (int columna = 0; columna < gridSize; ++columna) {
            if (board[fila][columna] == 0) { // Celda vacía
                int opcion = 0;
                for (int num = 1; num <= gridSize; ++num) {
                    if (esValido(board, fila, columna, num, gridSize, boxSize))
                        opcion++;
                }
                if (opcion < minOpcion) {
                    minOpcion = opcion;
                    bestCell = {fila, columna};
                }
                if (minOpcion == 1)
                    return bestCell; // Devuelve inmediatamente si solo hay una opción
            }
        }
    }
    return bestCell;
}

// Función recursiva para resolver el Sudoku
bool solveSudoku(vector<vector<int>> &board, int gridSize, int boxSize) {
    pair<int, int> cell = EncontrarCelda(board, gridSize, boxSize);
    int fila = cell.first, columna = cell.second;

    if (fila == -1 && columna == -1)
        return true; // Tablero completo

    for (int num = 1; num <= gridSize; ++num) {
        if (esValido(board, fila, columna, num, gridSize, boxSize)) {
            board[fila][columna] = num;
            if (solveSudoku(board, gridSize, boxSize))
                return true;
            board[fila][columna] = 0; // Retrocede si no se puede resolver
        }
    }
    return false;
}

// Función paralela para resolver el Sudoku
bool solucionparalela(vector<vector<int>> &board, int gridSize, int boxSize) {
    bool solucion = false;

    #pragma omp parallel
    {
        #pragma omp single
        {
            pair<int, int> cell = EncontrarCelda(board, gridSize, boxSize);
            int fila = cell.first, columna = cell.second;

            if (fila != -1 && columna != -1) {
                for (int num = 1; num <= gridSize; ++num) {
                    if (esValido(board, fila, columna, num, gridSize, boxSize)) {
                        {
                            vector<vector<int>> localBoard = board;
                            localBoard[fila][columna] = num;
                            if (solveSudoku(localBoard, gridSize, boxSize)) {
                                #pragma omp critical
                                {
                                    if (!solucion) {
                                        board = localBoard;
                                        solucion = true;
                                    }
                                }
                            }
                        }
                    }
                }
                #pragma omp taskwait
            }
        }
    }

    return solucion;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <directorio>" << endl;
        return 1;
    }

    string inputDir = argv[1];

    // Verificar que el directorio es válido
    if (!fs::is_directory(inputDir)) {
        cerr << "Error: El argumento proporcionado no es un directorio valido." << endl;
        return 1;
    }

    omp_set_num_threads(omp_get_max_threads());

    // Procesar cada archivo JSON en el directorio
    for (const auto &entry : fs::directory_iterator(inputDir)) {
        if (entry.path().extension() == ".json") {
            string inputFile = entry.path().string();
            string outputFile = inputFile.substr(0, inputFile.find_last_of(".")) + "_resuelto_paralelo.json";

            // Leer el archivo JSON
            ifstream inFile(inputFile);
            if (!inFile.is_open()) {
                cerr << "Error: No se pudo abrir el archivo " << inputFile << endl;
                continue;
            }

            json inputJson;
            try {
                inFile >> inputJson;
            } catch (json::parse_error &e) {
                cerr << "Error: JSON mal formado en el archivo " << inputFile << endl;
                continue;
            }
            inFile.close();

            // Convertir el JSON a un vector 2D
            if (!inputJson.contains("board") || !inputJson["board"].is_array()) {
                cerr << "Error: El JSON no contiene un tablero válido en la clave 'board'." << endl;
                continue;
            }
            vector<vector<int>> board = inputJson["board"];
            int gridSize = board.size();
            int boxSize = sqrt(gridSize);

            // Verificar que el tablero es válido
            if (boxSize * boxSize != gridSize) {
                cerr << "Error: El tablero en " << inputFile << " no tiene un tamaño valido." << endl;
                continue;
            }

            // Resolver el Sudoku
            double start = omp_get_wtime();
            if (solucionparalela(board, gridSize, boxSize)) {
                double end = omp_get_wtime();
                cout << "Sudoku resuelto en " << (end - start) << " segundos en " << inputFile << endl;

                // Guardar el resultado en un archivo con formato visual
                ofstream outFile(outputFile);
                if (!outFile.is_open()) {
                    cerr << "Error: No se pudo abrir el archivo de salida " << outputFile << endl;
                    continue;
                }

                // Escribir el JSON con formato visual
                outFile << "{\n  \"board\": [\n";

                for (int i = 0; i < gridSize; ++i) {
                    outFile << "    [";
                    for (int j = 0; j < gridSize; ++j) {
                        outFile << board[i][j];
                        if (j < gridSize - 1) outFile << ", ";

                        // Agregar separador vertical entre subcuadrículas
                        if ((j + 1) % boxSize == 0 && j + 1 < gridSize)
                            outFile << "  ";
                    }
                    outFile << "]";
                    if (i < gridSize - 1) outFile << ",";

                    // Agregar separador horizontal entre subcuadrículas
                    outFile << "\n";
                    if ((i + 1) % boxSize == 0 && i + 1 < gridSize)
                        outFile << "    " << "\n";
                }

                outFile << "  ]\n}\n";
                outFile.close();

                cout << "El Sudoku resuelto se guardó en " << outputFile << endl;
            } else {
                cout << "No se pudo resolver el Sudoku en " << inputFile << endl;
            }
        }
    }

    return 0;
}