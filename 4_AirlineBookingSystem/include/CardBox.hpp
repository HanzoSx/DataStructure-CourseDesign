#pragma once

#include <list>
#include <Control.hpp>
#include <windowWithScrollBar.hpp>

#include <Airline.hpp>
#include <flightCard.hpp>

class CardBox : public sfu::Control
{
public:

    CardBox(sf::Font &font);

    void clear();
    void add(Flight &flight);
    void add(Order &order);

    void setSize(float width, float height);
    void setCallbackBook(flightCard::func_book fun);
    void setCallbackCancel(flightCard::func_cancel fun);

protected:

    void processEvent(const sf::Event& event) override;
    void render(sf::RenderTarget& target) const override;

private:
    
    sf::Vector2f m_size;
    sf::Font &m_font;
    flightCard::func_book m_fun_book;
    flightCard::func_cancel m_fun_cancel;

    mutable std::list<flightCard> m_card;
    mutable windowWithScrollBar m_window;

};
