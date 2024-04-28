#include <uintNumber.hpp>

uintNumber::uintNumber(sf::Font &font) :
m_number(0),
m_text("0", font),
m_addButton(s_spriteSize, s_spriteSize),
m_decButton(s_spriteSize, s_spriteSize)
{
    if (!s_makeSprite)
        makeSprite();
    m_addButton.setDrawable(s_addSprite);
    m_decButton.setDrawable(s_decSprite);
    m_text.setCharacterSize(s_spriteSize * 0.7);
    m_text.setFillColor(sf::Color::Black);

    m_addButton.setViewFather(*this);
    m_decButton.setViewFather(*this);

    m_addButton.setViewPosition(sf::Vector2f(
        s_size.x - (s_size.x - 2*s_spriteSize) * 0.1 - s_spriteSize,
        (s_size.y - s_spriteSize) / 2
    ));
    m_decButton.setViewPosition(sf::Vector2f(
        (s_size.x - 2*s_spriteSize) * 0.1,
        (s_size.y - s_spriteSize) / 2
    ));

    setViewSize(s_size);
}

void uintNumber::setNumber(unsigned int number)
{
    m_number = number;
}

unsigned int uintNumber::getNumber() const
{
    return m_number;
}

void uintNumber::processEvent(const sf::Event& event)
{
    m_addButton.setRenderWindow(getRenderWindow());
    m_decButton.setRenderWindow(getRenderWindow());

    m_addButton.handleEvent(event);
    m_decButton.handleEvent(event);

    if (m_addButton.isPressed(sf::Mouse::Left))
        m_number ++;
    if (m_decButton.isPressed(sf::Mouse::Left) && m_number > 0)
        m_number --;
}

void uintNumber::render(sf::RenderTarget& target) const
{
    m_text.setString(std::to_string(m_number));
    m_text.setOrigin(
        m_text.getLocalBounds().width / 2,
        m_text.getLocalBounds().height / 2 * 1.6
    );
    m_text.setPosition(s_size.x / 2, s_size.y / 2);

    target.draw(m_text);
    target.draw(m_addButton);
    target.draw(m_decButton);
}

const sf::Vector2f uintNumber::s_size(s_spriteSize * 5, s_spriteSize * 1.5);
bool uintNumber::s_makeSprite = false;
sf::Texture uintNumber::s_addTexture, uintNumber::s_decTexture;
sf::Sprite uintNumber::s_addSprite, uintNumber::s_decSprite;
void uintNumber::makeSprite()
{
    if (s_makeSprite) return;
    s_makeSprite = true;

    sf::RenderTexture render;
    render.create(s_spriteSize, s_spriteSize);

    sf::CircleShape circle(s_spriteSize / 2);
    circle.setFillColor(sf::Color(210, 210, 255));
    render.draw(circle);

    sf::RectangleShape rect(sf::Vector2f(s_spriteSize * 0.6, s_spriteSize * 0.15));
    rect.setFillColor(sf::Color::White);
    rect.setOrigin(rect.getLocalBounds().width / 2, rect.getLocalBounds().height / 2);
    rect.setPosition(s_spriteSize / 2, s_spriteSize / 2);
    render.draw(rect);

    render.display();
    s_decTexture = render.getTexture();
    s_decSprite.setTexture(s_decTexture, true);

    rect.setRotation(90);
    render.draw(rect);
    render.display();
    s_addTexture = render.getTexture();
    s_addSprite.setTexture(s_addTexture, true);
}
