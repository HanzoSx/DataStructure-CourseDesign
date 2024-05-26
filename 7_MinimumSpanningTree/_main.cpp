#include <iostream>
#include <math.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Node : public sf::Drawable
{
public:

    Node(float radius = 30) : m_radius(radius) {}

    void set_position(float x, float y) { m_pos = sf::Vector2f(x, y); }

    void set_speed(float x, float y) { m_speed = sf::Vector2f(x, y); }
    void set_speed(sf::Vector2f speed) { set_speed(speed.x, speed.y); }

    void set_a(float x, float y) { m_a = sf::Vector2f(x, y); }
    void set_a(sf::Vector2f a) { set_a(a.x, a.y); }

    sf::Vector2f get_position() { return m_pos; }
    sf::Vector2f get_speed() { return m_speed; }
    sf::Vector2f get_a() { return m_a; }

    void update(sf::Time time)
    {
        m_speed += m_a * time.asSeconds();
        m_pos += m_speed * time.asSeconds();
    }

protected:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        m_circle.setRadius(m_radius);
        m_circle.setPosition(m_pos);
        m_circle.setFillColor(m_speed == sf::Vector2f(0, 0) ? sf::Color::Black : sf::Color::Blue);
        m_circle.setOrigin(sf::Vector2f(m_radius, m_radius));
        target.draw(m_circle, states);
    }

private:

    sf::Vector2f m_pos, m_speed, m_a;
    float m_radius;

    mutable sf::CircleShape m_circle;

};



int main(int argc, char const *argv[])
{
    sf::RenderWindow window(sf::VideoMode(2000, 1600), "Test", sf::Style::Close | sf::Style::Titlebar);

    std::vector<Node> NodeVec;

    for (size_t i = 0; i < 50; i++)
    {
        NodeVec.emplace_back();
        NodeVec.back().set_position(rand() % 2000, rand() % 1600);
    }
    sf::Clock clock;
    clock.restart();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::EventType::Closed:
                window.close();
                break;
            
            default:
                break;
            }
        }

        auto a = [](sf::Vector2f delta) {
            float dis = sqrt(pow(delta.x, 2) + pow(delta.y, 2));
            const float a = 120, b = a + 30, c = 10;
            if (dis > a or dis == 0.f) return sf::Vector2f(0, 0);
            return -delta / dis * (1000.f - dis * (1000.f / a) + 200.f);
        };
        for (auto &it : NodeVec)
        {
            sf::Vector2f force(0, 0);
            sf::Vector2f delta = sf::Vector2f(1000, 800) - it.get_position();
            float dis = sqrt(pow(delta.x, 2) + pow(delta.y, 2));
            force += delta / 2.f;

            for (auto &other : NodeVec)
            {
                if (&it == &other) continue;
                force += a(other.get_position() - it.get_position());
            }

            force += -it.get_speed() * 2.f;
            it.set_a(force);
        }
        
        window.clear(sf::Color::White);
        sf::Time time = clock.restart();
        for (auto &&it : NodeVec)
        {
            it.update(time);
            window.draw(it);
            std::cout << it.get_position().x << " " << it.get_position().y << "\n";
        }
        window.display();
        
    }
    

    return 0;
}
