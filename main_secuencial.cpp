//Comandos de ejecucion main paralelo
//g++ -std=c++17 main_secuencial.cpp funciones_sudokuS.cpp -o main_secuencial
//./main_secuencial C:\Users\nicko\Downloads\Proyecto Paralela FINAL (directorio donde esta el archivo)

#include <iostream>
#include <filesystem>
#include "declaraciones_sudokuS.h"

using namespace std;
using namespace std::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <directorio>" << endl;
        return EXIT_FAILURE;
    }

    const string inputDir = argv[1];

    // Verificar que el directorio sea válido
    if (!is_directory(inputDir)) {
        cerr << "Error: El argumento proporcionado no es un directorio válido." << endl;
        return EXIT_FAILURE;
    }

    // Procesar cada archivo JSON en el directorio
    for (const auto& entry : directory_iterator(inputDir)) {
        if (entry.path().extension() == ".json") {
            string inputFile = entry.path().string();
            string outputFile = inputFile.substr(0, inputFile.find_last_of(".")) + "_resuelto_secuencial.json";

            int size = 0;

            try {
                cout << "Procesando archivo: " << inputFile << "\n";
                vector<vector<int>> board = load_sudoku(inputFile, size);

                cout << "Sudoku recibido (" << size << "x" << size << "):\n";

                auto start = chrono::high_resolution_clock::now();
                if (!solved_sudoku(board)) {
                    throw runtime_error("Error: El Sudoku no tiene solución.");
                }
                auto end = chrono::high_resolution_clock::now();
                chrono::duration<double> sequential_time = end - start;

                cout << "\nSudoku resuelto (Secuencial):\n";
                cout << "Tiempo de ejecución: " << sequential_time.count() << " segundos\n";

                // Guardar el resultado en un archivo con formato visual
                save_sudoku_visual(board, outputFile);

            } catch (const exception& e) {
                cerr << e.what() << "\n";
            }
        }
    }

    return 0;
}
