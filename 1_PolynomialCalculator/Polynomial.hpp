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
    void remove(int exp);
    void set(int exp, double constant);

    void derivative();
    double value(double x);

    double at(int exp) const;
    double operator [](int exp) const;
    size_t size() const;
    std::pair<int, double> node(size_t index) const;

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
        int exp;
        double constant;
        LinkList *nxt;
    }   *m_data;

};

#endif
