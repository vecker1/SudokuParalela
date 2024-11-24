#include "declaraciones_sudoku.h"
#include <cmath>
#include <omp.h>

bool esValido(std::vector<std::vector<int>>& board, int fila, int columna, int num, int gridSize, int boxSize) {
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

std::pair<int, int> EncontrarCelda(std::vector<std::vector<int>>& board, int gridSize, int boxSize) {
    int minOpcion = gridSize + 1;
    std::pair<int, int> bestCell = {-1, -1};

    for (int fila = 0; fila < gridSize; ++fila) {
        for (int columna = 0; columna < gridSize; ++columna) {
            if (board[fila][columna] == 0) {
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
                    return bestCell;
            }
        }
    }
    return bestCell;
}

bool solveSudoku(std::vector<std::vector<int>>& board, int gridSize, int boxSize) {
    std::pair<int, int> cell = EncontrarCelda(board, gridSize, boxSize);
    int fila = cell.first, columna = cell.second;

    if (fila == -1 && columna == -1)
        return true;

    for (int num = 1; num <= gridSize; ++num) {
        if (esValido(board, fila, columna, num, gridSize, boxSize)) {
            board[fila][columna] = num;
            if (solveSudoku(board, gridSize, boxSize))
                return true;
            board[fila][columna] = 0;
        }
    }
    return false;
}

bool solucionparalela(std::vector<std::vector<int>>& board, int gridSize, int boxSize) {
    bool solucion = false;

    #pragma omp parallel
    {
        #pragma omp single
        {
            std::pair<int, int> cell = EncontrarCelda(board, gridSize, boxSize);
            int fila = cell.first, columna = cell.second;

            if (fila != -1 && columna != -1) {
                for (int num = 1; num <= gridSize; ++num) {
                    if (esValido(board, fila, columna, num, gridSize, boxSize)) {
                        {
                            std::vector<std::vector<int>> localBoard = board;
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
