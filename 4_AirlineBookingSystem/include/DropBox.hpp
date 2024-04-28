#pragma once

#include <list>

#include <Control.hpp>
#include <simpleButton.hpp>
#include <windowWithScrollBar.hpp>

class DropBox : public sfu::Control
{
public:

    DropBox(sf::Font &font);

    void add(sf::String string);
    sf::String get() const;

protected:

    void processEvent(const sf::Event& event) override;
    void render(sf::RenderTarget& target) const override;

private:

    bool m_expend;
    sf::Font &m_font;

    mutable simpleButton m_button;
    mutable windowWithScrollBar m_window;
    
    struct WB
    {
        simpleButton button;
        sf::Text text;
        int index;
    };
    WB *m_index;
    mutable std::list<WB> m_winbutton;

    static const sf::Vector2f s_size;
    
};
