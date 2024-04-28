#include <CardBox.hpp>


CardBox::CardBox(sf::Font &font) :
m_font(font)
{
    m_window.setViewFather(*this);
    m_window.setBarWidth(15);
}

void CardBox::clear()
{
    for (auto &i : m_card)
        m_window.eraseElement(i);
    m_card.clear();
}

void CardBox::add(Flight &flight)
{
    m_card.emplace_back(m_font, flight);
    m_card.back().setCallbackBook(m_fun_book);
    m_window.push_front(m_card.back());
}

void CardBox::add(Order &order)
{
    m_card.emplace_back(m_font, order);
    m_card.back().setCallbackCancel(m_fun_cancel);
    m_window.push_front(m_card.back());
}

void CardBox::setSize(float width, float height)
{
    m_size = sf::Vector2f(width, height);
    m_window.setSize(sf::Vector2f(width - 15, height - 15));
    setViewSize(m_size);
}

void CardBox::setCallbackBook(flightCard::func_book fun)
{
    m_fun_book = fun;
}

void CardBox::setCallbackCancel(flightCard::func_cancel fun)
{
    m_fun_cancel = fun;
}

void CardBox::processEvent(const sf::Event& event)
{
    m_window.setRenderWindow(getRenderWindow());
    m_window.handleEvent(event);
}

void CardBox::render(sf::RenderTarget& target) const
{
    float height = 10;
    for (auto &i : m_card)
    {
        i.setWidth(m_size.x * 0.8);
        i.setViewPosition(sf::Vector2f(m_size.x * 0.1, height));
        height += 10 + i.getHeight();
    }
    m_window.setTotalSize(sf::Vector2f(
        m_size.x - 15, std::max(height, m_size.y - 15)
    ));

    target.draw(m_window);
}

