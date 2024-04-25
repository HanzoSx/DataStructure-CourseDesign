#include <windowWithScrollBar.hpp>

windowWithScrollBar::windowWithScrollBar() : m_rectImage(m_rectShape)
{
    push_back(m_heightbar);
    push_back(m_widthbar);
    push_back(m_rectImage);
    m_heightbar.setCallBackInbound([this](){return inBound();});
    m_widthbar.setCallBackInbound([this](){return inBound();});

    m_rectShape.setFillColor(sf::Color(150, 150, 150));
}

void windowWithScrollBar::setSize(const sf::Vector2f size)
{
    sf::Vector2f offset = getViewLocalOffset();
    if (size.x > m_totalSize.x)
    {
        m_totalSize.x = size.x;
        offset.x = 0;
    }
    if (offset.x + size.x > m_totalSize.x)
        offset.x = m_totalSize.x - size.x;

    if (size.y > m_totalSize.y)
    {
        m_totalSize.y = size.y;
        offset.y = 0;
    }
    if (offset.y + size.y > m_totalSize.y)
        offset.y = m_totalSize.y - size.y;

    m_size = size;
    setViewOffset(offset);
    setViewSize(sf::Vector2f(m_size.x + m_barWidth, m_size.y + m_barWidth));

    m_heightbar.setSize(m_barWidth, m_size.y, m_barWidth, m_size.y * m_size.y / m_totalSize.y);
    m_widthbar.setSize(m_size.x, m_barWidth, m_size.x * m_size.x / m_totalSize.x, m_barWidth);
}

void windowWithScrollBar::setTotalSize(const sf::Vector2f size)
{
    sf::Vector2f offset = getViewLocalOffset();
    if (m_size.x > size.x)
    {
        m_size.x = size.x;
        offset.x = 0;
    }
    if (offset.x + m_size.x > size.x)
        offset.x = size.x - m_size.x;

    if (m_size.y > size.y)
    {
        m_size.y = size.y;
        offset.y = 0;
    }
    if (offset.y + m_size.y > size.y)
        offset.y = size.y - m_size.y;

    m_totalSize = size;
    setViewOffset(offset);
    setViewSize(sf::Vector2f(m_size.x + m_barWidth, m_size.y + m_barWidth));

    m_heightbar.setSize(m_barWidth, m_size.y, m_barWidth, m_size.y * m_size.y / m_totalSize.y);
    m_widthbar.setSize(m_size.x, m_barWidth, m_size.x * m_size.x / m_totalSize.x, m_barWidth);
}

void windowWithScrollBar::setBarWidth(float width)
{
    m_barWidth = width;
    setViewSize(sf::Vector2f(m_size.x + m_barWidth, m_size.y + m_barWidth));
}

void windowWithScrollBar::update()
{

}

void windowWithScrollBar::adaptItemsInWindow()
{

}

void windowWithScrollBar::processEvent(const sf::Event &event)
{
    m_heightbar.setRenderWindow(getRenderWindow());
    m_widthbar.setRenderWindow(getRenderWindow());
    sfu::Window::processEvent(event);
    // m_heightbar.handleEvent(event);
    // m_widthbar.handleEvent(event);

    sf::Vector2f moffset = m_totalSize - m_size;
    setViewOffset(sf::Vector2f(moffset.x * m_widthbar.getWidthScale(), moffset.y * m_heightbar.getHeightScale()));
    m_heightbar.setViewPosition(sf::Vector2f(m_size.x, 0) + getViewLocalOffset());
    m_widthbar.setViewPosition(sf::Vector2f(0, m_size.y) + getViewLocalOffset());
}

void windowWithScrollBar::render(sf::RenderTarget &target) const
{
    m_heightbar.setViewPosition(sf::Vector2f(m_size.x, 0) + getViewLocalOffset());
    m_widthbar.setViewPosition(sf::Vector2f(0, m_size.y) + getViewLocalOffset());
    m_rectShape.setSize(sf::Vector2f(m_barWidth, m_barWidth));
    m_rectImage.setViewSize(sf::Vector2f(m_barWidth, m_barWidth));
    m_rectImage.setViewPosition(sf::Vector2f(m_size.x, m_size.y) + getViewLocalOffset());

    sfu::Window::render(target);
}
