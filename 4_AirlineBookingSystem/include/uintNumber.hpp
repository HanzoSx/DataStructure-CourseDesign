#pragma once

#include <Control.hpp>
#include <simpleButton.hpp>

class uintNumber : public sfu::Control
{
public:

    uintNumber(sf::Font &font);

    void setNumber(unsigned int number);
    unsigned int getNumber() const;

    static void makeSprite();

protected:

    virtual void processEvent(const sf::Event& event) override;
    virtual void render(sf::RenderTarget& target) const override;

private:
    
    unsigned int m_number;

    mutable sf::Text m_text;
    mutable simpleButton m_addButton, m_decButton;

    static const unsigned int s_spriteSize = 30;
    static const sf::Vector2f s_size;
    static bool s_makeSprite;
    static sf::Texture s_addTexture, s_decTexture;
    static sf::Sprite s_addSprite, s_decSprite;

};

