#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <regex>
#include <unordered_map>

namespace fs = std::filesystem;
using namespace std;
using namespace chrono;

void processFile(const string& filename, unordered_map<string, pair<int, int>>& tokenMap) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        return;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    // Usar una expresión regular para extraer los tokens
    regex wordRegex("\\b\\w+\\b");
    sregex_iterator wordsBegin(content.begin(), content.end(), wordRegex);
    sregex_iterator wordsEnd;

    // Contar los tokens y su posición
    int position = 0;
    for (sregex_iterator iter = wordsBegin; iter != wordsEnd; ++iter) {
        string token = iter->str();
        tokenMap[token].first++; // Incrementar el conteo de ocurrencias
        tokenMap[token].second = position++; // Asignar la posición
    }

    file.close();
}

void writeUniqueTokens(const unordered_map<string, pair<int, int>>& tokenMap, const string& outputFileName, char separator) {
    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cerr << "Error al abrir el archivo de salida: " << outputFileName << endl;
        return;
    }

    // Escribir los tokens únicos y sus estadísticas en el archivo de salida
    for (const auto& entry : tokenMap) {
        outputFile << entry.first << separator << entry.second.first << separator << entry.second.second << endl;
    }

    outputFile.close();
}

int main() {
    auto programStart = high_resolution_clock::now();

    string outputFileName;
    cout << "Ingrese el nombre del archivo de salida: ";
    cin >> outputFileName;

    outputFileName += ".txt";

    // Ruta de la carpeta que contiene los archivos HTML
    const string folderPath = "C:\\Users\\Alberto Iwakura\\Documents\\CS13309_Archivos_HTML";

    unordered_map<string, pair<int, int>> tokenMap; // Almacena los tokens únicos y su información

    // Procesar cada archivo en la carpeta
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            string fileName = entry.path().string();
            processFile(fileName, tokenMap);
        }
    }

    // Escribir los tokens únicos en el archivo de salida con el separador elegido
    char separator;
    cout << "Seleccione el separador para las columnas (punto y coma (;), pipes (|), o tabs (\\t)): ";
    cin >> separator;

    writeUniqueTokens(tokenMap, outputFileName, separator);

    auto programStop = high_resolution_clock::now();
    auto programDuration = duration_cast<microseconds>(programStop - programStart);

    cout << "Archivo de salida generado correctamente: " << outputFileName << endl;
    cout << "Tiempo total de ejecucion: " << programDuration.count() / 1000000.0 << " segundos." << endl;

    return 0;
}
