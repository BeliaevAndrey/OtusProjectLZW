#ifndef __LZW_LOGGER__
#define __LZW_LOGGER__

#include <fstream>
#include <string>
#include <ctime>
#include <iostream>

#include "lzw_exceptions.h"

// extern bool logging;

class Logger
{
    std::string filename{"lzw_project.log"};
    std::ofstream * log_file;
    std::string name;
    size_t name_len{0};
public:
    Logger(std::string name) : name{name}
    {
        this->name.append("-logger");
        this->name_len = this->name.length();
        std::cout << name << std::endl;
        log_file = new std::ofstream(filename, std::ios::app);
        if (!log_file->is_open()) throw LogFileError(name);
    }
    ~Logger()
    {
        if (log_file->is_open()) log_file->close();
        if (log_file != nullptr) delete log_file;
    }

    void info (std::string msg);
    void error(const char* msg);
    void message(std::string msg);

private:
    std::string time_now();
    void countChars(const char* msg, size_t &len);
};


#endif
