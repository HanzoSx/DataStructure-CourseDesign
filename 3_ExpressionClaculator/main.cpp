#include <iostream>
#include <iomanip>
#include <cctype>

#include <vector>
#include <stack>
#include <map>


void err(size_t errid, std::string arg = "")
{
    static std::string errMsg[] = {
        "Variable does not exist",
        "Illegal variable name",
        "Illegal expression",
        "Command not found",
        "Wrong number of parameters",
        "div 0"
    };
    std::cout << "ERROR : " << errMsg[errid];
    if (arg.size()) std::cout << " : " << arg;
    std::cout << "\n";
}

typedef std::map<std::string, int>::iterator MapIterator;
std::map<std::string, int> IntMap;
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

    if ((out_ptrPoly = IntMap.find(str)) != IntMap.end())
        return true;

    if (!creat)
    {
        err(0, str);
        return false;
    }
    IntMap.insert(std::make_pair(str, int()));
    out_ptrPoly = IntMap.find(str);
    return true;
}

bool is_op(char c) { return c == '+' || c == '-' || c == '*' || c == '/'; }
bool is_unary(char c) { return c == '+' || c == '-'; }
int priority(char op)
{
    if (op < 0) return 3;
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return -1;
}

bool process_op(std::stack<int>& st, char op)
{
    if (op < 0)
    {
        if (st.size() < 1) return false;
        int l = st.top(); st.pop();
        switch (-op)
        {
            case '+': st.push(l); break;
            case '-': st.push(-l); break;
        }
    }
    else
    {
        if (st.size() < 2) return false;
        int r = st.top(); st.pop();
        int l = st.top(); st.pop();
        switch (op)
        {
            case '+': st.push(l + r); break;
            case '-': st.push(l - r); break;
            case '*': st.push(l * r); break;
            case '/':
                if (r == 0)
                {
                    err(4);
                    return false;
                }
                st.push(l / r);
                break;
        }
    }
    return true;
}

bool parseExpression(std::string str, int &out_Int)
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

    for (size_t i = 1; i < str.size(); ++ i)
        if (is_op(str[i]) or str[i] == '(' or str[i] == ')')
            isContainExpression = true;

    if (!isContainExpression)
    {
        if (!str.size()) return false;
        if (!std::isdigit(str[0]))
        {
            MapIterator var;
            if (!parseVariable(str, var)) return false;
            out_Int = var->second;
            return true;
        }
        else
        {
            int constant = 1;
            try
            {
                if (!str.empty())
                {
                    size_t pos;
                    constant = std::stod(str, &pos);
                    if (pos < str.size())
                    {
                        err(2, str);
                        return false;
                    }
                }
            }
            catch (const std::invalid_argument& e)
            {
                err(2, str);
                return false;
            }
            
            out_Int = constant;
            return true;
        }
    }
    else
    {
        std::stack<int> st;
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
                while (!op.empty() and op.top() != '(')
                {
                    if (!process_op(st, op.top()))
                    {
                        err(2, str);
                        return false;
                    }
                    op.pop();
                }
                if (op.empty())
                {
                    err(2, str);
                    return false;
                }
                op.pop();
                may_be_unary = false;
            }
            else if (is_op(str[i]))
            {
                char cur_op = str[i];
                if (may_be_unary && is_unary(cur_op)) cur_op = -cur_op;
                while (!op.empty() &&
                        ((cur_op >= 0 && priority(op.top()) >= priority(cur_op)) ||
                        (cur_op < 0 && priority(op.top()) > priority(cur_op)))) {
                    if (!process_op(st, op.top()))
                    {
                        err(2, str);
                        return false;
                    }
                    op.pop();
                }
                op.push(cur_op);
                may_be_unary = true;
            }
            else
            {
                std::string tmp; tmp.clear();
                while (i < str.size() && !is_op(str[i]) && str[i] != '(' && str[i] != ')')
                    tmp += str[i], ++ i;
                -- i;

                int poly;
                if (!parseExpression(tmp, poly)) return false;
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
            if (!process_op(st, op.top()))
            {
                err(2, str);
                return false;
            }
            op.pop();
        }
        out_Int = st.top();
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
        if (args[0].find_first_not_of(' ') == std::string::npos or
            args[1].find_first_not_of(' ') == std::string::npos)
            command = str;
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
            int expression;
            if (parseExpression(args[1], expression) and parseVariable(args[0], var, true))
                var->second = expression;
        }
        else if (command == "calc" or command == "c")
        {
            if (args.size() == 0)
            {
                err(4);
                continue;
            }
            std::string arg;
            arg.clear();
            for (auto it : args) arg += it;

            int expression;
            if (parseExpression(arg, expression))
            {
                std::cout << expression << "\n";
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
            for (auto it : IntMap)
            {
                std::cout << std::setw(10) << std::left << it.first;
                std::cout << it.second << "\n";
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
                IntMap.erase(var);
        }
        else if (command.size() and command != "exit")
            err(3, command);
    }
    
    return 0;
}
