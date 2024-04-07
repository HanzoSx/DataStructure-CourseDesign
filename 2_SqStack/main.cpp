#include <iostream>
#include <vector>
#include "Stack.hpp"

class StackOrderCalcer
{
public:

    typedef std::vector<int> ResArr;
    StackOrderCalcer() : m_size(0) {}

    int build(int size)
    {
        m_size = size;
        m_res.clear();
        buildRecursion(0);
        return m_res.size();
    }

    size_t size() const { return m_res.size(); }

    const ResArr& get(size_t index) const
    {
        return m_res.at(index);
    }


private:

    int m_size;

    std::vector<ResArr> m_res;

    void buildRecursion(int val)
    {
        static Stack stack;
        static ResArr res;

        if (val == 0) stack.clear(), res.clear();
        if (val == m_size)
        {
            Stack tmpStack = stack;
            ResArr tmpRes = res;
            while (!tmpStack.empty())
            {
                tmpRes.push_back(tmpStack.top());
                tmpStack.pop();
            }
            m_res.push_back(tmpRes);
            return;
        }

        if (!stack.empty())
        {
            int tmp = stack.top();
            res.push_back(tmp);
            stack.pop();
            buildRecursion(val);
            res.pop_back();
            stack.push(tmp);
        }
        stack.push(val);
        buildRecursion(val + 1);
        stack.pop();
    }

};

int main(int argc, char const *argv[])
{
    // for (size_t i = 0; i < argc; ++ i)
    // {
    //     std::cout << argv[i] << "\n";
    // }
    
    StackOrderCalcer stackc;
    stackc.build(std::stoi(argv[1]));
    for (size_t i = 0, size = stackc.size(); i < size; ++ i)
    {
        StackOrderCalcer::ResArr res = stackc.get(i);
        std::cout << "[" << i << "] ";
        for (size_t j = 0; j < res.size(); j++)
            std::cout << stackc.get(i)[j] << " ";
        std::cout << "\n";
    }
    
    return 0;
}
