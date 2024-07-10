#include "huffman.h" 


std::unordered_map<char, std::string> HuffmanTree::getCodes() const {
    return codes;
}

HuffmanTree::HuffmanTree(const std::string& text) {
    buildTree(text); // Construir el árbol Huffman basado en el texto
    buildCodes(root, ""); // Construir los códigos Huffman a partir del árbol
}

std::string HuffmanTree::encode(const std::string& text) const {
    std::string encodedText;
    for (char c : text) {
        if (codes.find(c) != codes.end()) {
            encodedText += codes.at(c); // Añadir el código correspondiente al carácter
        } else {
            std::cerr << "Error: Character '" << c << "' not found in codes." << std::endl;
        }
    }
    return encodedText; // Devolver el texto codificado
}

std::string HuffmanTree::decode(const std::string& encodedText) const {
    std::string decodedText;
    std::shared_ptr<Node> current = root; // Comenzar desde la raíz del árbol
    for (char bit : encodedText) {
        if (bit == '0') {
            current = current->left; // Ir al nodo izquierdo para el bit '0'
        } else {
            current = current->right; // Ir al nodo derecho para el bit '1'
        }

        // Si se alcanza una hoja, añadir el símbolo al texto decodificado
        if (!current->left && !current->right) {
            decodedText += current->symbol;
            current = root; // Volver a la raíz para el siguiente símbolo
        }
    }
    return decodedText; // Devolver el texto decodificado
}

void HuffmanTree::buildTree(const std::string& text) {
    std::unordered_map<char, int> frequencies;
    for (char c : text) {
        frequencies[c]++; // Calcular la frecuencia de cada carácter
    }

    // Crear una cola de prioridad para construir el árbol
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNodes> pq;
    for (const auto& entry : frequencies) {
        pq.push(std::make_shared<Node>(Node{entry.first, entry.second, nullptr, nullptr}));
    }

    // Construir el árbol combinando los nodos de menor frecuencia
    while (pq.size() > 1) {
        auto left = pq.top();
        pq.pop();
        auto right = pq.top();
        pq.pop();

        auto parent = std::make_shared<Node>(Node{'\0', left->frequency + right->frequency, left, right});
        pq.push(parent);
    }

    root = pq.top(); 
}

void HuffmanTree::buildCodes(std::shared_ptr<Node> node, const std::string& code) {
    if (!node) {
        return;
    }

    // Si el nodo es una hoja, asignar el código al símbolo del nodo
    if (!node->left && !node->right) {
        codes[node->symbol] = code;
    } else {
        // Recursivamente construir los códigos para los nodos izquierdo y derecho
        buildCodes(node->left, code + "0");
        buildCodes(node->right, code + "1");
    }
}
