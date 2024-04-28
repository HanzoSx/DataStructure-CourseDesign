#pragma once

#include <string>
#include <array>
#include <list>

class Flight;
class Order;
class User
{
public:

    User(const std::string &name);
    void book(Flight &flight, const std::array<int, 3> &seats);
    void cancel(Order &order);

    std::list<Order>& orders();

private:
    
    std::string m_name;
    std::list<Order> m_orders;

};

class Order
{
public:

    Order(User &user, Flight &flight, const std::array<int, 3> &seats);

    bool isStandby() const;
    void setStandby();
    void setBooked();

    Flight& getFlight() const;
    User& getUser() const;
    const std::array<int, 3>& getSeats() const;

private:
    
    User &m_user;
    Flight &m_flight;
    std::array<int, 3> m_seats;

    bool m_isStandby;

};
