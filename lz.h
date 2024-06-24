#ifndef LZ_H
#define LZ_H

#include <vector>
#include <string>

std::vector<std::pair<std::string, int>> lzCompress(const std::string& input);
std::string lzDecompress(const std::vector<std::pair<std::string, int>>& compressed);

#endif // LZ_H
