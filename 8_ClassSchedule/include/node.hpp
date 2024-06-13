#pragma once

#include <SFUI.hpp>
#include <list>
#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>

class Edge;

class Node : public sfu::Control
{
public:
    Node(const std::string &name, sf::Font &font, float radius = 30);
    
    void set_position(float x, float y);
    void set_position(sf::Vector2f pos);

    void set_speed(float x, float y);
    void set_speed(sf::Vector2f speed);

    void set_a(float x, float y);
    void set_a(sf::Vector2f a);

    sf::Vector2f get_position() const;
    sf::Vector2f get_speed() const;
    sf::Vector2f get_a() const;
    std::string get_name() const;
    bool get_click() const;
    bool get_hover() const;
    bool get_del() const;

    void update(sf::Time time);

    void add_edge(Edge &edge);
    std::list<Edge*>& get_edge();

    int score;
    
protected:

    void processEvent(const sf::Event& event) override;
    void render(sf::RenderTarget &target) const override;

private:
    float m_radius;
    sf::Vector2f m_pos, m_speed, m_a;
    bool m_hover, m_clicked, m_selected, m_del;
    sf::Vector2f m_presspos;

    mutable sf::CircleShape m_circle;
    mutable sf::Text m_text;
    std::list<Edge*> m_edge;
};

class Edge : public sfu::Control
{
public:
    Edge(Node &node0, Node &node1);
    
    Node* other(Node &node);
    Node* node(size_t index) const;
    
    void del();
    bool get_del() const;

protected:
    void processEvent(const sf::Event& event) override;
    void render(sf::RenderTarget &target) const override;

private:
    bool m_del;
    std::array<Node*, 2> m_node;
    mutable sf::RectangleShape m_rect;
    mutable sf::CircleShape m_circle;
};
