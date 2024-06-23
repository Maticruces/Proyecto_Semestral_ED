#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>

using namespace std;

// Estructura para representar un nodo del árbol de Huffman
struct HuffmanNode {
    char data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, int frequency) {
        this->data = data;
        this->frequency = frequency;
        left = right = nullptr;
    }
};

// Comparador para la cola de prioridad en función de la frecuencia
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency;
    }
};

// Función para construir el árbol de Huffman y obtener los códigos de cada caracter
void buildHuffmanTree(string text, unordered_map<char, string>& codes) {
    // Contar la frecuencia de cada caracter
    unordered_map<char, int> freq;
    for (char c : text) {
        freq[c]++;
    }

    // Crear una cola de prioridad para almacenar los nodos del árbol de Huffman
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    // Crear un nodo para cada caracter y agregarlo a la cola de prioridad
    for (auto pair : freq) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // Construir el árbol de Huffman
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* newNode = new HuffmanNode('$', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    // Recorrer el árbol de Huffman y obtener los códigos de cada caracter
    HuffmanNode* root = pq.top();
    string code = "";
    unordered_map<char, string> huffmanCodes;
    traverseHuffmanTree(root, code, huffmanCodes);

    // Almacenar los códigos en el mapa de códigos
    codes = huffmanCodes;
}

// Función auxiliar para recorrer el árbol de Huffman y obtener los códigos de cada caracter
void traverseHuffmanTree(HuffmanNode* root, string code, unordered_map<char, string>& codes) {
    if (root == nullptr) {
        return;
    }

    if (root->data != '$') {
        codes[root->data] = code;
    }

    traverseHuffmanTree(root->left, code + "0", codes);
    traverseHuffmanTree(root->right, code + "1", codes);
}

// Función para codificar un archivo usando el mapa de códigos de Huffman
void encodeFile(string inputFile, string outputFile, unordered_map<char, string>& codes) {
    ifstream input(inputFile);
    ofstream output(outputFile);

    char c;
    while (input.get(c)) {
        output << codes[c];
    }

    input.close();
    output.close();
}

int main() {
    string inputFile = "input.txt";
    string outputFile = "output.txt";

    // Leer el archivo de entrada
    ifstream input(inputFile);
    string text((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
    input.close();

    // Construir el árbol de Huffman y obtener los códigos de cada caracter
    unordered_map<char, string> codes;
    buildHuffmanTree(text, codes);

    // Codificar el archivo de entrada usando los códigos de Huffman
    encodeFile(inputFile, outputFile, codes);

    cout << "Archivo codificado exitosamente." << endl;

    return 0;
}