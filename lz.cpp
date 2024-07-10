#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

// Función de compresión LZ78
std::vector<std::pair<int, char>> lzCompress(const std::string &input) {
    std::unordered_map<std::string, int> dictionary; // Diccionario para almacenar prefijos
    std::vector<std::pair<int, char>> result; // Resultado de pares (índice, carácter)
    std::string buffer; // Buffer para construir prefijos
    int dictSize = 1; // Tamaño del diccionario, empieza en 1

    for (size_t i = 0; i < input.length(); ++i) {
        buffer += input[i];
        if (dictionary.find(buffer) == dictionary.end()) {
            dictionary[buffer] = dictSize++; // Añadir el nuevo prefijo al diccionario
            if (buffer.length() == 1) {
                result.push_back({0, buffer[0]}); // Caso especial para primer carácter
            } else {
                std::string prefix = buffer.substr(0, buffer.length() - 1);
                result.push_back({dictionary[prefix], buffer.back()}); // Añadir el par (índice, carácter)
            }
            buffer.clear(); // Limpiar el buffer
        }
    }

    if (!buffer.empty()) {
        if (buffer.length() == 1) {
            result.push_back({0, buffer[0]}); // Caso especial para último carácter
        } else {
            std::string prefix = buffer.substr(0, buffer.length() - 1);
            result.push_back({dictionary[prefix], buffer.back()}); // Añadir el par (índice, carácter)
        }
    }

    return result;
}

// Función de descompresión LZ78
std::string lzDecompress(const std::vector<std::pair<int, char>> &compressed) {
    std::vector<std::string> dictionary(1);  // Inicializar con una entrada ficticia para el índice 0
    std::string result;

    for (const auto &pair : compressed) {
        if (pair.first == 0) {
            result += pair.second; // Añadir el carácter directamente
            dictionary.push_back(std::string(1, pair.second)); // Añadir al diccionario
        } else {
            std::string entry = dictionary[pair.first] + pair.second; // Construir la entrada del diccionario
            result += entry; // Añadir la entrada al resultado
            dictionary.push_back(entry); // Añadir la entrada al diccionario
        }
    }

    return result;
}

