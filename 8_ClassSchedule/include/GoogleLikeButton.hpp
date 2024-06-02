#ifndef SFUI_GOOGLE_LIKE_BUTTON_HPP
#define SFUI_GOOGLE_LIKE_BUTTON_HPP

#include <string>
#include <SFML/Graphics.hpp>

#include <DynamicParameter.hpp>
#include <Shapes.hpp>
#include <Button.hpp>

namespace sfu
{


class GoogleLikeButton : public Button
{
public:

    GoogleLikeButton();
    GoogleLikeButton(sf::Drawable& drawable, float radius, float width = 0.f);

    GoogleLikeButton(const GoogleLikeButton &other);

    void setDrawableTarget(sf::Drawable& drawable);
    void setRadius(float radius);
    void setWidth(float width);

    float getRadius() const;
    float getWidth() const;

    void Select();
    void Unselect();

    bool isSelect() const;

protected:

    void processEvent(const sf::Event& event) override;
    void render(sf::RenderTarget& target) const override;

private:

    void update() const;
    
    sf::Drawable* m_drawable;
    float m_radius, m_width;

    mutable bool m_needUpdate;
    mutable DynamicParameter dp_shadowRadius, dp_gloableA, dp_shadowA;
    sf::Vector2f m_center;
    bool m_select;

    mutable sfu::CircleFan m_circleL, m_circleR;
    mutable sf::RectangleShape m_rect;

    mutable sf::Shader m_shader;
};

static const std::string GoogleLikeButton_frag = R"(
uniform vec2 center;
uniform float radius;
uniform vec4 circle;
uniform vec4 shadow;

void main() {
    float distanceToCenter = distance(gl_FragCoord.xy, center);
    if (distanceToCenter < radius)
        gl_FragColor = shadow;
    else
        gl_FragColor = circle;
}
)";


} // namespace sfu

#endif
