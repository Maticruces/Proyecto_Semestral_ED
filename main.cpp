#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>
#include "huffman.h"
#include "lz.cpp"
#include "compresion.cpp"

// Función para obtener el uso de memoria en bytes utilizando new/delete
size_t getMemoryUsage() {
    // Esta función simplemente crea y destruye un bloque de memoria para
    // medir el uso de memoria aproximado.
    size_t* dummy = new size_t;
    size_t memoryUsage = sizeof(*dummy);
    delete dummy;
    return memoryUsage;
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
    
    start = high_resolution_clock::now();
    std::string huffmanDecoded = huffmanTree.decode(huffmanEncoded);
    end = high_resolution_clock::now();
    size_t finalMemoryUsage = getMemoryUsage();
    auto huffmanDuration2 = duration_cast<microseconds>(end - start).count();
    size_t huffmanMemoryUsage = finalMemoryUsage - initialMemoryUsage;

    

    // Medir el tiempo y el tamaño de LZ Compression
    start = high_resolution_clock::now();
    initialMemoryUsage = getMemoryUsage();
    std::vector<std::pair<int, int>> lzCompressed = lzCompress(text);
    end = high_resolution_clock::now();
    auto lzDuration = duration_cast<microseconds>(end - start).count();
    
    start = high_resolution_clock::now();
    std::string lzDecompressed = lzDecompress(lzCompressed);
    end = high_resolution_clock::now();
    finalMemoryUsage = getMemoryUsage();
    auto lzDuration2 = duration_cast<microseconds>(end - start).count();
    size_t lzMemoryUsage = finalMemoryUsage - initialMemoryUsage;

    
    

    // Guardar resultados en el archivo "num + experimentacion.txt"
    std::ofstream outputFile(num + "experimentacionnnnn.txt" );
    if (!outputFile) {
        std::cerr << "Error: Could not open output file." << std::endl;
        return;
    }

    outputFile << "Huffman Encoding successful: " << (text == huffmanDecoded ? "Yes" : "No") << std::endl;
    outputFile << "Huffman Encoding time: " << huffmanDuration << " microseconds" << std::endl;
    outputFile << "Huffman Decoding time: " << huffmanDuration2 << " microseconds" << std::endl;
    outputFile << "Huffman Memory usage: " << huffmanMemoryUsage << " bytes" << std::endl;
    outputFile << "LZ Compression successful: " << (text == lzDecompressed ? "Yes" : "No") << std::endl;
    outputFile << "LZ Compression time: " << lzDuration << " microseconds" << std::endl;
    outputFile << "LZ Decompression time: " << lzDuration2 << " microseconds" << std::endl;
    outputFile << "LZ Memory usage: " << lzMemoryUsage << " bytes" << std::endl;

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