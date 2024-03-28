#include <iostream>
#include <iomanip>
#include <cctype>

#include <vector>
#include <stack>
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

    if (!creat)
    {
        err(0, str);
        return false;
    }
    PolyMap.insert(std::make_pair(str, Polynomial()));
    out_ptrPoly = PolyMap.find(str);
    return true;
}

bool is_op(char c) { return c == '+' || c == '-' || c == '*'; }
bool is_unary(char c) { return c == '+' || c == '-'; }
int priority(char op)
{
    if (op < 0) return 3;
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return -1;
}

void process_op(std::stack<Polynomial>& st, char op)
{
    if (op < 0)
    {
        Polynomial l = st.top(); st.pop();
        switch (-op)
        {
            case '+': st.push(l); break;
            case '-': st.push(-l); break;
        }
    }
    else
    {
        Polynomial r = st.top(); st.pop();
        Polynomial l = st.top(); st.pop();
        switch (op)
        {
            case '+': st.push(l + r); break;
            case '-': st.push(l - r); break;
            case '*': st.push(l * r); break;
        }
    }
}

bool parseExpression(std::string str, Polynomial &out_Poly)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    str = str.substr(first, last - first + 1);

    bool isContainExpression = false;
    for (size_t i = 0; i < str.size(); ++ i)
    {
        if (!(is_op(str[i]) or str[i] == '(' or str[i] == ')'))
            continue;
        while (str[i - 1] == ' ') str.erase(i - 1, 1), -- i;
        while (str[i + 1] == ' ') str.erase(i + 1, 1);
    }

    std::vector<bool> notop(str.size(), false);
    for (size_t i = 1; i < str.size(); ++ i)
        if (str[i] == '-' and str[i-1] == '^')
            notop[i] = true;
        else if (is_op(str[i]) or str[i] == '(' or str[i] == ')')
            isContainExpression = true;

    if (!isContainExpression)
    {
        if (!str.size()) return false;
        if (!std::isdigit(str[0]) and str[0] != 'x')
        {
            MapIterator var;
            if (!parseVariable(str, var)) return false;
            out_Poly = var->second;
            return true;
        }
        else
        {
            size_t pos = str.find('x');
            int exp = 0;
            double constant = 1.;

            try
            {
                std::string tmp;
                if (pos != std::string::npos) tmp = str.substr(0, pos);
                    else tmp = str;
                if (!tmp.empty())
                    constant = std::stod(tmp);
                else
                    constant = 1.;
            }
            catch (const std::invalid_argument& e)
            {
                err(2, str);
                return false;
            }
            
            if (pos != std::string::npos)
            {
                if (pos < str.size() - 1 and str[pos + 1] != '^')
                {
                    err(2, str);
                    return false;
                }
                try
                {
                    if (pos < str.size() - 1)
                    {
                        std::string tmp = str.substr(pos + 2);
                        exp = std::stol(tmp);
                    }
                    else exp = 1;
                }
                catch (const std::invalid_argument& e)
                {
                    err(2, str);
                    return false;
                }
            }
            
            out_Poly.clear();
            out_Poly.set(exp, constant);
            return true;
        }
    }
    else
    {
        std::stack<Polynomial> st;
        std::stack<char> op;
        bool may_be_unary = true;

        for (size_t i = 0; i < str.size(); ++ i) 
        {
            if (str[i] == '(')
            {
                op.push('(');
                may_be_unary = true;
            }
            else if (str[i] == ')')
            {
                while (op.top() != '(')
                {
                    process_op(st, op.top());
                    op.pop();
                }
                op.pop();
                may_be_unary = false;
            }
            else if (is_op(str[i]) and !notop[i])
            {
                char cur_op = str[i];
                if (may_be_unary && is_unary(cur_op)) cur_op = -cur_op;
                while (!op.empty() &&
                        ((cur_op >= 0 && priority(op.top()) >= priority(cur_op)) ||
                        (cur_op < 0 && priority(op.top()) > priority(cur_op)))) {
                    process_op(st, op.top());
                    op.pop();
                }
                op.push(cur_op);
                may_be_unary = true;
            }
            else
            {
                std::string tmp; tmp.clear();
                while (i < str.size() && !is_op(str[i]) && str[i] != '(' && str[i] != ')' || notop[i])
                    tmp += str[i], ++ i;
                -- i;

                Polynomial poly;
                if (!parseExpression(tmp, poly))
                {
                    err(2, tmp);
                    return false;
                }
                st.push(poly);
                may_be_unary = false;
            }
        }

        while (!op.empty())
        {
            if (op.top() == '(' or op.top() == ')')
            {
                err(2, str);
                return false;
            }
            process_op(st, op.top());
            op.pop();
        }
        out_Poly = st.top();
        return true;
    }
    return false;
}

void getCommand(std::string &command, std::vector<std::string> &args)
{
    command.clear(); args.clear();

    std::cout << "> ";
    std::string str;
    getline(std::cin, str);
    if (!str.size()) return;

    size_t pos = str.find('=');
    if (pos != std::string::npos and
        pos < str.size() - 1 and str.find('=', pos + 1) == std::string::npos)
    {
        command = "=";
        args.push_back(str.substr(0, pos));
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
            if (parseExpression(args[1], expression) and parseVariable(args[0], var, true))
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
        else if (command == "d")
        {
            if (args.size() != 1)
            {
                err(4);
                continue;
            }
            MapIterator var;
            if (parseVariable(args[0], var))
                var->second.derivative();
        }
        else if (command == "value" or command == "val")
        {
            if (args.size() != 2)
            {
                err(4);
                continue;
            }
            MapIterator var;
            if (!parseVariable(args[0], var)) continue;
            try
            {
                double x = std::stod(args[1]);
                std::cout << "value = " << var->second.value(x) << "\n";
            }
            catch(const std::invalid_argument& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (command.size() and command != "exit")
            err(3, command);
    }
    
    return 0;
}
