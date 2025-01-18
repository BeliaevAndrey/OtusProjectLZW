#ifndef __LZW_FILE_WORK__
#define __LZW_FILE_WORK__

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include "lzw_exceptions.h"


std::ifstream get_ifstream(std::string &path, size_t &file_size);
int writeFile(std::string path, const char *buffer, size_t buff_size);
int writeFile(std::string path, std::vector<int> &buffer);


#endif