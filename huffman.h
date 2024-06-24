#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <unordered_map>
#include <memory>
#include <queue>
#include <vector>
#include <iostream>

class HuffmanTree {
public:
    struct Node {
        char symbol;
        int frequency;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
    };

    struct CompareNodes {
        bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
            return a->frequency > b->frequency;
        }
    };

    HuffmanTree(const std::string& text);
    std::string encode(const std::string& text) const;
    std::string decode(const std::string& encodedText) const;
    std::unordered_map<char, std::string> getCodes() const;

private:
    void buildTree(const std::string& text);
    void buildCodes(std::shared_ptr<Node> node, const std::string& code);

    std::shared_ptr<Node> root;
    std::unordered_map<char, std::string> codes;
};

#endif
