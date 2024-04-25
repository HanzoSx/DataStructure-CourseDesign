#pragma once

#include <Button.hpp>

class simpleButton : public sfu::Button
{
public:

    simpleButton();
    simpleButton(sf::Vector2f size);
    simpleButton(float width, float height);

    void setDrawable(sf::Drawable &drawable);
    void setSelectDrawable(sf::Drawable &drawable);
    void setUnselectDrawable(sf::Drawable &drawable);

    void setSelect(bool isSelect);
    bool isSelect() const;

protected:

    void processEvent(const sf::Event& event) override;
    void render(sf::RenderTarget& target) const override;

private:

    bool m_select;
    sf::RectangleShape m_shadowRect;
    sf::Drawable *m_selectDraw, *m_unselectDraw;
    
};
