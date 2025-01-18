#include "lzw_file_work.h"

std::ifstream get_ifstream(std::string &path, size_t &file_size)
{
    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    if (!ifs.is_open()) throw FileOpenError(1);

    file_size = ifs.tellg();
    
    ifs.seekg(0);

    return ifs;
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
