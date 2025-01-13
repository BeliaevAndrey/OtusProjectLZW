#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <exception>

std::string TESTSRC = "../TestData/src/";
std::string TESTCMP = "../TestData/compressed/";
std::string TESTBCK = "../TestData/back/";

std::vector<int> compress(const char* line_in, size_t buff_size) 
{
    std::cout << "Compressing..." << std::endl;
   
    int currCode{256};

    std::map<std::string, int> dict;
    std::string word;
    std::vector<int> result;

    for (int i = 0; i < currCode; i++) dict[std::string(1, i)] = i; 
    
    

    for (size_t i = 0; i < buff_size; i++) 
    {
        char c = line_in[i];
        std::string current = word + c;
        if (dict.contains(current)) word = current;
        else {
            result.push_back(dict[word]);
            dict[current] = currCode++;
            word = std::string(1, c);
        }
    }

    if (!word.empty()) result.push_back(dict[word]);


    return result;
}


std::string decompress(std::vector<int> compressed)
{

    std::cout << "Decompressing..." << std::endl;

    int currCode{256};

    int vectPos{0};
    int vectLen = compressed.size();

    std::map<int, std::string> dict; 

    for(int i = 0; i < currCode; i++) dict[i] = std::string(1, i);

    std::cout << "\ncompressed.size(): " << compressed.size() << std::endl;

    std::string word(1, compressed[vectPos++]);
    std::string result{word};

    std::string substr;

    for (; vectPos < vectLen; vectPos++) 
    {
        int code = compressed[vectPos];
        if (dict.contains(code)) substr = dict[code];  
        else if (code == currCode) substr = word + word[0];
        else throw std::invalid_argument("Error while decompressing");

        result += substr;
        dict[currCode++] = word + substr[0];
        word = substr;
    }


    return result;
}


const char* readFile(std::string path, size_t &buf_size)
{
    std::cout << "Reading chars..." << std::endl;

    std::ifstream *ifs = new std::ifstream(path, std::ios::binary | std::ios::ate);

    if (!ifs->is_open()) throw std::invalid_argument("File  not found");
    buf_size = ifs->tellg();
    ifs->seekg(0);

    char* buffer = new char[buf_size];

    for (size_t i = 0; i < buf_size; i++ ) ifs->read(&buffer[i], sizeof(*buffer));
    
    buffer[buf_size] = '\0';


    ifs->close();

    delete ifs;

    return buffer;
}


int writeFile(std::string path, const char *buffer, size_t buff_size)
{
    std::ofstream *ofs = new std::ofstream(path, std::ios::binary);
    if (!ofs->is_open()) throw std::invalid_argument("File  not found");
    
    for (size_t i = 0; i < buff_size; i++ ) ofs->write(&buffer[i], sizeof(buffer[i]));

    ofs->flush();

    ofs->close();
    
    delete ofs;

    return 0;

}


int writeFile(std::string path, std::vector<int> &buffer)
{
    std::cout << "Writing ints..." << std::endl;

    std::ofstream *ofs = new std::ofstream(path, std::ios::binary);
    if (!ofs->is_open()) throw std::invalid_argument("File  not found");
    
    size_t buff_size = buffer.size();
    std::cout << "buff_size: " << buff_size << std::endl;
    
    for (size_t i = 0; i < buff_size; i++ ) ofs->write(reinterpret_cast<char*>(&buffer[i]), sizeof(int));

    ofs->flush();

    ofs->close();

    delete ofs;

    return 0;
}


std::vector<int> readIntFile(std::string path)
{
    std::cout << "Reading ints..." << std::endl;

    std::ifstream *ifs = new std::ifstream(path, std::ios::binary);

    if (!ifs->is_open()) throw std::invalid_argument("File not found");

    std::vector<int> buffer;

    int code{0};
    size_t csize = sizeof(code);
    while (ifs->read((char*)&code, csize)) buffer.push_back(code);
    
    ifs->close();

    delete ifs;

    return buffer;
}



int readAndCompress() 
{
    std::cout << "readAndCompress..." << std::endl;
    
    std::string  pathIn{TESTSRC + "test_picture.bmp"};
    std::string pathOut{TESTCMP + "test_picture.bin"};

    size_t buf_size{0};

    const char* buffer = readFile(pathIn, buf_size);
    
    std::vector<int> dataCompressed = compress(buffer, buf_size);

    writeFile(pathOut, dataCompressed);


    return 0;
}

int readAndDecompress() 
{
    
    std::cout << "readAndDecompress..." << std::endl;

    // std::string  pathIn{TESTCMP + "test3.bin"};
    // std::string pathOut{TESTBCK + "test3.txt"};
    std::string  pathIn{TESTCMP + "test_picture.bin"};
    std::string pathOut{TESTBCK + "test_picture.bmp"};

    std::vector<int> dataCompressed = readIntFile(pathIn);

    std::cout <<"Compressed data size: "<< dataCompressed.size() << std::endl;

    if (dataCompressed.size() == 0) throw std::invalid_argument("Vector error");

    std::string dataDecompressed = decompress(dataCompressed);
    
    std::cout <<"Decompressed data size: "<< dataDecompressed.size() << std::endl;
    
    const char* data_ptr = dataDecompressed.c_str();

    writeFile(pathOut, data_ptr, dataDecompressed.size());
    

    return 0;
}


int main()
{

    readAndCompress();
    readAndDecompress();

    return 0;
}
