#include <simpleScrollBar.hpp>

simpleScrollBar::simpleScrollBar() :
m_rectSize    (0, 0),
m_barSize     (0, 0),
m_scale       (0, 0),
m_dpBarColor  (160),
m_sInbound    (false),
m_sPressed    (false),
m_sPressCoords(0, 0)
{
    m_rectRect.setFillColor(sf::Color(200, 200, 200));
}

void simpleScrollBar::setSize(float rectWidth, float rectHeight, float barWidth, float barHeight)
{
    setSize(
        sf::Vector2f(rectWidth, rectHeight),
        sf::Vector2f(barWidth, barHeight)
    );
}

void simpleScrollBar::setSize(sf::Vector2f rectSize, sf::Vector2f barSize)
{
    m_rectSize = rectSize;
    m_barSize = barSize;
    setViewSize(rectSize);
}

void simpleScrollBar::setScale(sf::Vector2f scale)
{
    setWidthScale(scale.x);
    setHeightScale(scale.y);
}

void simpleScrollBar::setWidthScale(float scale)
{
    m_scale.x = scale;
    if (m_scale.x < 0) m_scale.x = 0;
    if (m_scale.x > 1) m_scale.x = 1;
}

void simpleScrollBar::setHeightScale(float scale)
{
    m_scale.y = scale;
    if (m_scale.y < 0) m_scale.y = 0;
    if (m_scale.y > 1) m_scale.y = 1;
}

sf::Vector2f simpleScrollBar::getScale() const
{
    return m_scale;
}

float simpleScrollBar::getWidthScale() const
{
    return m_scale.x;
}

float simpleScrollBar::getHeightScale() const
{
    return m_scale.y;
}

void simpleScrollBar::setCallBackInbound(const std::function<bool()> &cbInbound)
{
    m_cbInbound = cbInbound;
}

void simpleScrollBar::processEvent(const sf::Event& event)
{
    Control::processEvent(event);
    switch (event.type)
    {
    case sf::Event::EventType::MouseMoved:
    {
        m_sInbound = inBound() or (m_cbInbound ? m_cbInbound() : false);
        sf::Vector2i mousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
        if (m_sPressed)
        {
            sf::Vector2f deltaMouseCoord = MouseCoords(mousePos) - m_sPressCoords;
            setWidthScale(m_sPressScale.x + (
                m_rectSize.x - m_barSize.x > 0 ?
                deltaMouseCoord.x / (m_rectSize.x - m_barSize.x) : 0
            ));
            setHeightScale(m_sPressScale.y + (
                m_rectSize.y - m_barSize.y > 0 ?
                deltaMouseCoord.y / (m_rectSize.y - m_barSize.y) : 0
            ));
            // m_sPressCoords = MouseCoords(mousePos);
        }
        break;
    }

    case sf::Event::EventType::MouseButtonPressed:
    {
        if (event.mouseButton.button != sf::Mouse::Left) break;
        if (!inBound()) break;

        m_sPressed = true;
        sf::Vector2i mousePos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
        if (!InBarBound(mousePos))
        {
            setWidthScale(
                m_rectSize.x - m_barSize.x > 0 ?
                (MouseCoords(mousePos).x - m_barSize.x / 2) / (m_rectSize.x - m_barSize.x) : 0);
            setHeightScale(
                m_rectSize.y - m_barSize.y > 0 ?
                (MouseCoords(mousePos).y - m_barSize.y / 2) / (m_rectSize.y - m_barSize.y) : 0);
        }

        m_sPressCoords = MouseCoords(mousePos); -
            sf::Vector2f(
                m_scale.x * (m_rectSize.x - m_barSize.x),
                m_scale.y * (m_rectSize.y - m_barSize.y)
            );
        m_sPressScale = m_scale;

        m_dpBarColor.timeTO(120, 0.5);
        break;
    }

    case sf::Event::EventType::MouseButtonReleased:
        if (event.mouseButton.button != sf::Mouse::Left) break;
        m_sPressed = false;
        m_dpBarColor.timeTO(160, 0.5);
        break;

    case sf::Event::EventType::MouseWheelScrolled:
        if (!m_sInbound or m_sPressed) break;
        switch (event.mouseWheelScroll.wheel)
        {
        case sf::Mouse::Wheel::HorizontalWheel:
            setWidthScale(getWidthScale() -
                event.mouseWheelScroll.delta * (m_barSize.x / m_rectSize.x * 0.25));
            break;

        case sf::Mouse::Wheel::VerticalWheel:
            setHeightScale(getHeightScale() -
                event.mouseWheelScroll.delta * (m_barSize.y / m_rectSize.y * 0.25));
            break;
        
        default:
            break;
        } ;
        break;
    
    default:
        break;
    }
}

void simpleScrollBar::render(sf::RenderTarget& target) const
{
    m_rectRect.setSize(m_rectSize);
    m_barRect.setSize(sf::Vector2f(
        std::min(m_rectSize.x, m_barSize.x),
        std::min(m_rectSize.y, m_barSize.y)
    ));
    m_barRect.setPosition(
        (m_rectSize.x - m_barSize.x) * m_scale.x,
        (m_rectSize.y - m_barSize.y) * m_scale.y
    );
    float grayVal = m_dpBarColor.get();
    sf::Color color(grayVal, grayVal, grayVal);
    m_barRect.setFillColor(color);

    target.draw(m_rectRect);
    target.draw(m_barRect);
}

sf::Vector2f simpleScrollBar::MouseCoords(sf::Vector2i mousePos) const
{
    return getRenderWindow().mapPixelToCoords(
        mousePos, getGlobalView(getRenderWindow())
    );
}

bool simpleScrollBar::InBarBound(sf::Vector2i mousePos) const
{
    sf::Vector2f pos = MouseCoords(mousePos);
    return sf::FloatRect(
        m_scale.x * (m_rectSize.x - m_barSize.x),
        m_scale.y * (m_rectSize.y - m_barSize.y),
        m_barSize.x,
        m_barSize.y
    ).contains(pos);
}
