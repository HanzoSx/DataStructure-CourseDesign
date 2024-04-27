#include <User.hpp>
#include <Airline.hpp>

User::User(const std::string &name) :
m_name(name)
{
}

void User::book(Flight &flight, const std::array<int, 3> &seats)
{
    m_orders.emplace_back(*this, flight, seats);
    flight.book(m_orders.back());
}

void User::cancel(Order &order)
{
    for (auto i = m_orders.begin(); i != m_orders.end(); ++ i)
        if (&*i == &order)
        {
            order.getFlight().cancel(order);
            m_orders.erase(i);
            break;
        }
}

const std::list<Order>& User::orders() const
{
    return m_orders;
}


Order::Order(User &user, Flight &flight, const std::array<int, 3> &seats) :
m_user     (user),
m_flight   (flight),
m_seats    (seats),
m_isStandby(false)
{
}

bool Order::isStandby() const
{
    return m_isStandby;
}

void Order::setStandby()
{
    m_isStandby = true;
}

void Order::setBooked()
{
    m_isStandby = false;
}

Flight& Order::getFlight() const
{
    return m_flight;
}

User& Order::getUser() const
{
    return m_user;
}

const std::array<int, 3>& Order::getSeats() const
{
    return m_seats;
}

