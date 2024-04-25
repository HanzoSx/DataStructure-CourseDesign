#include <simpleButton.hpp>

simpleButton::simpleButton() : m_select(false), m_selectDraw(nullptr), m_unselectDraw(nullptr)
{
}

simpleButton::simpleButton(sf::Vector2f size) : simpleButton()
{
    setViewSize(size);
    m_shadowRect.setSize(size);
}

simpleButton::simpleButton(float width, float height) : simpleButton(sf::Vector2f(width, height))
{
}

void simpleButton::setDrawable(sf::Drawable &drawable)
{
    setSelectDrawable(drawable);
    setUnselectDrawable(drawable);
}

void simpleButton::setSelectDrawable(sf::Drawable &drawable)
{
    m_selectDraw = &drawable;
}

void simpleButton::setUnselectDrawable(sf::Drawable &drawable)
{
    m_unselectDraw = &drawable;
}

void simpleButton::setSelect(bool isSelect)
{
    m_select = isSelect;
}

bool simpleButton::isSelect() const
{
    return m_select;
}

void simpleButton::processEvent(const sf::Event& event)
{
    Button::processEvent(event);
    sfu::Button::KeyState state = getKeyState(sf::Mouse::Left);
    switch (state)
    {
        case sfu::Button::KeyState::free: m_shadowRect.setFillColor(sf::Color(0, 0, 0, 0)); break;
        case sfu::Button::KeyState::hover: m_shadowRect.setFillColor(sf::Color(0, 0, 0, 30)); break;
        case sfu::Button::KeyState::press: m_shadowRect.setFillColor(sf::Color(0, 0, 0, 60)); break;
        default: break;
    }

    if (isPressed(sf::Mouse::Left))
        m_select = !m_select;
}

void simpleButton::render(sf::RenderTarget& target) const
{
    target.draw(m_shadowRect);
    if (m_select && m_selectDraw != nullptr)
        target.draw(*m_selectDraw);
    if (!m_select && m_unselectDraw != nullptr)
        target.draw(*m_unselectDraw);
}
