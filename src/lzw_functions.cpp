#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <exception>
#include <ctime>


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


std::ifstream get_ifstream(std::string &path, size_t &file_size)
{
    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    if (!ifs.is_open()) throw FileOpenError(1);

    file_size = ifs.tellg();
    
    ifs.seekg(0);

    return ifs;
}


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


int writeFile(std::string path, const char *buffer, size_t buff_size)
{
    std::cout << "Writing data..." << std::endl;

    std::ofstream *ofs = new std::ofstream(path, std::ios::binary);
    if (!ofs->is_open()) throw FileOpenError(2);
    
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
    if (!ofs->is_open()) throw FileOpenError(2);
    
    size_t buff_size = buffer.size();
  
    for (size_t i = 0; i < buff_size; i++ ) ofs->write(reinterpret_cast<char*>(&buffer[i]), sizeof(int));

    ofs->flush();

    ofs->close();

    delete ofs;

    return 0;
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


int parseargs(int argc, char **argv)
{

    const char* lzw_ext = ".lzwbin";

    void printHelp();

    for (int i = 0; i < argc; i++) std::cout << argv[i] << '\n';
   
    if (argc < 3)
    {
        printHelp();
        return 0;
    }

    std::string key_c{"-c"};
    std::string key_d{"-d"};

    std::string file_in;
    std::string file_out;

    if (key_c.compare(argv[1]) == 0)
    {
        file_in = argv[2];
        
        if (argc > 3) file_out = argv[3];
        else file_out = file_in + lzw_ext;
        
        if (!file_out.ends_with(lzw_ext)) file_out = file_out + lzw_ext;
        
        std::cout << "file_in:  "<< file_in  << "\n"
                  << "file_out: "<< file_out 
                  << std::endl << std::endl;

        std::time_t start =  time(nullptr);
        int errCode = readAndCompress(file_in, file_out);
        std::time_t end = time(nullptr);

        if (!errCode) 
        {
            std::cout << "compression:\n";
            std::cout << "time taken: " << (end - start) << " seconds" << std::endl;
        } 

        return errCode;
    }

    else if (key_d.compare(argv[1]) == 0)
    {
        file_in = argv[2];
        if (!file_in.ends_with(lzw_ext))
        {
            std::cout << "Wrong file extension"<< std::endl;
            return -1;
        }
        if (argc > 3)
            file_out = argv[3];
        else
        {
            file_out = file_in.substr(0, file_in.find_last_of(lzw_ext) - 3) + ".decompressed";
        }
        std::cout << "file_in  : " << file_in << "\n"
                  << "file_out : " << file_out 
                  << std::endl << std::endl;
    

        std::time_t start =  time(nullptr);
        int errCode = readAndDecompress(file_in, file_out);
        std::time_t end = time(nullptr);
        
        if (!errCode)
        {
            std::cout << "decompression:\n";
            std::cout << "time taken: " << (end - start) << " seconds" << std::endl;
        }

        return errCode;
    }
    
    std::cout << "\nWrong command.\n" << std::endl;

    printHelp();
    return -1;

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
