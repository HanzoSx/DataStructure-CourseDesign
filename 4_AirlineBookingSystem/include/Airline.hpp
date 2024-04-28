#pragma once

#include <string>
#include <array>
#include <vector>
#include <list>

class AirPort
{
public:

    AirPort(const std::string &name);

    std::string getName() const;

private:
    
    std::string m_name;

};

class Flight;
class Airline
{
public:

    Airline(AirPort &from, AirPort &to);

    void addFlight(Flight &flight);
    const std::list<Flight> flight() const;

    AirPort& getFrom() const;
    AirPort& getTo() const;

private:
    
    AirPort &m_AirportFrom, &m_AirportTo;
    std::list<Flight> m_flight;

};

struct Time
{
    Time(int month, int day, int hour, int min, int sec) :
    month(month), day(day), hour(hour), min(min), sec(sec){}
    int month, day, hour, min, sec;
};

class Order;
class Flight
{
public:

    Flight(
        Airline &airline,
        const Time &takeoff,
        const Time &arrive,
        const std::string &flightId,
        const std::string &planeId,
        const std::array<int, 3> totalSeats
    );

    const std::array<int, 3>& getTotalSeats() const;
    const std::array<int, 3>& getBookedSeats() const;

    void book(Order &order);
    void cancel(Order &order);

    const Airline& getAirline() const;
    const Time getTakeoffTime() const;
    const Time getArriveTime() const;
    const std::string getFlightId() const;
    const std::string getPlaneId() const;
    const std::array<int, 3> getAviliableSeats() const;

private:
    
    Airline &m_airline;
    Time m_tTakeoff, m_tArrive;

    std::string m_flightId, m_planeId;
    std::array<int, 3> m_totalSeats, m_bookedSeats;

    std::list<Order*> m_orders;

};
