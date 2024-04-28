#include <DropBox.hpp>


const sf::Vector2f DropBox::s_size(160, 30);
DropBox::DropBox(sf::Font &font) :
m_font  (font),
m_button(s_size.x + 10, s_size.y),
m_index (nullptr),
m_expend(false)
{
    m_button.setViewFather(*this);
    m_window.setViewFather(*this);
    m_window.setSize(sf::Vector2f(s_size.x, s_size.y * 5));
    m_window.setBarWidth(10);
    m_window.setViewPosition(sf::Vector2f(0, s_size.y));

    add("");
    m_index = &m_winbutton.back();

    setViewSize(sf::Vector2f(s_size.x + 10, s_size.y * (5+1) + 10));
}

void DropBox::add(sf::String string)
{
    m_window.setRenderWindow(getRenderWindow());
    WB tmp = {
        simpleButton(s_size), sf::Text(string, m_font), int(m_winbutton.size())
    };
    m_winbutton.push_back(tmp);
    WB &it = m_winbutton.back();
    m_window.push_front(it.button);
    it.button.setViewPosition(sf::Vector2f(0, it.index * s_size.y));
    it.button.setDrawable(it.text);
    it.text.setFillColor(sf::Color::Black);
    it.text.setCharacterSize(22);

    m_window.setTotalSize(sf::Vector2f(s_size.x, s_size.y * std::max(5, it.index)));
}

sf::String DropBox::get() const
{
    return m_index->text.getString();
}

void DropBox::processEvent(const sf::Event& event)
{
    m_winbutton.front().button.setRenderWindow(getRenderWindow());
    m_button.setRenderWindow(getRenderWindow());
    m_window.setRenderWindow(getRenderWindow());

    m_button.handleEvent(event);
    m_window.handleEvent(event);
    
    if (m_expend)
    for (auto &i : m_winbutton)
    {
        if (i.button.isPressed(sf::Mouse::Left))
        {
            m_expend = false;
            m_index = &i;
            break;
        }
    }
    
    if (m_button.isPressed(sf::Mouse::Left))
    {
        m_expend = !m_expend;
    }
    
    m_window.setEnabled(m_expend);
}

void DropBox::render(sf::RenderTarget& target) const
{
    sf::RenderTexture render;
    render.create(s_size.x + 10, s_size.y);
    render.clear(sf::Color(200, 200, 200));
    render.draw(m_index->text);
    render.display();
    sf::Texture texture = render.getTexture();
    sf::Sprite sprite(texture);
    m_button.setDrawable(sprite);

    sf::RectangleShape rect(sf::Vector2f(s_size.x + 10, s_size.y * (m_expend ? 6 : 1)));
    rect.setFillColor(sf::Color::White);
    target.draw(rect);

    target.draw(m_button);
    if (m_expend)
    {
        target.draw(m_window);
    }
}

