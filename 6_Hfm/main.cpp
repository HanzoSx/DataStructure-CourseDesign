#include <osopt.hpp>
#include <hfm.hpp>

#include <iostream>
#include <fstream>

typedef std::map<char, size_t> FreqMap;
void load_frepmap_from_file(const std::string &path, FreqMap &freqmap)
{
    std::ifstream input_file(path);
    if (!input_file) return;

    freqmap.clear();
    char ch;
    while ((ch = input_file.get()) != EOF)
    {
        size_t freq;
        input_file >> freq;
        input_file.get();
        freqmap.insert(std::make_pair(ch, freq));
    }
}

void save_bin_file(const std::string &path, const std::string &str) {
    std::ofstream file(path, std::ios::binary);
    if (file.is_open()) {
        file.write(str.c_str(), str.size());
        file.close();
        std::cout << "Binary file saved successfully." << std::endl;
    } else {
        std::cerr << "Failed to save binary file." << std::endl;
    }
}

void load_bin_file(const std::string &path, std::string &str) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        str.resize(size);
        if (file.read(&str[0], size))
            std::cout << "Binary file loaded successfully." << std::endl;
        else
            std::cerr << "Failed to load binary file." << std::endl;

        file.close();
    } else {
        std::cerr << "Unable to open binary file." << std::endl;
    }
}

#include <vector>
void getCommand(std::string &command, std::vector<std::string> &args)
{
    command.clear(); args.clear();

    std::cout << "> ";
    std::string str;
    getline(std::cin, str);
    if (!str.size()) return;

    size_t pos = str.find_first_not_of(' ');
    if (pos != std::string::npos)
    {
        str.erase(0, pos);
        pos = str.find_first_of(' ');
        if (pos != std::string::npos)
        {
            command = str.substr(0, str.find_first_of(' '));
            str.erase(0, pos);
        }
        else
        {
            command = str;
            str.clear();
        }
    }
    else return;

    while (str.size())
    {
        pos = str.find_first_not_of(' ');
        if (pos != std::string::npos)
        {
            str.erase(0, pos);
            pos = str.find_first_of(' ');
            if (pos != std::string::npos)
            {
                args.push_back(str.substr(0, pos));
                str.erase(0, pos);
            }
            else
            {
                args.push_back(str);
                str.clear();
            }
        }
        else break;
    }
}

int main(int argc, char const *argv[])
{
    FreqMap freqmap;
    load_frepmap_from_file("test.freqmap", freqmap);

    HfmTree tree;
    tree.buildTree(freqmap);

    std::string text("AHSKDJ");
    std::string code;
    tree.code(text, code);
    std::cout << code << "\n";


    std::string command;
    std::vector<std::string> args;
    while (command != "exit")
    {
        getCommand(command, args);
        
        if (command == "build")
        {
            load_frepmap_from_file(args[0], freqmap);
            tree.buildTree(freqmap);
        }
        else if (command == "")
        {
        }
        else if (command == "")
        {
        }
        else if (command == "")
        {
        }
        else if (command == "")
        {
        }
        else if (command.size() and command != "exit")
        {
        }
    }

    return 0;
}

