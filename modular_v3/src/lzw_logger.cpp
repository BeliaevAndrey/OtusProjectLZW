#include "lzw_logger.h"

const size_t time_len{20};
const size_t extra{20};

std::string Logger::time_now()
{
    time_t timestamp = time(nullptr);
    struct tm* now = std::localtime(&timestamp);
    char time_mark[time_len];
    std::strftime(time_mark, time_len, "%Y-%m-%d %H:%M:%S", now);
    return time_mark;
}


void Logger::info(std::string msg)
{
    std::cout << msg << std::endl;

    size_t el = name_len + time_len + msg.length() + extra;

    char* log_entry = new char[el];
    size_t entry_len = sprintf(log_entry, "INFO (%s): %s -- %s\n", 
                               name.c_str(), time_now().c_str(), msg.c_str());

    log_file->write(log_entry, entry_len);
    log_file->flush();

    delete[] log_entry;
}

void Logger::countChars(const char* msg, size_t &len)
{
    for (;msg[len] != '\0'; len++);
    std::cout << '\t' << len << std::endl;
}

void Logger::error(const char* msg)
{

    std::cerr << msg << std::endl;
    
    size_t len{0};
    countChars(msg, len);

    size_t el = len + time_len + extra + name_len;

    char* log_entry = new char[el];
    size_t entry_len = sprintf(log_entry, "ERROR: (%s): %s -- %s\n", name.c_str(), time_now().c_str(), msg);


    log_file->write(log_entry, entry_len);
    log_file->flush();

    delete[] log_entry;
}

void Logger::message(std::string msg) { std::cout << msg << std::endl; }
