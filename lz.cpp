#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

std::vector<std::pair<int, char>> lzCompress(const std::string &input) {
    std::unordered_map<std::string, int> dictionary;
    std::vector<std::pair<int, char>> result;
    std::string buffer;
    int dictSize = 1;

    for (size_t i = 0; i < input.length(); ++i) {
        buffer += input[i];
        if (dictionary.find(buffer) == dictionary.end()) {
            dictionary[buffer] = dictSize++;
            if (buffer.length() == 1) {
                result.push_back({0, buffer[0]});
            } else {
                std::string prefix = buffer.substr(0, buffer.length() - 1);
                result.push_back({dictionary[prefix], buffer.back()});
            }
            buffer.clear();
        }
    }

    if (!buffer.empty()) {
        if (buffer.length() == 1) {
            result.push_back({0, buffer[0]});
        } else {
            std::string prefix = buffer.substr(0, buffer.length() - 1);
            result.push_back({dictionary[prefix], buffer.back()});
        }
    }

    return result;
}

std::string lzDecompress(const std::vector<std::pair<int, char>> &compressed) {
    std::vector<std::string> dictionary(1);  // Initialize with a dummy entry for 0 index
    std::string result;

    for (const auto &pair : compressed) {
        if (pair.first == 0) {
            result += pair.second;
            dictionary.push_back(std::string(1, pair.second));
        } else {
            std::string entry = dictionary[pair.first] + pair.second;
            result += entry;
            dictionary.push_back(entry);
        }
    }

    return result;
}