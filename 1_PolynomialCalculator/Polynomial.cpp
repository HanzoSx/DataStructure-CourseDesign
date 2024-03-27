#include "Polynomial.hpp"

#include <list>

Polynomial::Polynomial()
{
    m_data.exp = 0;
    m_data.constant = 0.;
    m_data.nxt = nullptr;
}

Polynomial::Polynomial(const Polynomial &other) : Polynomial()
{
    *this = other;
}

Polynomial::~Polynomial()
{
    clear();
}

void Polynomial::clear()
{
    std::list<LinkList*> ptrList;
    LinkList *ptr = &m_data;
    ptrList.clear();
    while (ptr->nxt != nullptr)
    {
        ptr = ptr->nxt;
        ptrList.push_front(ptr);
    }
    for (auto &it : ptrList)
        delete it->nxt;

    m_data.constant = 0.;
    m_data.nxt = nullptr;
}

void Polynomial::remove(size_t exp)
{
    if (exp == 0)
    {
        m_data.constant = 0.;
        return;
    }
    LinkList *ptr = &m_data, *ptrLast = nullptr;
    while (ptr->exp != exp)
    {
        if (ptr->nxt == nullptr) return;
        ptrLast = ptr;
        ptr = ptr->nxt;
    }
    ptrLast->nxt = ptr->nxt;
    delete ptr;
}

void Polynomial::set(size_t exp, double constant)
{
    LinkList *ptr = &m_data, *ptrLast = nullptr;
    while (ptr != nullptr and ptr->exp < exp)
    {
        ptrLast = ptr;
        ptr = ptr->nxt;
    }
    if (ptr != nullptr and ptr->exp == exp)
        ptr->constant = constant;
    else
    {
        ptrLast->nxt = new LinkList;
        ptrLast->nxt->exp = exp;
        ptrLast->nxt->constant = constant;
        ptrLast->nxt->nxt = ptr;
    }
}

double Polynomial::at(size_t exp) const
{
    const LinkList *ptr = &m_data;
    while (ptr != nullptr)
    {
        if (ptr->exp == exp)
            return ptr->constant;
        ptr = ptr->nxt;
    }
    return 0.;
}

double Polynomial::operator [](size_t exp) const
{
    return at(exp);
}

size_t Polynomial::size() const
{
    size_t size = 0;
    const LinkList *ptr = &m_data;
    while (ptr != nullptr)
    {
        ptr = ptr->nxt;
        ++ size;
    }
    return size;
}

std::pair<size_t, double> Polynomial::node(size_t index) const
{
    const LinkList *ptr = &m_data;
    for (size_t i = 0; i < index; ++ i)
        if (ptr->nxt != nullptr)
            ptr = ptr->nxt;
        else
            break;
    return std::make_pair(ptr->exp, ptr->constant);
}

Polynomial& Polynomial::operator = (const Polynomial &other)
{
    if (this == &other) return *this;
    
    clear();
    for (size_t index = 0; index < other.size(); ++ index)
    {
        auto [exp, constant] = other.node(index);
        set(exp, constant);
    }
    return *this;
}

Polynomial Polynomial::operator + (const Polynomial &other) const
{
    Polynomial tmp; tmp = *this;
    for (size_t index = 0; index < other.size(); ++ index)
    {
        auto [exp, constant] = other.node(index);
        tmp.set(exp, constant + tmp.at(exp));
    }
    return tmp;
}

Polynomial Polynomial::operator - () const
{
    Polynomial tmp;
    for (size_t index = 0; index < size(); ++ index)
    {
        auto [exp, constant] = node(index);
        tmp.set(exp, -constant);
    }
    return tmp;
}

Polynomial Polynomial::operator - (const Polynomial &other) const
{
    return *this + -other;
}

Polynomial Polynomial::operator * (const Polynomial &other) const
{
    Polynomial tmp;
    for (size_t index = 0; index < size(); ++ index)
    {
        auto [exp, constant] = node(index);
        for (size_t _index = 0; _index < other.size(); ++ _index)
        {
            auto [_exp, _constant] = other.node(_index);
            tmp.set(exp+_exp, tmp.at(exp+_exp) + constant*_constant);
        }
    }
    return tmp;
}

Polynomial& Polynomial::operator += (const Polynomial &other)
{
    return *this = *this + other;
}

Polynomial& Polynomial::operator -= (const Polynomial &other)
{
    return *this = *this - other;
}

Polynomial& Polynomial::operator *= (const Polynomial &other)
{
    return *this = *this * other;
}

void Polynomial::Print()
{
    for (size_t index = 1; index < size(); ++ index)
        if (std::abs(node(index).second) < 1e-6)
        {
            remove(node(index).first);
            -- index;
        }

    for (size_t index = 0; index < size(); ++ index)
    {
        auto [exp, constant] = node(size() - index - 1);
        if (std::abs(constant) < 1e-6) continue;
        if (index > 0 or constant < 0)
            std::cout << (constant > 0 ? "+" : "-") << " ";
        if (exp == 0 or std::abs(std::abs(constant) - 1) > 1e-6)
            std::cout << std::abs(constant) << " ";
        if (exp > 0)
        {
            std::cout << "x";
            if (exp > 1)
                std::cout << "^" << exp;
            std::cout << " ";
        }
    }

    if (size() == 1 and std::abs(m_data.constant) < 1e-6)
        std::cout << 0;
}
