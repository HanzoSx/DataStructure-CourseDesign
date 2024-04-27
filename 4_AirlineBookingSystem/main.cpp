#include <SFUI.hpp>
#include <simpleScrollBar.hpp>
#include <windowWithScrollBar.hpp>

int main(int argc, char const *argv[])
{
    sf::Vector2f windowSize(800, 600);
    sfu::Window UIwindow;
    UIwindow.creat(sf::VideoMode(windowSize.x, windowSize.y), "Airline Book System", sf::Style::Default);
    sf::RenderWindow &window = UIwindow.getWindow();

    bool doublePage = false;
    DynamicParameter dp(1);
    windowWithScrollBar page1;
    UIwindow.push_back(page1);

    // simpleScrollBar bar;
    // bar.setSize(20, 500, 20, 100);
    // page1.push_back(bar);
    sf::RectangleShape bgRect(sf::Vector2f(500, 500));
    bgRect.setFillColor(sf::Color::Blue);
    sfu::SimpleImage image(bgRect);
    image.setViewPosition(sf::Vector2f(100, 100));
    image.setViewSize(bgRect.getSize());

    page1.push_front(image);
    float barw = 60;
    page1.setBarWidth(barw);

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

            case sf::Event::EventType::Resized:
            {
                windowSize = sf::Vector2f(event.size.width, event.size.height);
                UIwindow.update();

                bool tmp_doublePage = windowSize.x > 1000;
                float val = tmp_doublePage ? 0.5 : 1;
                if (tmp_doublePage != doublePage)
                {
                    doublePage = tmp_doublePage;
                    dp.speedTO(val, float(doublePage ? -1 : 1) * 3);
                }
                else
                {
                    if (dp.isChanging())
                        dp.speedTO(val, float(doublePage ? -1 : 1) * 3);
                    else
                        dp.set(val);
                }
                break;
            }
            
            default:
                break;
            }
            UIwindow.handleEvent(event);
        }

        auto f = [](float x){
            // return (double)x;
            float wid = 12;
            float dx = exp(- 0.25 * wid);
            x = (x - 0.75) * wid;
            if (x < 0) x = exp(x); else x = -exp(-x) + 2;
            return (x - dx) / (2 - 2*dx) * (1 - 0.5) + 0.5;
        };
        page1.setSize(sf::Vector2f(windowSize.x * f(dp) - barw, windowSize.y - barw));
        page1.setTotalSize(sf::Vector2f(windowSize.x - barw, windowSize.y - barw + 100));
        // bar.setViewPosition(sf::Vector2f(windowSize.x * f(dp) - 10, 50));

        window.clear(sf::Color::White);
        window.draw(UIwindow);
        // window.draw(rect);
        window.display();

        // sf::sleep(sf::milliseconds(5));
    }
    

    return 0;
}
