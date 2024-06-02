#include "node.hpp"

Node::Node(const std::string &name, sf::Font &font, float radius)
    : m_radius(radius),
      m_selected(false),
      m_hover(false),
      m_del(false)
{
    m_text.setFont(font);
    m_text.setFillColor(sf::Color::Black);
    m_text.setCharacterSize(30);
    m_text.setString(name);
    m_text.setOrigin(
        m_text.getLocalBounds().width / 2,
        m_text.getLocalBounds().height / 2 * 1.7
    );
}

void Node::set_position(float x, float y) { m_pos = sf::Vector2f(x, y); }
void Node::set_position(sf::Vector2f pos) { m_pos = pos; }

void Node::set_speed(float x, float y) { m_speed = sf::Vector2f(x, y); }
void Node::set_speed(sf::Vector2f speed) { m_speed = speed; }

void Node::set_a(float x, float y) { m_a = sf::Vector2f(x, y); }
void Node::set_a(sf::Vector2f a) { m_a = a; }

sf::Vector2f Node::get_position() const { return m_pos; }
sf::Vector2f Node::get_speed() const { return m_speed; }
sf::Vector2f Node::get_a() const { return m_a; }
std::string Node::get_name() const { return m_text.getString(); }
bool Node::get_click() const { return m_clicked; }
bool Node::get_hover() const { return m_hover; }
bool Node::get_del() const { return m_del; }

void Node::update(sf::Time time)
{
    if (m_selected)
    {
        set_a(0, 0);
        set_speed(0, 0);
        return;
    }
    m_speed += m_a * time.asSeconds();

    float speed_limit = 500.f;
    if (float speed = sqrt(pow(m_speed.x, 2) + pow(m_speed.y, 2)); speed > speed_limit)
        m_speed = m_speed / speed * speed_limit;

    m_pos += m_speed * time.asSeconds();
}

void Node::add_edge(Edge &edge) { m_edge.emplace_back(&edge); }
std::list<Edge*>& Node::get_edge() { return m_edge; }

void Node::processEvent(const sf::Event& event)
{
    if (m_del) return;
    m_clicked = false;
    switch (event.type)
    {
    case sf::Event::EventType::MouseMoved:
    {
        sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
        float dis = sqrt(pow(mousePos.x - m_pos.x, 2) + pow(mousePos.y - m_pos.y, 2));
        m_hover = dis < m_radius;

        if (m_selected)
        {
            set_position(m_pos + mousePos - m_presspos);
            m_presspos = mousePos;
        }
        break;
    }
    case sf::Event::EventType::MouseButtonPressed:
    {
        if (m_hover && event.mouseButton.button == sf::Mouse::Right) 
        {
            m_del = true;
            for (auto &edge : m_edge)
                edge->del();
        }
        if (event.mouseButton.button != sf::Mouse::Left) break;
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        if (m_hover)
        {
            m_selected = true;
            m_presspos = mousePos;
            m_clicked = true;
        }
        break;
    }
    case sf::Event::EventType::MouseButtonReleased:
    {
        if (event.mouseButton.button != sf::Mouse::Left) break;
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        if (m_hover)
        {
            m_selected = false;
        }
        break;
    }
    
    default:
        break;
    }
}

void Node::render(sf::RenderTarget &target) const
{
    if (m_del) return;
    m_circle.setRadius(m_radius);
    m_circle.setPosition(m_pos);
    m_circle.setFillColor(m_speed == sf::Vector2f(0, 0) ? sf::Color::Green : sf::Color(130, 130, 255));
    m_circle.setOrigin(sf::Vector2f(m_radius, m_radius));
    target.draw(m_circle);

    m_text.setPosition(m_pos);
    target.draw(m_text);
}

Edge::Edge(Node &node0, Node &node1)
    : m_del(false)
{
    m_node[0] = &node0;
    m_node[1] = &node1;
    node0.add_edge(*this);
    node1.add_edge(*this);

    m_rect.setFillColor(sf::Color(150, 150, 150));
    m_rect.setOutlineColor(sf::Color(70, 70, 70));
    // m_rect.setOutlineThickness(4);

    m_circle.setRadius(20);
    m_circle.setPointCount(3);
    m_circle.setFillColor(sf::Color(70, 70, 70));
    m_circle.setOrigin(20, 20);
}

Node* Edge::other(Node &node)
{
    if (&node == m_node[0]) return m_node[1];
    if (&node == m_node[1]) return m_node[0];
    return nullptr;
}

Node* Edge::node(size_t index) const
{
    return m_node[index];
}

void Edge::del() { m_del = true; }
bool Edge::get_del() const { return m_del; }

void Edge::processEvent(const sf::Event& event)
{
}

void Edge::render(sf::RenderTarget &target) const
{
    if (m_del) return;
    sf::Vector2f delta = m_node[0]->get_position() - m_node[1]->get_position();
    float dis = sqrt(pow(delta.x, 2) + pow(delta.y, 2));
    m_rect.setSize(sf::Vector2f(dis, 8));
    m_rect.setOrigin(
        m_rect.getLocalBounds().width / 2,
        m_rect.getLocalBounds().height / 2
    );
    m_rect.setPosition(m_node[0]->get_position() - delta / 2.f);
    m_rect.setRotation(atan2(delta.y, delta.x) / M_PI * 180.f);
    target.draw(m_rect);

    m_circle.setPosition(m_node[0]->get_position() - delta / dis * (dis - 30.f - 20.f));
    m_circle.setRotation(atan2(delta.y, delta.x) / M_PI * 180.f + 30.f);
    target.draw(m_circle);
}
