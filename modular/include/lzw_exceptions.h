#ifndef __LZW_EXC__
#define __LZW_EXC__

#include <exception>

class DecompressError : public std::exception
{
public:
    const char* what() const noexcept override{ return "Error while reading codes"; }
};

class FileOpenError : public std::exception
{
    const char* msg;
public:
    FileOpenError(int condition) 
    {
        if (condition == 1) msg = "Error while opening input file";
        else if (condition == 1) msg = "Error while opening output file";
        else msg = "Other file opening error.";
    }
    const char* what() const noexcept override{ return msg; }
};

class EmptyFileError : public std::exception 
{
public:
    const char* what() const noexcept override { return "File is empty!"; }
};

#endif