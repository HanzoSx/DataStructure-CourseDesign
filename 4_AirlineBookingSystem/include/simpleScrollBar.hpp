#pragma once

#include <Control.hpp>
#include <DynamicParameter.hpp>
#include <functional>

class simpleScrollBar : public sfu::Control
{
public:

    simpleScrollBar();

    void setSize(float rectWidth, float rectHeight, float barWidth, float barHeight);
    void setSize(sf::Vector2f rectSize, sf::Vector2f barSize);

    void setScale(sf::Vector2f scale);
    void setWidthScale(float scale);
    void setHeightScale(float scale);

    sf::Vector2f getScale() const;
    float getWidthScale() const;
    float getHeightScale() const;

    void setCallBackInbound(const std::function<bool()> &cbInbound);

protected:

    void processEvent(const sf::Event& event) override;
    void render(sf::RenderTarget& target) const override;

private:

    sf::Vector2f MouseCoords(sf::Vector2i mousePos) const;
    bool InBarBound(sf::Vector2i mousePos) const;

    sf::Vector2f m_rectSize, m_barSize;
    sf::Vector2f m_scale;
    DynamicParameter m_dpBarColor;
    mutable sf::RectangleShape m_rectRect, m_barRect;

    bool m_sInbound, m_sPressed;
    sf::Vector2f m_sPressCoords, m_sPressScale;
    std::function<bool()> m_cbInbound;

};
