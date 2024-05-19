#include <osopt.hpp>
#include <hfm.hpp>

#include <iostream>
#include <fstream>

typedef std::map<char, size_t> FreqMap;
void load_freqmap_from_file(const std::string &path, FreqMap &freqmap)
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
    HfmTree tree;

    std::string command;
    std::vector<std::string> args;
    while (command != "exit")
    {
        getCommand(command, args);
        
        if (command == "build") {
            if (args.size() == 1) {
                load_freqmap_from_file(args[0], freqmap);
                tree.buildTree(freqmap);
            } else {
                std::cerr << "Usage: build <freqmap_file>" << std::endl;
            }
        } else if (command == "t2c") {
            if (args.size() == 1) {
                std::string text_to_encode = args[0];
                std::string encoded_text;
                tree.code(text_to_encode, encoded_text);
                std::cout << "Encoded: " << encoded_text << std::endl;
            } else {
                std::cerr << "Usage: t2c <text_to_encode>" << std::endl;
            }
        } else if (command == "c2t") {
            if (args.size() == 1) {
                std::string encoded_text = args[0];
                std::string decoded_text;
                tree.decode(encoded_text, decoded_text);
                std::cout << "Decoded: " << decoded_text << std::endl;
            } else {
                std::cerr << "Usage: c2t <encoded_text>" << std::endl;
            }
        } else if (command == "t2f") {
            if (args.size() == 2) {
                std::string text_to_encode = args[0];
                std::string file_path = args[1];
                std::string encoded_text;
                tree.code(text_to_encode, encoded_text);
                save_bin_file(file_path, encoded_text);
            } else {
                std::cerr << "Usage: t2f <text_to_encode> <file_path>" << std::endl;
            }
        } else if (command == "f2t") {
            if (args.size() == 1) {
                std::string file_path = args[0];
                std::string encoded_text;
                load_bin_file(file_path, encoded_text);
                std::string decoded_text;
                tree.decode(encoded_text, decoded_text);
                std::cout << "Decoded from file: " << decoded_text << std::endl;
            } else {
                std::cerr << "Usage: f2t <file_path>" << std::endl;
            }
        } else if (!command.empty() && command != "exit") {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }

    return 0;
}

