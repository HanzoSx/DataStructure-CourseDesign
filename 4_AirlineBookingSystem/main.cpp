#include <iostream>
#include <fstream>
#include <map>

#include <CSVdata.hpp>

#include <SFUI.hpp>
#include <simpleScrollBar.hpp>
#include <windowWithScrollBar.hpp>
#include <uintNumber.hpp>
#include <flightCard.hpp>
#include <DropBox.hpp>
#include <CardBox.hpp>

#include <Airline.hpp>
#include <User.hpp>

std::map<std::string, AirPort> airports;
std::map<std::string, Airline> airlines;
std::map<std::string, Flight> flights;

std::map<std::string, User> users;
User *currentUser;

void loadAirlineData()
{
    CSVdata AirlineData;
    AirlineData.loadFromFile("./AirlineData.csv");
    bool firstLine = true;
    for (auto &i : AirlineData.m_data)
    {
        if (firstLine){ firstLine = false; continue; }
        Time tf(1, std::stoi(i[2]), std::stoi(i[3]), std::stoi(i[4]), 0);
        Time ar(1, std::stoi(i[5]), std::stoi(i[6]), std::stoi(i[7]), 0);
        std::array<int, 3> seats = {std::stoi(i[10]), std::stoi(i[11]), std::stoi(i[12])};
        std::string from(i[8]), to(i[9]), flightId(i[0]), planeId(i[1]);
        if (airports.count(from) == 0)
            airports.insert(std::make_pair(from, AirPort(from)));
        if (airports.count(to) == 0)
            airports.insert(std::make_pair(to, AirPort(to)));
        if (airlines.count(from + to) == 0)
            airlines.insert(std::make_pair(from + to, Airline(airports.at(from), airports.at(to))));
        flights.insert(std::make_pair(
            flightId,
            Flight(
                airlines.at(from + to),
                tf, ar,
                flightId, planeId,
                seats
            )
        ));
    }
    
}

void cb_book(Flight &flight, std::array<int, 3> seats)
{
    if (currentUser != nullptr)
        currentUser->book(flight, seats);
}

bool needupdate = false;
void cb_cancel(Order &Order)
{
    if (currentUser != nullptr)
        currentUser->cancel(Order);
    needupdate = true;
}

void updateCardBox(CardBox &box, std::string from, std::string to)
{
    box.clear();
    for (auto &i : flights)
    {
        if ((from == "" || from == i.second.getAirline().getFrom().getName()) &&
            (to == "" || to == i.second.getAirline().getTo().getName()))
            box.add(i.second);
    }
}

void updateCancelBox(CardBox &box, std::string user)
{
    box.clear();
    if (!users.count(user)) return;
    for (auto &i : users.at(user).orders())
    {
        box.add(i);
    }
    
}

void init()
{
    loadAirlineData();
    users.insert(std::make_pair("user1", User("user1")));
    users.insert(std::make_pair("user2", User("user2")));
    users.insert(std::make_pair("user3", User("user3")));
    users.insert(std::make_pair("user4", User("user4")));
}

int main(int argc, char const *argv[])
{

    init();

    sf::Vector2f windowSize(1000, 800);
    sfu::Window UIwindow;
    UIwindow.creat(sf::VideoMode(windowSize.x, windowSize.y), "Airline Book System", sf::Style::Default);
    sf::RenderWindow &window = UIwindow.getWindow();

    sf::Font font;
    font.loadFromFile("./DENG.TTF");

    //////////////////////////////////////////////////
    DropBox userBox(font);
    UIwindow.push_back(userBox);
    userBox.setViewPosition(sf::Vector2f(50, 20));
    for (auto &&i : users)
        userBox.add(i.first);

    DropBox flightFromBox(font);
    UIwindow.push_back(flightFromBox);
    flightFromBox.setViewPosition(sf::Vector2f(250, 20));
    for (auto &&i : airports)
        flightFromBox.add(i.first);

    DropBox flightToBox(font);
    UIwindow.push_back(flightToBox);
    flightToBox.setViewPosition(sf::Vector2f(450, 20));
    for (auto &&i : airports)
        flightToBox.add(i.first);
    
    CardBox flightBox(font);
    UIwindow.push_front(flightBox);
    flightBox.setCallbackBook(cb_book);
    flightBox.setSize(800, 600);
    flightBox.setViewPosition(sf::Vector2f(100, 100));
    
    updateCardBox(flightBox, std::string(flightFromBox.get()), std::string(flightToBox.get()));
    
    CardBox cancelBox(font);
    UIwindow.push_front(cancelBox);
    cancelBox.setCallbackCancel(cb_cancel);
    cancelBox.setSize(800, 600);
    cancelBox.setViewPosition(sf::Vector2f(100, 100));
    cancelBox.disable();
    cancelBox.invisiable();

    updateCancelBox(cancelBox, userBox.get());

    std::string lastFrom = flightFromBox.get();
    std::string lastTo = flightToBox.get();
    std::string lastUser = userBox.get();
    //////////////////////////////////////////////////
    simpleButton bookPageButton(80, 50), cancelPageButton(80, 50);
    bookPageButton.setViewPosition(sf::Vector2f(700, 20));
    cancelPageButton.setViewPosition(sf::Vector2f(800, 20));
    UIwindow.push_back(bookPageButton);
    UIwindow.push_back(cancelPageButton);
    sf::Text bookText("Book", font, 25), cancelText("Cancel", font, 25);
    bookText.setFillColor(sf::Color::Black);
    cancelText.setFillColor(sf::Color::Black);
    bookPageButton.setDrawable(bookText);
    cancelPageButton.setDrawable(cancelText);

    bool page = false;
    //////////////////////////////////////////////////

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::EventType::Closed:
                window.close();
                break;

            case sf::Event::EventType::Resized:
            {
                windowSize = sf::Vector2f(event.size.width, event.size.height);
                UIwindow.update();

                break;
            }
            
            default:
                break;
            }
            UIwindow.handleEvent(event);

            if (bookPageButton.isPressed(sf::Mouse::Left) ||
                cancelPageButton.isPressed(sf::Mouse::Left))
            {
                page = cancelPageButton.isPressed(sf::Mouse::Left);
                if (page)
                {
                    flightBox.disable();
                    flightBox.invisiable();

                    cancelBox.enable();
                    cancelBox.visiable();
                    updateCancelBox(cancelBox, userBox.get());
                }
                else
                {
                    flightBox.enable();
                    flightBox.visiable();

                    cancelBox.disable();
                    cancelBox.invisiable();
                }
            }
        }

        if (needupdate)
        {
            needupdate = false;
            updateCancelBox(cancelBox, userBox.get());
        }
        //////////////////////////////////////////////////
        bool fromChanged = (lastFrom != flightFromBox.get());
        bool toChanged = (lastTo != flightToBox.get());
        bool userChanged = (lastUser != userBox.get());
        lastFrom = flightFromBox.get();
        lastTo = flightToBox.get();
        lastUser = userBox.get();
        if (userChanged)
            currentUser = &users.at(userBox.get());
        //////////////////////////////////////////////////
        if (fromChanged || toChanged)
            updateCardBox(flightBox, std::string(flightFromBox.get()), std::string(flightToBox.get()));
        //////////////////////////////////////////////////
        if (userChanged)
            updateCancelBox(cancelBox, userBox.get());
        //////////////////////////////////////////////////


        window.clear(sf::Color::White);
        window.draw(UIwindow);
        window.display();

        sf::sleep(sf::milliseconds(5));
    }
    

    return 0;
}
