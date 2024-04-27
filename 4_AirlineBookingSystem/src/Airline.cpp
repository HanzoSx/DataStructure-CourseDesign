#include <Airline.hpp>
#include <User.hpp>

AirPort::AirPort(const std::string &name) :
m_name(name)
{
}


Airline::Airline(AirPort &from, AirPort &to) :
m_AirportFrom(from),
m_AirportTo  (to)
{
}

void Airline::addFlight(Flight &flight)
{
    m_flight.push_back(flight);
}

const std::list<Flight> Airline::flight() const
{
    return m_flight;
}



Flight::Flight(
    Airline &airline,
    const Time &takeoff,
    const Time &arrive,
    const std::string &flightId,
    const std::string &planeId,
    const std::array<int, 3> totalSeats
) : 
m_airline   (airline),
m_tTakeoff  (takeoff),
m_tArrive   (arrive),
m_flightId  (flightId),
m_planeId   (planeId),
m_totalSeats(totalSeats)
{
}


const std::array<int, 3>& Flight::getTotalSeats() const
{
    return m_totalSeats;
}

const std::array<int, 3>& Flight::getBookedSeats() const
{
    return m_bookedSeats;
}

void Flight::book(Order &order)
{
    m_orders.emplace_back(&order);
    auto &seats = order.getSeats();
    if (seats[0] + m_bookedSeats[0] <= m_totalSeats[0] &&
        seats[1] + m_bookedSeats[1] <= m_totalSeats[1] &&
        seats[2] + m_bookedSeats[2] <= m_totalSeats[2])
    {
        m_bookedSeats[0] += seats[0];
        m_bookedSeats[1] += seats[1];
        m_bookedSeats[2] += seats[2];
        order.setBooked();
    }
    else
        order.setStandby();
}

void Flight::cancel(Order &order)
{
    for (auto i = m_orders.begin(); i != m_orders.end(); ++ i)
        if (*i == &order)
        {
            auto &seats = order.getSeats();
            m_bookedSeats[0] -= seats[0];
            m_bookedSeats[1] -= seats[1];
            m_bookedSeats[2] -= seats[2];
            m_orders.erase(i);
            break;
        }
    for (auto i = m_orders.begin(); i != m_orders.end(); ++ i)
        if ((*i)->isStandby())
        {
            auto &seats = order.getSeats();
            if (seats[0] + m_bookedSeats[0] <= m_totalSeats[0] &&
                seats[1] + m_bookedSeats[1] <= m_totalSeats[1] &&
                seats[2] + m_bookedSeats[2] <= m_totalSeats[2])
            {
                m_bookedSeats[0] += seats[0];
                m_bookedSeats[1] += seats[1];
                m_bookedSeats[2] += seats[2];
                order.setBooked();
            }
            else break;
        }
}
