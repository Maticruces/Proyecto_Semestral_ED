#include <string>
#include <vector>
#include <unordered_map>

std::vector<std::pair<int, int>> lzCompress(const std::string &input) {
    std::unordered_map<std::string, int> dictionary;
    std::vector<std::pair<int, int>> result;
    std::string buffer;
    
    for (size_t i = 0; i < input.length(); ++i) {
        buffer += input[i];
        if (dictionary.find(buffer) == dictionary.end()) {
            if (buffer.length() > 1) {
                std::string prefix = buffer.substr(0, buffer.length() - 1);
                result.push_back({dictionary[prefix], static_cast<int>(buffer.length() - 1)});
                dictionary[buffer] = i - buffer.length() + 1;
                buffer = buffer.substr(buffer.length() - 1);
            }
            dictionary[buffer] = i - buffer.length() + 1;
            result.push_back({buffer[0], 0});
            buffer.clear();
        }
    }
    
    if (!buffer.empty()) {
        result.push_back({dictionary[buffer], static_cast<int>(buffer.length())});
    }
    
    return result;
}

std::string lzDecompress(const std::vector<std::pair<int, int>> &compressed) {
    std::string result;
    std::string input; // Necesitamos el input original para decodificar
    int currentIndex = 0; // Índice para seguir la posición en el input original

    for (const auto &pair : compressed) {
        if (pair.second == 0) {
            result += input[pair.first]; // Agrega el caracter del input original
            input += input[pair.first]; // Agrega el caracter al input
            currentIndex++;
        } else {
            std::string substring = input.substr(pair.first, pair.second); // Obtiene el substring del input original
            result += substring; // Agrega el substring al resultado
            input += substring; // Agrega el substring al input
            currentIndex += pair.second;
        }
    }

    return result;
}
