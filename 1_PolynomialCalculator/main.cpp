#include <iostream>
#include <iomanip>
#include <cctype>

#include <vector>
#include <map>

#include "Polynomial.hpp"

void err(size_t errid, std::string arg = "")
{
    static std::string errMsg[] = {
        "Variable does not exist",
        "Illegal variable name",
        "Illegal expression",
        "Command not found",
        "Wrong number of parameters"
    };
    std::cout << "ERROR : " << errMsg[errid];
    if (arg.size()) std::cout << " : " << arg;
    std::cout << "\n";
}

typedef std::map<std::string, Polynomial>::iterator MapIterator;
std::map<std::string, Polynomial> PolyMap;
bool parseVariable(std::string str, MapIterator &out_ptrPoly, bool creat = false)
{
    // std::string LegalSpecialCh = "";
    size_t pos = str.find_first_not_of(' ');
    if (pos != std::string::npos) str.erase(0, pos);
    pos = str.find_last_not_of(' ');
    if (pos != std::string::npos) str.erase(pos + 1);

    for (auto ch = str.begin(); ch != str.end(); ++ ch)
        if (ch == str.begin() and std::isdigit(*ch) or
            !std::isdigit(*ch) and !std::isalpha(*ch) and *ch != '_')
        {
            err(1, str);
            return false;
        }
    if (str == "x")
    {
        err(1, str);
        return false;
    }

    if ((out_ptrPoly = PolyMap.find(str)) != PolyMap.end())
        return true;

    if (!creat) return false;
    PolyMap.insert(std::make_pair(str, Polynomial()));
    out_ptrPoly = PolyMap.find(str);
    return true;
}

bool parseExpression(std::string str, Polynomial &out_Poly)
{

}

void getCommand(std::string &command, std::vector<std::string> &args)
{
    std::cout << "> ";
    std::string str;
    getline(std::cin, str);
    if (!str.size()) return;

    size_t pos = str.find('=');
    if (pos != std::string::npos and
        pos < str.size() - 1 and str.find('=', pos + 1) == std::string::npos)
    {
        command = "=";
        args.push_back(str.substr(0, pos - 1));
        args.push_back(str.substr(pos + 1));
        return;
    }

    pos = str.find_first_not_of(' ');
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
    std::string command;
    std::vector<std::string> args;
    while (command != "exit")
    {
        getCommand(command, args);
        
        if (command == "=")
        {
            MapIterator var;
            Polynomial expression;
            if (parseVariable(args[0], var, true) and parseExpression(args[1], expression))
                var->second = expression;
        }
        else if (command == "calc" or command == "c")
        {
            std::string arg;
            arg.clear();
            for (auto it : args) arg += it;

            Polynomial expression;
            if (parseExpression(arg, expression))
            {
                expression.Print();
                std::cout << "\n";
            }
        }
        else if (command == "list" or command == "ls")
        {
            if (args.size())
            {
                err(4);
                continue;
            }

            std::cout << std::setw(10) << std::left << "name" << "val\n";
            for (auto it : PolyMap)
            {
                std::cout << std::setw(10) << std::left << it.first;
                it.second.Print();
                std::cout << "\n";
            }
        }
        else if (command == "remove" or command == "rm")
        {
            if (args.size() > 1)
            {
                err(4);
                continue;
            }

            MapIterator var;
            if (!parseVariable(args[0], var))
                err(0, args[0]);
            else
                PolyMap.erase(var);
        }
        else if (command.size())
            err(3, command);
    }
    
    return 0;
}
