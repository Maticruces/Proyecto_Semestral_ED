#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>
#include "huffman.cpp"
#include "lz.cpp"

// Función para obtener el uso de memoria en bytes utilizando new/delete
size_t getMemoryUsage() {
    const size_t blockSize = 1024 * 1024;
    std::vector<char*> blocks;

    // Consumir memoria en bloques de 1 MB
    while (true) {
        try {
            blocks.push_back(new char[blockSize]);
        } catch (const std::bad_alloc&) {
            break;
        }
    }

    // Liberar memoria
    for (char* block : blocks) {
        delete[] block;
    }

    return blocks.size() * blockSize;
}

void runExperiments(const std::string& text, int num) {
    using namespace std::chrono;

    // Medir el tiempo y el tamaño de Huffman Encoding
    auto start = high_resolution_clock::now();
    size_t initialMemoryUsage = getMemoryUsage();
    HuffmanTree huffmanTree(text);
    std::string huffmanEncoded = huffmanTree.encode(text);
    auto end = high_resolution_clock::now();
    auto huffmanDuration = duration_cast<microseconds>(end - start).count();
    int encodedSize = huffmanEncoded.size(); // El tamaño en bits es el número de caracteres en la cadena codificada
    
    start = high_resolution_clock::now();
    std::string huffmanDecoded = huffmanTree.decode(huffmanEncoded);
    end = high_resolution_clock::now();
    auto huffmanDuration2 = duration_cast<microseconds>(end - start).count();
    size_t finalMemoryUsage = getMemoryUsage();
    size_t huffmanMemoryUsage = finalMemoryUsage - initialMemoryUsage;

    

    // Medir el tiempo y el tamaño de LZ Compression
    start = high_resolution_clock::now();
    initialMemoryUsage = getMemoryUsage();
    auto lzCompressed = lzCompress(text);
    end = high_resolution_clock::now();
    auto lzDuration = duration_cast<microseconds>(end - start).count();
    int compressedSize = 0;
    for (const auto& pair : lzCompressed) {
        compressedSize += sizeof(pair.first) + sizeof(pair.second);
    }
    
    start = high_resolution_clock::now();
    std::string lzDecompressed = lzDecompress(lzCompressed);
    end = high_resolution_clock::now();
    auto lzDuration2 = duration_cast<microseconds>(end - start).count();
    finalMemoryUsage = getMemoryUsage();
    size_t lzMemoryUsage = finalMemoryUsage - initialMemoryUsage;

   // Guardar resultados en la carpeta "experimentos"
    std::ofstream outputFile(std::to_string(num) + "experimentacion.txt");
    if (!outputFile) {
        std::cerr << "Error: Could not open output file." << std::endl;
        return;
    }

    outputFile << "Huffman Encoding successful: " << (text == huffmanDecoded ? "Yes" : "No") << std::endl;
    outputFile << "Huffman Encoding time: " << huffmanDuration << " microseconds" << std::endl;
    outputFile << "Huffman Decoding time: " << huffmanDuration2 << " microseconds" << std::endl;
    outputFile << "Tamaño del input codificado: " << encodedSize << " bits" << std::endl;
    outputFile << "LZ Compression successful: " << (text == lzDecompressed ? "Yes" : "No") << std::endl;
    outputFile << "LZ Compression time: " << lzDuration << " microseconds" << std::endl;
    outputFile << "LZ Decompression time: " << lzDuration2 << " microseconds" << std::endl;
    outputFile << "Tamaño del input comprimido: " << compressedSize << " bytes" << std::endl;

    outputFile.close();
}

int main() {
    // Leer el archivo de entrada
    std::ifstream inputFile("proteins.50MB");
    if (!inputFile) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string text = buffer.str();

    // Ejecutar experimentos  
    for (int i = 1; i <= 20; i++)
    {
       runExperiments(text, i); 
    }
    
    return 0;
}