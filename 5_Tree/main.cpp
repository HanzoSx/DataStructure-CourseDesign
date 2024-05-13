#include <iostream>
#include <string>

#include <list>
#include <vector>

class Node
{
public:    
    
private:

};


class People : public Node
{
public:

    struct Date
    {
        Date(int year, int month, int day) : year(year), month(month), day(day) {}
        int year, month, day;
    };

    People(const std::string &name, const Date &date);

private:

    std::string m_name;
    Date m_borndate;
    Couple *m_couple;
    Couple *m_father;

};

People::People(const std::string &name, const Date &date) :
m_name    (name),
m_borndate(date),
m_couple  (nullptr)
{
}

class Couple : public Node
{
public:

    Couple(People &people1, People &people2);

private:
    
    People *m_p1, *m_p2;
    std::list<People*> m_son;

};

Couple::Couple(People &people1, People &people2) :
m_p1(&people1),
m_p2(&people2)
{
}

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
    std::string command;
    std::vector<std::string> args;
    while (command != "exit")
    {
        getCommand(command, args);
        
        if (command == "add")
        {
        }
        else if (command == "setf")
        {
        }
        else if (command == "setc")
        {
        }
        else if (command == "sets")
        {
        }
        else if (command == "value")
        {
        }
        else if (command.size() and command != "exit")
        {
        }
    }
    
    return 0;
}
