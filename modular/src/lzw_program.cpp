#include "lzw_program.h"


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

int main(int argc, char** argv)
{
    
    if (argc > 1) return parseargs(argc, argv);
    else printHelp();
    return 0;

}
