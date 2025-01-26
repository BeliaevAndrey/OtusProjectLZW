// v5
#include "lzw_functions.h"


#define INITIAL_SIZE 256

const u_short LIMIT{(u_short)65535};


void resetDict(std::unordered_map<std::string, u_short> &dict, u_short &currCode)
{
    dict.clear();
    currCode = INITIAL_SIZE;
    for (u_short i = 0; i < currCode; i++) dict[std::string(1, i)] = i;
}

std::vector<u_short> compress(std::string &path, size_t &file_size) 
{
    std::ifstream ifs;
    try { ifs = get_ifstream(path, file_size); } 
    catch(FileOpenError e)
    {
        std::cerr << e.what() << std::endl;
        throw e;
    }

    if (file_size == 0) throw EmptyFileError();



    logit->message("Compressing...");

    u_short currCode{256};

    std::unordered_map<std::string, u_short> dict;
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

void resetDict(std::unordered_map<u_short, std::string> &dict, u_short &currCode)
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

    logit->message("Decompressing...");
    
    u_short currCode{256};

    std::unordered_map<u_short, std::string> dict; 

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
        else throw DecompressError(currCode, code);
        
        result += substr;
        if (currCode == LIMIT) resetDict(dict, currCode);
        dict[currCode++] = word + substr[0];
        word = substr;
    }

    ifs.close();

    return result;
}


int readAndCompress(std::string pathIn = "", 
                    std::string pathOut = "") 
{

    logit->message("Read and Compress...");
   
    size_t file_size{0};
    
    std::vector<u_short> dataCompressed;

    try { dataCompressed = compress(pathIn, file_size); } 
    catch (FileOpenError e) 
    {
        std::cerr << e.what() << '\n';
        logit->error(e.what());
        return 1; 
    }
    catch (EmptyFileError & e) 
    { 
        std::cerr << e.what() << std::endl;
        logit->error(e.what());
        return 3; 
    }
    
    int size_compressed = dataCompressed.size() * sizeof(u_short);

    try { writeFile(pathOut, dataCompressed); }
    catch (FileOpenError &e) 
    {
        logit->error(e.what()); 
        return 2; 
    }
    
    std::string log_entry{"File size: "};
    log_entry.append(std::to_string(file_size)).append(" bytes");
    logit->info(log_entry);

    log_entry = "Compressed size: ";
    log_entry.append(std::to_string(size_compressed)).append(" bytes");
    logit->info(log_entry);

    double c_ratio = (double)size_compressed / file_size * 100;

    log_entry = "Compression ratio: ";
    log_entry.append(std::to_string(c_ratio)).append(" %");
    logit->info(log_entry);



    return 0;
}

int readAndDecompress(std::string pathIn = "", 
                      std::string pathOut = "") 
{
    

    logit->message("Read and Decompress...");
    
    size_t file_size{0};


    std::string dataDecompressed;

    try {   dataDecompressed = decompress(pathIn, file_size); }
    catch (FileOpenError &e) 
    {
        std::cerr << e.what() << std::endl;
        logit->error(e.what());
        return 1; 
    }
    catch (EmptyFileError &e) 
    { 
        std::cerr << e.what() << std::endl;
        logit->error(e.what());
        return 3; 
    }
    catch (DecompressError &e) 
    {
        std::cerr << e.what() << std::endl;
        logit->error(e.what());
        return 5;
    }

    
    std::string log_entry{"Compressed data size: "};

    log_entry.append(std::to_string(file_size)).append(" bytes");
    logit->info(log_entry);

    log_entry = "Decompressed data size: ";
    log_entry.append(std::to_string(dataDecompressed.size())).append(" bytes");
    logit->info(log_entry);


    const char* data_ptr = dataDecompressed.c_str();

    try { writeFile(pathOut, data_ptr, dataDecompressed.size()); }
    catch (FileOpenError &e) 
    {
        logit->error(e.what());
        return 2; 
    }

    return 0;
}


