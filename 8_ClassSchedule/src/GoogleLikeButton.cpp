#include <GoogleLikeButton.hpp>

#include <algorithm>
#include <iostream>

namespace sfu
{

GoogleLikeButton::GoogleLikeButton() :
m_drawable(nullptr),
m_select  (false)
{
    dp_gloableA.set(-30);
    dp_shadowA.set(0);
    dp_shadowRadius.set(0);
    m_needUpdate = true;

    m_circleL.setFanAngle(pi);
    m_circleR.setFanAngle(pi);
    m_circleL.setStartAngle(pi / 2);
    m_circleR.setStartAngle(- pi / 2);

    GoogleLikeButton::m_shader.loadFromMemory(GoogleLikeButton_frag, sf::Shader::Type::Fragment);
}

GoogleLikeButton::GoogleLikeButton(sf::Drawable& drawable, float radius, float width) : GoogleLikeButton()
{
    setDrawableTarget(drawable);
    setRadius(radius);
    setWidth(width);
}

GoogleLikeButton::GoogleLikeButton(const GoogleLikeButton &other) :
    GoogleLikeButton(*other.m_drawable, other.m_radius, other.m_width) {}


void GoogleLikeButton::processEvent(const sf::Event& event)
{
    Button::processEvent(event);

    if (isKeyStateChanged(sf::Mouse::Button::Left))
        switch (getKeyState(sf::Mouse::Button::Left))
        {
            case Button::KeyState::free:
                dp_gloableA.speedTO(-30, -1200);
                dp_shadowA.timeTO(0, 0.2);
                break;

            case Button::KeyState::hover:
                dp_gloableA.speedTO(255, 1200);
                dp_shadowA.timeTO(0, 0.2);
                break;

            case Button::KeyState::press:
            {
                dp_shadowA.set(1.f);
                dp_shadowRadius.timeHT(0, 1, 0.2);
                sf::Vector2i MousePos = sf::Mouse::getPosition(getRenderWindow());
                m_center = sf::Vector2f(MousePos.x, MousePos.y);
                break;
            }

            default:
                break;
        }
}

void GoogleLikeButton::render(sf::RenderTarget& target) const
{
    if (m_needUpdate)
    {
        update();
        m_needUpdate = false;
    }

    sf::Uint8 cGScale = 200;
    sf::Uint8 sGScale = cGScale - dp_shadowA.get() * (cGScale - 160);
    sf::Color c_circle(cGScale, cGScale, cGScale, std::max(0.f, dp_gloableA.get()));
    sf::Color c_shadow(sGScale, sGScale, sGScale, std::max(0.f, dp_gloableA.get()));
    if (m_select)
    {
        c_circle = sf::Color(cGScale, cGScale, cGScale, 255);
        c_shadow = sf::Color(sGScale, sGScale, sGScale, 255);
    }

    m_shader.setUniform("center", sf::Vector2f(m_center.x, getRenderWindow().getSize().y - m_center.y));
    m_shader.setUniform("radius", dp_shadowRadius.get() * (2 * m_radius + m_width));
    m_shader.setUniform("circle", sf::Glsl::Vec4(c_circle));
    m_shader.setUniform("shadow", sf::Glsl::Vec4(c_shadow));
    
    // m_circleL.setFillColor(c_circle);
    target.draw(m_circleL, &m_shader);
    target.draw(m_circleR, &m_shader);
    target.draw(m_rect, &m_shader);

    if (m_drawable != nullptr) target.draw(*m_drawable);
}

void GoogleLikeButton::setDrawableTarget(sf::Drawable& drawable)
{
    m_drawable = &drawable;
}

void GoogleLikeButton::setRadius(float radius)
{
    m_radius = radius;
    setViewSize(sf::Vector2f(m_radius * 2 + m_width, m_radius * 2));
    m_needUpdate = true;
}

void GoogleLikeButton::setWidth(float width)
{
    m_width = width;
    setViewSize(sf::Vector2f(m_radius * 2 + m_width, m_radius * 2));
    m_needUpdate = true;
}

float GoogleLikeButton::getRadius() const
{
    return m_radius;
}

float GoogleLikeButton::getWidth() const
{
    return m_width;
}

void GoogleLikeButton::Select()
{
    m_select = true;
}

void GoogleLikeButton::Unselect()
{
    m_select = false;
}

bool GoogleLikeButton::isSelect() const
{
    return m_select;
}

void GoogleLikeButton::update() const
{
    m_circleL.setRadius(m_radius);

    m_circleR.setRadius(m_radius);
    m_circleR.setPosition(m_width, 0);

    m_rect.setSize(sf::Vector2f(m_width, 2 * m_radius));
    m_rect.setPosition(m_radius, 0);
}


} // namespace sfu
