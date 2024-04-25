#pragma once

#include <Control.hpp>
#include <Window.hpp>
#include <SimpleImage.hpp>
#include <simpleScrollBar.hpp>

class windowWithScrollBar : public sfu::Window
{
public:

    windowWithScrollBar();

    void setSize(const sf::Vector2f size);
    void setTotalSize(const sf::Vector2f size);
    void setBarWidth(float width);

    void update();
    void adaptItemsInWindow();

protected:    

    void processEvent(const sf::Event &event) override;
    void render(sf::RenderTarget &target) const override;

private:

    float m_barWidth;
    sf::Vector2f m_size, m_totalSize;

    mutable simpleScrollBar m_heightbar, m_widthbar;
    mutable sf::RectangleShape m_rectShape;
    mutable sfu::SimpleImage m_rectImage;

};
