//Comandos de ejecucion main paralelo
//g++ -std=c++17 -fopenmp main_paralelo.cpp funciones_sudoku.cpp -o main_paralelo
//./main_paralelo "C:\Users\nicko\Downloads\Proyecto Paralela FINAL"(directorio donde esta el archivo)


#include <iostream>
#include <fstream>
#include <filesystem>
#include <omp.h>
#include "declaraciones_sudoku.h"
#include "json.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

// Función para guardar el resultado del Sudoku en un formato visual JSON
void guardar_en_json(const std::string& archivo, const std::vector<std::vector<int>>& board) {
    std::ofstream salida(archivo);
    if (!salida.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo para guardar " + archivo);
    }

    // Escribir el JSON manualmente con el formato deseado
    salida << "{\n  \"board\": [\n";

    for (size_t i = 0; i < board.size(); ++i) {
        salida << "    [";
        for (size_t j = 0; j < board[i].size(); ++j) {
            salida << board[i][j];
            if (j < board[i].size() - 1) salida << ", ";
        }
        salida << "]";
        if (i < board.size() - 1) salida << ",";
        salida << "\n";
    }

    salida << "  ]\n}";
    salida.close();

    std::cout << "Archivo de salida generado correctamente: " << archivo << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <directorio>" << std::endl;
        return 1;
    }

    std::string inputDir = argv[1];

    if (!fs::is_directory(inputDir)) {
        std::cerr << "Error: El argumento proporcionado no es un directorio válido." << std::endl;
        return 1;
    }

    omp_set_num_threads(omp_get_max_threads());

    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (entry.path().extension() == ".json") {
            std::string inputFile = entry.path().string();
            std::string outputFile = inputFile.substr(0, inputFile.find_last_of(".")) + "_resuelto_paralelo.json";

            std::ifstream inFile(inputFile);
            if (!inFile.is_open()) {
                std::cerr << "Error: No se pudo abrir el archivo " << inputFile << std::endl;
                continue;
            }

            json inputJson;
            try {
                inFile >> inputJson;
            } catch (json::parse_error& e) {
                std::cerr << "Error: JSON mal formado en el archivo " << inputFile << std::endl;
                continue;
            }
            inFile.close();

            if (!inputJson.contains("board") || !inputJson["board"].is_array()) {
                std::cerr << "Error: El JSON no contiene un tablero válido en la clave 'board'." << std::endl;
                continue;
            }
            std::vector<std::vector<int>> board = inputJson["board"];
            int gridSize = board.size();
            int boxSize = sqrt(gridSize);

            if (boxSize * boxSize != gridSize) {
                std::cerr << "Error: El tablero en " << inputFile << " no tiene un tamaño válido." << std::endl;
                continue;
            }

            double start = omp_get_wtime();
            if (solucionparalela(board, gridSize, boxSize)) {
                double end = omp_get_wtime();
                std::cout << "Sudoku resuelto en " << (end - start) << " segundos en " << inputFile << std::endl;

                try {
                    guardar_en_json(outputFile, board);
                } catch (const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                }
            } else {
                std::cout << "No se pudo resolver el Sudoku en " << inputFile << std::endl;
            }
        }
    }

    return 0;
}
