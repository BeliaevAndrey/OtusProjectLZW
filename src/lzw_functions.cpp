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
    std::cout << "Writing data..." << std::endl;

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

    std::cout << "Compressed size: " << buff_size * sizeof(int) << std::endl;
    
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



int readAndCompress(std::string pathIn = "", 
                    std::string pathOut = "") 
{
    std::cout << "Read and Compress..." << std::endl;
    
    if (pathIn.empty() || pathOut.empty())  /* to be removed */
    {
        std::string  pathIn{TESTSRC + "test3.txt"};
        std::string pathOut{TESTCMP + "test3.bin"};
        std::cout << "Lost filenames, using default" << std::endl;
    }
    
    size_t buf_size{0};

    const char* buffer = readFile(pathIn, buf_size);
    
    std::vector<int> dataCompressed = compress(buffer, buf_size);

    writeFile(pathOut, dataCompressed);


    return 0;
}

int readAndDecompress(std::string pathIn = "", 
                      std::string pathOut = "") 
{
    
    std::cout << "Read and Decompress..." << std::endl;

    
    if (pathIn.empty() || pathOut.empty()) /* to be removed */
    {
        std::string  pathIn{TESTSRC + "test3.txt"};
        std::string pathOut{TESTCMP + "test3.bin"};
        std::cout << "Lost filenames, using default" << std::endl;
    }
    
    std::vector<int> dataCompressed = readIntFile(pathIn);

    std::cout <<"Compressed data size: "<< dataCompressed.size() * sizeof(int) << std::endl;

    if (dataCompressed.size() == 0) throw std::invalid_argument("Vector error");

    std::string dataDecompressed = decompress(dataCompressed);
    
    std::cout <<"Decompressed data size: "<< dataDecompressed.size() << std::endl;
    
    const char* data_ptr = dataDecompressed.c_str();

    writeFile(pathOut, data_ptr, dataDecompressed.size());
    

    return 0;
}


int parseargs(int argc, char **argv)
{

    void printHelp();

    std::string c{"-c"};
    std::string d{"-d"};

   
    if (argc < 3)
    {
        printHelp();
        return 0;
    }

    std::string file_in;
    std::string file_out;

    if (c.compare(argv[1]) == 0)
    {
        file_in = argv[2];
        if (argc > 3)
            file_out = argv[3];
        else
        {
            file_out = file_in + ".bin";
        }
        std::cout << "file_in:  "<< file_in  << "\n"
                  << "file_out: "<< file_out 
                  << std::endl << std::endl;

        readAndCompress(file_in, file_out);
    }

    else if (d.compare(argv[1]) == 0)
    {
        file_in = argv[2];
        if (file_in.find_first_of(".bin") == 0)
        {
            std::cout << " Wrong file extension"<< std::endl;
            return -1;
        }
        if (argc > 3)
            file_out = argv[3];
        else
        {
            file_out = file_in.substr(0, file_in.find_first_of(".bin")) + ".decompressed";
        }
        std::cout << "file_in  : " << file_in << "\n"
                  << "file_out : " << file_out << std::endl;
    
        readAndDecompress(file_in, file_out);
    }

    return 0;
}

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

int main(int argc, char** argv)
{
    
    if (argc > 1) return parseargs(argc, argv);
    else printHelp();
    return 0;

}
