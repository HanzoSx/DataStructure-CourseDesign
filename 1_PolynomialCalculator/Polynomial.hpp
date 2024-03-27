#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <iostream>
#include <utility>

class Polynomial
{
public:

    Polynomial();
    Polynomial(const Polynomial &other);
    ~Polynomial();

    void clear();
    void remove(size_t exp);
    void set(size_t exp, double constant);

    double at(size_t exp) const;
    double operator [](size_t exp) const;
    size_t size() const;
    std::pair<size_t, double> node(size_t index) const;

    Polynomial& operator = (const Polynomial &other); // deep copy

    Polynomial operator + (const Polynomial &other) const;
    Polynomial operator - () const;
    Polynomial operator - (const Polynomial &other) const;
    Polynomial operator * (const Polynomial &other) const;
    Polynomial& operator += (const Polynomial &other);
    Polynomial& operator -= (const Polynomial &other);
    Polynomial& operator *= (const Polynomial &other);

    void Print();

private:
    
    struct LinkList
    {
        size_t exp;
        double constant;
        LinkList *nxt;
    }   m_data;

};

#endif
