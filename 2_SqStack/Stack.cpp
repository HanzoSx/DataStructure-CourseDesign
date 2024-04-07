#include "Stack.hpp"

Stack::Stack() : m_data(nullptr), m_top(nullptr)
{
}

Stack::Stack(const Stack &other)
{
    *this = other;
}

Stack::~Stack()
{
    clear();
}

void Stack::push(int val)
{
    if (empty())
    {
        m_top = new LinkList;
        m_data = m_top;
    }
    else
    {
        m_top->nxt = new LinkList;
        m_top = m_top->nxt;
    }
    m_top->val = val;
    m_top->nxt = nullptr;
}

void Stack::pop()
{
    if (empty()) return;
    if (m_data == m_top)
    {
        delete m_top;
        m_data = m_top = nullptr;
        return;
    }
    LinkList* tmp = m_data;
    while (tmp->nxt != m_top)
        tmp = tmp->nxt;
    delete m_top;
    m_top = tmp;
}

void Stack::clear()
{
    while (!empty()) pop();
}

bool Stack::empty() const
{
    return m_data == nullptr;
}

size_t Stack::size() const
{
    size_t size = 0;
    LinkList *tmp = m_data;
    while (tmp != nullptr)
    {
        tmp = tmp->nxt;
        ++ size;
    }
    return size;
}

int Stack::top() const
{
    if (!empty()) return m_top->val;
    return 0;
}

Stack& Stack::operator = (const Stack& other)
{
    clear();
    LinkList *ptr = other.m_data;
    while (ptr != nullptr)
    {
        push(ptr->val);
        ptr = ptr->nxt;
    }
    return *this;
}

