#include <iostream>
#include <map>
#include <vector>


std::vector<int> compress(std::string line_in) 
{
    int startLen{256};
    std::map<std::string, int> dict;

    for (int i = 0; i < startLen; i++) dict[std::string(1, i)] = i; 
    
    std::string word;
    std::vector<int> result;

    for (char c : line_in) 
    {
        std::string current = word + c;
        if (dict.contains(current)) word = current;
        else {
            result.push_back(dict[word]);
            dict[current] = startLen++;
            word = std::string(1, c);
        }
    }

    if (!word.empty()) result.push_back(dict[word]);

    return result;
}


std::string decompress(std::vector<int> compressed)
{
    int startLen{256};

    int vectPos{0};
    int vectLen = compressed.size();

    std::map<int, std::string> dict; 

    for(int i = 0; i < startLen; i++) dict[i] = std::string(1, i);

    std::string word(1, compressed[vectPos++]);
    std::string result{word};

    std::string substr;

    for (; vectPos < vectLen; vectPos++) 
    {
        int code = compressed[vectPos];
        if (dict.contains(code)) substr = dict[code];  
        else if (code == startLen) substr = word + word[0];
        else throw "ERROR";

        result += substr;
        dict[startLen++] = word + substr[0];
        word = substr;
    }


    return result;
}


int main()
{

    std::string my_line = "а роза упала на лапу азора";

    std::vector<int> compressed = compress(my_line);

    for (int c : compressed) std::cout << c << ' ';

    std::cout << std::endl;

    std::string restored = decompress(compressed);

    std::cout << restored << std::endl;
    std::cout << std::boolalpha << (restored == my_line) << std::endl;


    return 0;
}