#ifndef __LZW_EXC__
#define __LZW_EXC__

#include <exception>

class DecompressError : public std::exception
{
    std::string msg{"Error while reading codes"};

public:
    DecompressError()=default; 
    DecompressError(int currCode, int code) 
    {    
        msg = new char[70];
        msg.append("current code: ")
           .append(std::to_string(currCode))
           .append(", searched code: ")
           .append(std::to_string(code));
       
    }
    ~DecompressError() { }

    const char *what() const noexcept override { return msg.c_str(); }
};

class FileOpenError : public std::exception
{
    std::string msg;

public:
    FileOpenError(int condition)
    {
        if (condition == 1)
            msg = "Error while opening input file";
        else if (condition == 2)
            msg = "Error while opening output file";
        else
            msg = "Other file opening error.";
    }
    ~FileOpenError() { }

    const char *what() const noexcept override { return msg.c_str(); }
};

class EmptyFileError : public std::exception
{
public:
    const char *what() const noexcept override { return "File is empty!"; }
};

class LogFileError : public std::exception
{
    std::string msg;
public:
    LogFileError(std::string logger_name)
    {
        msg = logger_name.append(": Logfile open error.");
    }
    ~LogFileError() { }
    const char * what() const noexcept override {return msg.c_str();}
};

#endif
