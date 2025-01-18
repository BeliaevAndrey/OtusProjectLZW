#include <iostream>
#include <map>
#include <vector>
#include <ctime>

#include "lzw_exceptions.h"
#include "lzw_file_work.h"

std::vector<int> compress(std::string &path, size_t &file_size);
std::string decompress(std::string path, size_t &file_size);
int readAndCompress(std::string pathIn, std::string pathOut);
int readAndDecompress(std::string pathIn, std::string pathOut);
