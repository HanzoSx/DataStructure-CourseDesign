#include <flightCard.hpp>

const float flightCard::s_unexpdHeight(120);
const float flightCard::s_expdHeight(120);

flightCard::flightCard(sf::Font &font, Flight &flight, bool book, bool standby) :
m_font      (font),
m_flight    (flight),
m_seatNum   ({uintNumber(font), uintNumber(font), uintNumber(font)}),
m_size      (400, s_unexpdHeight),
m_expand    (false),
m_button    (m_size),
m_bookbutton(60, 60),
m_book      (book),
m_standby   (standby)
{
    m_button.setViewFather(*this);
    for (auto &i : m_seatNum)
        i.setViewFather(*this);
    m_bookbutton.setViewFather(*this);
    
    setViewSize(m_size);

    m_button.setDrawable(m_sprite);

    sf::RenderTexture render;
    render.create(60, 60);
    render.clear(m_book ? sf::Color(120, 210, 120) : sf::Color(210, 120, 120));
    sf::Text text(m_book ? "book" : "cancel", font, 20);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    // text.setOutlineThickness(2);
    text.setPosition(8, 16);
    render.draw(text);
    render.display();
    s_bookBT = render.getTexture();
    s_bookBS.setTexture(s_bookBT, true);
        
    m_bookbutton.setDrawable(s_bookBS);
}

flightCard::flightCard(sf::Font &font, Order &order) : flightCard(font, order.getFlight(), false, order.isStandby())
{
    m_order = &order;

    size_t index = 0;
    for (auto &&i : m_seatNum)
    {
        i.disable();
        i.setNumber(m_order->getSeats().at(index));
        index ++;
    }
    
}

void flightCard::setWidth(float width)
{
    m_size.x = width;
    setViewSize(m_size);
}

float flightCard::getHeight() const
{
    return m_size.y;
}

void flightCard::setCallbackBook(func_book &fun)
{
    m_fun_book = fun;
}

void flightCard::setCallbackCancel(func_cancel &fun)
{
    m_fun_cancel = fun;
}

std::array<int, 3> flightCard::getSeats() const
{
    std::array<int, 3> arr;
    for (size_t i = 0; i < 3; ++ i)
        arr[i] = m_seatNum[i].getNumber();
    return arr;
}

void flightCard::processEvent(const sf::Event& event)
{
    m_button.setRenderWindow(getRenderWindow());
    m_bookbutton.setRenderWindow(getRenderWindow());
    for (auto &i : m_seatNum)
        i.setRenderWindow(getRenderWindow());
    
    m_button.handleEvent(event);
    for (auto &i : m_seatNum)
        i.handleEvent(event);
    m_bookbutton.handleEvent(event);

    if (m_button.isPressed(sf::Mouse::Left))
    {
        m_expand = !m_expand;
        m_size.y = s_unexpdHeight + (m_expand ? s_expdHeight : 0);
        setViewSize(m_size);
        if (m_book)
            for (auto &i : m_seatNum)
                i.setEnabled(m_expand);
        m_bookbutton.setEnabled(m_expand);
    }

    if (m_bookbutton.isPressed(sf::Mouse::Left))
    {
        if (m_book)
            m_fun_book(m_flight, getSeats());
        else
            m_fun_cancel(*m_order);
    }
}

void flightCard::render(sf::RenderTarget& target) const
{
    sf::RenderTexture render;
    render.create(m_size.x, s_unexpdHeight);
    render.clear(m_standby ? sf::Color(200, 150, 150) : sf::Color(200, 200, 200));

    sf::Text text;
    text.setFont(m_font);
    text.setFillColor(sf::Color::Black);

    auto drawtext = [&text](sf::Vector2f pos, sf::String str, uint size, sf::RenderTarget &target){
        text.setCharacterSize(size);
        text.setString(str);
        text.setOrigin(
            text.getLocalBounds().width / 2,
            text.getLocalBounds().height / 2 * 1.7
        );
        text.setPosition(pos);
        target.draw(text);
    };

    drawtext(
        sf::Vector2f(m_size.x * 0.2, s_unexpdHeight * 0.4),
        m_flight.getAirline().getFrom().getName(),
        20, render
    );

    drawtext(
        sf::Vector2f(m_size.x * 0.8, s_unexpdHeight * 0.4),
        m_flight.getAirline().getTo().getName(),
        20, render
    );

    Time tf = m_flight.getTakeoffTime();
    Time ar = m_flight.getArriveTime();
    drawtext(
        sf::Vector2f(m_size.x * 0.2, s_unexpdHeight * 0.6),
        std::to_string(tf.month) + "/" + std::to_string(tf.day) + " " +
        std::to_string(tf.hour) + ":" + std::to_string(tf.min),
        17, render
    );
    drawtext(
        sf::Vector2f(m_size.x * 0.8, s_unexpdHeight * 0.6),
        std::to_string(ar.month) + "/" + std::to_string(ar.day) + " " +
        std::to_string(ar.hour) + ":" + std::to_string(ar.min),
        17, render
    );
    drawtext(
        sf::Vector2f(m_size.x * 0.5, s_unexpdHeight * 0.25),
        m_flight.getFlightId(),
        17, render
    );
    drawtext(
        sf::Vector2f(m_size.x * 0.5, s_unexpdHeight * 0.65),
        m_flight.getPlaneId(),
        17, render
    );
    drawtext(
        sf::Vector2f(m_size.x / 2, s_unexpdHeight * 0.4),
        ">> ---------- >>",
        17, render
    );

    render.display();
    m_texture = render.getTexture();
    m_sprite.setTexture(m_texture, true);
    m_button.setSize(sf::Vector2f(m_size.x, s_unexpdHeight));

    if (m_expand)
    {
        sf::RectangleShape rect(sf::Vector2f(m_size.x, s_expdHeight));
        rect.setFillColor(sf::Color(220, 220, 220));
        rect.setPosition(0, s_unexpdHeight);
        target.draw(rect);

        for (size_t i = 0; i < 3; ++ i)
            drawtext(
                sf::Vector2f(
                    m_size.x * 0.2,
                    s_unexpdHeight + s_expdHeight * 0.25 * (i + 1)
                ),
                "Level " + std::to_string(i + 1) + "  " +
                std::to_string(m_flight.getAviliableSeats()[i]),
                18, target
            );

        for (size_t i = 0; i < 3; ++ i)
        {
            m_seatNum[i].setViewPosition(sf::Vector2f(
                m_size.x * 0.45, s_unexpdHeight + s_expdHeight * 0.3 * i
            ));
            target.draw(m_seatNum[i]);
        }
        m_bookbutton.setViewPosition(sf::Vector2f(
            m_size.x * 0.82, s_unexpdHeight + s_expdHeight * 0.5 - 30
        ));
        target.draw(m_bookbutton);
    }
    target.draw(m_button);
}
