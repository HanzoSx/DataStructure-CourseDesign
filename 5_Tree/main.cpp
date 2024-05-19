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
        Date(int year = 0, int month = 0, int day = 0) : year(year), month(month), day(day) {}
        int year, month, day;
    };

    People(const std::string &name, const Date &date = Date());

    std::string get_name() const { return m_name; }
    Date get_borndate() const { return m_borndate; }
    Couple* get_couple() const { return m_couple; }
    Couple* get_father() const { return m_father; }

private:

    std::string m_name;
    Date m_borndate;
    Couple *m_couple;
    Couple *m_father;

};

People::People(const std::string &name, const Date &date) :
m_name    (name),
m_borndate(date),
m_couple  (nullptr),
m_father  (nullptr)
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

void err(size_t errid, std::string arg = "")
{
    static std::string errMsg[] = {
        "Wrong number of parameters"
    };
    std::cout << "ERROR : " << errMsg[errid];
    if (arg.size()) std::cout << " : " << arg;
    std::cout << "\n";
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

#include <map>
std::map<std::string, People> pmap;

int main(int argc, char const *argv[])
{
    std::string command;
    std::vector<std::string> args;
    while (command != "exit")
    {
        getCommand(command, args);
        
        if (command == "add")
        {
            if (args.size() != 1)
            {
                err(0);
                continue;
            }
            if (pmap.count(args[0]))
            {
                err(1);
                continue;
            }
            pmap.insert(std::pair(args[0], People(args[0])));
        }
        else if (command == "setf")
        {
            if (args.size() != 1 and args.size() != 2)
            {
                err(0);
                continue;
            }
            if (pmap.count(args[0]) == 0 or (args.size() == 2 and pmap.count(args[1]) == 0))
            {
                err(2);
                continue;
            }
            if (pmap[args[1]].get_couple() == nullptr)
            {
                err(3);
                continue;
            }
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
