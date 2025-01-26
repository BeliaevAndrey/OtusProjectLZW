#include "lzw_program.h"

int parseargs(int argc, char **argv)
{

    const char* lzw_ext = ".lzwbin";

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
        
    
        logit = new Logger("Compress");


        if (argc > 3) file_out = argv[3];
        else file_out = file_in + lzw_ext;
        
        if (!file_out.ends_with(lzw_ext)) file_out = file_out + lzw_ext;

        std::string log_entry{"File in: "};
        log_entry.append(file_in).append("; File out: ").append(file_out);
        logit->info(log_entry);

        std::time_t start =  time(nullptr);
        int errCode = readAndCompress(file_in, file_out);
        std::time_t end = time(nullptr);

        if (!errCode)
        {
            std::string log_entry{"Compression time: "};
            log_entry.append(std::to_string((end - start)));
            log_entry.append(" seconds");
            logit->info(log_entry);
        }

        return errCode;
    }

    else if (key_d.compare(argv[1]) == 0)
    {
        logit = new Logger("Decompress");

        file_in = argv[2];
        if (!file_in.ends_with(lzw_ext))
        {
            std::string err{"Wrong file extension"};
            logit->error(err.c_str());
            return 4;
        }
        if (argc > 3)
            file_out = argv[3];
        else
        {
            file_out = file_in.substr(0, file_in.find_last_of(lzw_ext) - 6) + ".decompressed";
        }

        std::string log_entry{"File in: "};
        log_entry.append(file_in).append("; File out: ").append(file_out);
        logit->info(log_entry);
        
        std::time_t start =  time(nullptr);
        int errCode = readAndDecompress(file_in, file_out);
        std::time_t end = time(nullptr);

        if (!errCode)
        {
            std::string log_entry{"Decompression time: "};
            log_entry.append(std::to_string((end - start)));
            log_entry.append(" seconds");
        
            logit->info(log_entry);
        }

        return errCode;
    }
    
    std::cerr << "\nCommand not reconized.\n" << std::endl;

    printHelp();
    return 4;

}

int main(int argc, char** argv)
{
    std::cout << "LZW compressor v 1.22\n" << std::endl;
  
    if (argc > 1) 
    { 
        int errCode = parseargs(argc, argv);
        std::cout << "Finishing " <<  errCode << std::endl;
        delete logit;
        return errCode;
    }
    else printHelp();
    
    if (logit != nullptr) delete logit;
    
    return 0;

}
