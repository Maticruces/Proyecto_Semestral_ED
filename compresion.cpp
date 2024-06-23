#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>

std::string compressLZ(const std::string& input) {
    std::unordered_map<std::string, int> dictionary;
    std::string compressed;

    int nextCode = 0;
    std::string current;

    for (char c : input) {
        std::string newCurrent = current + c;

        if (dictionary.count(newCurrent) > 0) {
            current = newCurrent;
        } else {
            compressed += "(" + std::to_string(dictionary[current]) + "," + std::to_string(current.length() - 1) + ")";
            dictionary[newCurrent] = nextCode++;
            current = std::string(1, c);
        }
    }

    compressed += "(" + std::to_string(dictionary[current]) + "," + std::to_string(current.length() - 1) + ")";

    return compressed;
}

void createZipFile(const std::string& filename, const std::string& content) {
    // Create a temporary file to store the content
    std::string tempFilename = "temp.txt";
    std::ofstream tempFile(tempFilename);
    tempFile << content;
    tempFile.close();

    // Create a zip archive
    zip_t* archive = zip_open(filename.c_str(), ZIP_CREATE | ZIP_TRUNCATE, nullptr);
    if (archive == nullptr) {
        std::cerr << "Failed to create the zip archive" << std::endl;
        return;
    }

    // Add the temporary file to the archive
    zip_source_t* source = zip_source_file(archive, tempFilename.c_str(), nullptr, nullptr);
    if (source == nullptr) {
        std::cerr << "Failed to add file to the zip archive" << std::endl;
        zip_close(archive);
        return;
    }

    // Set the file name in the archive
    std::filesystem::path filePath(tempFilename);
    std::string entryName = filePath.filename().string();
    zip_file_t* file = zip_fopen(archive, entryName.c_str(), ZIP_FL_OVERWRITE);
    if (file == nullptr) {
        std::cerr << "Failed to set file name in the zip archive" << std::endl;
        zip_source_free(source);
        zip_close(archive);
        return;
    }

    // Write the content to the file in the archive
    const void* buffer;
    zip_uint64_t bufferSize;
    zip_source_get_buffer(source, &buffer, &bufferSize);
    zip_fwrite(file, buffer, bufferSize);

    // Close the file and the archive
    zip_fclose(file);
    zip_source_free(source);
    zip_close(archive);

    // Remove the temporary file
    std::filesystem::remove(tempFilename);
}

int main() {
    std::string input;
    std::cout << "Enter the text to compress: ";
    std::getline(std::cin, input);

    std::string compressed = compressLZ(input);

    std::cout << "Compressed: " << compressed << std::endl;

    std::string filename;
    std::cout << "Enter the name of the zip file: ";
    std::getline(std::cin, filename);

    createZipFile(filename, compressed);

    std::cout << "Zip file created successfully" << std::endl;

    return 0;
}