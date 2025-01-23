// v4
#include "lzw_functions.h"

#define INITIAL_SIZE 256

const u_short LIMIT{(u_short)65535};


void resetDict(std::map<std::string, u_short> &dict, u_short &currCode)
{
    dict.clear();
    currCode = INITIAL_SIZE;
    for (u_short i = 0; i < currCode; i++) dict[std::string(1, i)] = i;
}

std::vector<u_short> compress(std::string &path, size_t &file_size) 
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
   
    u_short currCode{256};

    std::map<std::string, u_short> dict;
    std::string word;
    std::vector<u_short> result;

    for (u_short i = 0; i < currCode; i++) dict[std::string(1, i)] = i; 
    
    for (size_t i = 0; i < file_size; i++) 
    {
        char c;
        ifs.read(&c, sizeof(c));
        std::string current = word + c;
        if (dict.contains(current)) word = current;
        else {
            result.push_back(dict[word]);
            if (currCode == LIMIT) resetDict(dict, currCode);
            dict[current] = currCode++;
            word = std::string(1, c);
        }
    }

    if (!word.empty()) result.push_back(dict[word]);

    ifs.close();

    return result;
}

void resetDict(std::map<u_short, std::string> &dict, u_short &currCode)
{
    currCode = INITIAL_SIZE;
    dict.clear();
    for(u_short i = 0; i < currCode; i++) dict[i] = std::string(1, i);
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

    u_short currCode{256};

    std::map<u_short, std::string> dict; 

    for(u_short i = 0; i < currCode; i++) dict[i] = std::string(1, i);

    u_short code{0};
    ifs.read((char *)&code, sizeof(code));
    std::string word(1, code);

    std::string result{word};

    std::string substr;

    while(ifs.read((char *)&code, sizeof(code))) 
    {
        if (dict.contains(code)) substr = dict[code];  
        else if (code == currCode) substr = word + word[0];
        else 
        {
            std::cout << "code: "<< code << " currCode: " << currCode << '\n';
            throw DecompressError();
        }


        result += substr;
        if (currCode == LIMIT) resetDict(dict, currCode);
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
    
    std::vector<u_short> dataCompressed;

    try { dataCompressed = compress(pathIn, file_size); } 
    catch (FileOpenError & e) { return 1; }
    catch (EmptyFileError & e) 
    { 
        std::cerr << e.what() << std::endl;
        return 3; 
    }
    
    int sizeCompressed = dataCompressed.size() * sizeof(u_short);

    try { writeFile(pathOut, dataCompressed); }
    catch (FileOpenError) { return 2; }

    std::cout << "File size: " << file_size << " bytes" << std::endl;

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
    catch (DecompressError &e) 
    {
        std::cerr << e.what() << std::endl;
        return 5;
    }

    std::cout <<"Compressed data size: "<< file_size << std::endl;
    
    std::cout <<"Decompressed data size: "<< dataDecompressed.size() << std::endl;
    
    const char* data_ptr = dataDecompressed.c_str();

    try { writeFile(pathOut, data_ptr, dataDecompressed.size()); }
    catch (FileOpenError &e) { return 2; }

    return 0;
}


