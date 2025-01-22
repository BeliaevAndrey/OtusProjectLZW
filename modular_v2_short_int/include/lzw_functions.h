#ifndef __LZW_FUNC__
#define __LZW_FUNC__

#include <iostream>
#include <map>
#include <vector>
#include <ctime>

#include "lzw_exceptions.h"
#include "lzw_file_work.h"

std::vector<u_short> compress(std::string &path, size_t &file_size);
std::string decompress(std::string path, size_t &file_size);
int readAndCompress(std::string pathIn, std::string pathOut);
int readAndDecompress(std::string pathIn, std::string pathOut);
void resetDict(std::map<std::string, u_short> &dict, int &currCode);
void resetDict(std::map<u_short, std::string> &dict, int &currCode);

#endif
