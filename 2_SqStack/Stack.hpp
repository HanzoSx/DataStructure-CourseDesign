#ifndef STACK_HPP
#define STACK_HPP

#include <cstring>

class Stack
{
public:

    Stack();
    Stack(const Stack &other);
    ~Stack();

    void push(int val);
    void pop();
    void clear();

    bool empty() const;
    size_t size() const;
    int top() const;

    Stack& operator = (const Stack& other);

private:

    struct LinkList
    {
        int val;
        LinkList *nxt;
    }   *m_data, *m_top;
    

};

#endif // STACK_HPP
