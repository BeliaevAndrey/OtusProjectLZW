#ifndef __LZW_PROG__
#define __LZW_PROG__

#include <iostream>
#include <string>

#include "lzw_functions.h"

void printHelp()
{
    std::string usage{
        "Usage:\n"\
        "lzw -[c|d] file-in file-out\n"\
        "Commands:\n"\
        "-c\t\tcompress\n"\
        "-d\t\tdecompress\n\n"\
        "file-in \tinput path/to/file/filename\n"\
        "file-out\toutput path/to/file/filename\n"\
        "compressed file extension: '.bin'\n"
    };

    std::cout << usage << std::endl; 
}

int parseargs(int, char**);


#endif
