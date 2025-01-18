#include <iostream>
#include <map>
#include <vector>
#include <ctime>

#include "lzw_functions.h"


std::vector<int> compress(std::string &path, size_t &file_size) 
{
    std::ifstream ifs;
    try { ifs = get_ifstream(path, file_size); } 
    catch(FileOpenError & e)
    {
        std::cerr << e.what() << std::endl;
        throw e;
    }

    if (file_size == 0) throw EmptyFileError();

    std::cout << "Compressing..." << std::endl;
   
    int currCode{256};

    std::map<std::string, int> dict;
    std::string word;
    std::vector<int> result;

    for (int i = 0; i < currCode; i++) dict[std::string(1, i)] = i; 
    
    for (size_t i = 0; i < file_size; i++) 
    {
        char c;
        ifs.read(&c, sizeof(c));
        std::string current = word + c;
        if (dict.contains(current)) word = current;
        else {
            result.push_back(dict[word]);
            dict[current] = currCode++;
            word = std::string(1, c);
        }
    }

    if (!word.empty()) result.push_back(dict[word]);

    ifs.close();

    return result;
}


std::string decompress(std::string path, size_t &file_size)
{

    std::ifstream ifs;

    try { ifs = get_ifstream(path, file_size); }
    catch (std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
        throw e;
    }

    if (file_size == 0) throw EmptyFileError();

    std::cout << "Decompressing..." << std::endl;

    int currCode{256};

    std::map<int, std::string> dict; 

    for(int i = 0; i < currCode; i++) dict[i] = std::string(1, i);

    int code{0};
    ifs.read((char *)&code, sizeof(code));
    std::string word(1, code);

    std::string result{word};

    std::string substr;

    while(ifs.read((char *)&code, sizeof(code))) 
    {
        if (dict.contains(code)) substr = dict[code];  
        else if (code == currCode) substr = word + word[0];
        else throw std::invalid_argument("Error while decompressing");

        result += substr;
        dict[currCode++] = word + substr[0];
        word = substr;
    }

    return result;
}


int readAndCompress(std::string pathIn = "", 
                    std::string pathOut = "") 
{
    std::cout << "Read and Compress..." << std::endl;
    
    size_t file_size{0};
    
    std::vector<int> dataCompressed;

    try { dataCompressed = compress(pathIn, file_size); } 
    catch (FileOpenError & e) { return 1; }
    catch (EmptyFileError & e) 
    { 
        std::cerr << e.what() << std::endl;
        return 3; 
    }
    
    int sizeCompressed = dataCompressed.size() * sizeof(int);

    try { writeFile(pathOut, dataCompressed); }
    catch (FileOpenError) { return 2; }

    std::cout << "File size: " << file_size * 100 << " bytes" << std::endl;

    std::cout << "Compressed size: " << sizeCompressed << " bytes" << std::endl;

    std::cout << "Compression ratio: " << (double)sizeCompressed / file_size * 100 << "%" << std::endl;

    return 0;
}

int readAndDecompress(std::string pathIn = "", 
                      std::string pathOut = "") 
{
    
    std::cout << "Read and Decompress..." << std::endl;
    
    size_t file_size{0};


    std::string dataDecompressed;

    try {   dataDecompressed = decompress(pathIn, file_size); }
    catch (FileOpenError) { return 1; }
    catch (EmptyFileError & e) 
    { 
        std::cerr << e.what() << std::endl;
        return 3; 
    }

    std::cout <<"Compressed data size: "<< file_size << std::endl;
    
    std::cout <<"Decompressed data size: "<< dataDecompressed.size() << std::endl;
    
    const char* data_ptr = dataDecompressed.c_str();

    try { writeFile(pathOut, data_ptr, dataDecompressed.size()); }
    catch (FileOpenError &e) { return 2; }

    return 0;
}


