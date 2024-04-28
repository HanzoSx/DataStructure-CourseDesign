#pragma once

#include <functional>
#include <array>
// #include <list>

#include <Control.hpp>
#include <simpleButton.hpp>
#include <uintNumber.hpp>

#include <Airline.hpp>
#include <User.hpp>

class flightCard : public sfu::Control
{
public:
    typedef std::function<void(Flight&, std::array<int, 3>)> func_book;
    typedef std::function<void(Order&)> func_cancel;

    flightCard(sf::Font &font, Flight &flight, bool book = true, bool standby = false);
    flightCard(sf::Font &font, Order &order);

    void setWidth(float width);
    float getHeight() const;

    void setCallbackBook(func_book &fun);
    void setCallbackCancel(func_cancel &fun);

    std::array<int, 3> getSeats() const;

protected:

    virtual void processEvent(const sf::Event& event) override;
    virtual void render(sf::RenderTarget& target) const override;

private:

    bool m_book;
    bool m_standby;
    sf::Vector2f m_size;

    Flight &m_flight;
    Order *m_order;
    func_book m_fun_book;
    func_cancel m_fun_cancel;
    bool m_expand;

    sf::Font &m_font;
    mutable sf::Texture m_texture;
    mutable sf::Sprite m_sprite;
    mutable simpleButton m_button, m_bookbutton;
    mutable std::array<uintNumber, 3> m_seatNum;

    static const float s_expdHeight, s_unexpdHeight;

    sf::Texture s_bookBT;
    sf::Sprite s_bookBS;
    
};
