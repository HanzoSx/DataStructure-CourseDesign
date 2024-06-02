#include <iostream>
#include <math.h>
#include <vector>
#include <array>
#include <deque>
#include <string>

#include <SFUI.hpp>
#include <GoogleLikeButton.hpp>

#include <node.hpp>
#include <mapdata.hpp>
#include <file_selector.hpp>


int main(int argc, char const *argv[])
{
    srand(time(0));
    sf::Font font;
    font.loadFromFile("DENG.TTF");

    sf::Vector2f windowSize(2000, 1600);
    sfu::Window UIwindow;
    UIwindow.creat(sf::VideoMode(windowSize.x, windowSize.y), "Test", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);
    sf::RenderWindow &window = UIwindow.getWindow();

    //////////////////////////////////////////////////
    auto mid_text = [](sf::Text& text, float radius) {
        text.setFillColor(sf::Color::Black);
        text.setOrigin(
            text.getLocalBounds().width / 2,
            text.getLocalBounds().height / 2 * 1.9
        );
        text.setPosition(radius, radius);
    };

    sf::Text readfile_text("OPEN", font);
    mid_text(readfile_text, 40);
    sfu::GoogleLikeButton readfile_button(readfile_text, 40);
    readfile_button.setViewPosition(sf::Vector2f(100, 50));
    UIwindow.push_back(readfile_button);

    sf::Text savefile_text("SAVE", font);
    mid_text(savefile_text, 40);
    sfu::GoogleLikeButton savefile_button(savefile_text, 40);
    savefile_button.setViewPosition(sf::Vector2f(200, 50));
    UIwindow.push_back(savefile_button);

    sf::Text sortfile_text("SORT", font);
    mid_text(sortfile_text, 40);
    sfu::GoogleLikeButton sortfile_button(sortfile_text, 40);
    sortfile_button.setViewPosition(sf::Vector2f(300, 50));
    UIwindow.push_back(sortfile_button);

    sf::Text text("", font);
    text.setFillColor(sf::Color::Black);
    sfu::SimpleImage image(text);
    image.setViewPosition(sf::Vector2f(400, 70));
    UIwindow.push_back(image);

    //////////////////////////////////////////////////
    std::deque<Node> NodeVec;
    std::deque<Edge> EdgeVec;

    auto new_node = [&NodeVec, &windowSize, &UIwindow, &font](const sf::Vector2f &pos, const std::string &name, const int &score) {
        NodeVec.emplace_back(name, font);
        auto &it = NodeVec.back();
        it.set_position(pos);
        it.setViewSize(windowSize);
        it.score = score;
        UIwindow.push_back(it);
    };

    auto new_edge = [&EdgeVec, &windowSize, &UIwindow](Node &node0, Node &node1) {
        EdgeVec.emplace_back(node0, node1);
        auto &it = EdgeVec.back();
        it.setViewSize(windowSize);
        UIwindow.push_front(it);
    };

    MapData mapdata;

    auto load_from_file = [&mapdata, &NodeVec, &EdgeVec, &new_node, &new_edge, &UIwindow]() {
        std::string filename = FileSelector::select_file(FileDialogType::Open);
        std::cout << "filename = " << filename << "\n";
        if (!filename.size()) return;

        for (auto &node : NodeVec)
            UIwindow.eraseElement(node);
        for (auto &edge : EdgeVec)
            UIwindow.eraseElement(edge);

        mapdata.readFromFile(filename);
        mapdata.toDeques(NodeVec, EdgeVec, new_node, new_edge);
    };

    auto save_to_file = [&mapdata, &NodeVec, &EdgeVec]() {
        std::string filename = FileSelector::select_file(FileDialogType::Save);
        std::cout << "filename = " << filename << "\n";
        if (!filename.size()) return;

        mapdata.fromDeques(NodeVec, EdgeVec);
        mapdata.writeToFile(filename);
    };

    auto sort = [&mapdata, &NodeVec, &EdgeVec](std::vector<std::string> &result) {
        mapdata.fromDeques(NodeVec, EdgeVec);
        return mapdata.topologicalSort(result);
    };
    //////////////////////////////////////////////////

    sf::Clock clock;
    clock.restart();

    bool ctrl = false;
    Node *nodeFrom(nullptr);

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
                windowSize = sf::Vector2f(event.size.width, event.size.height);
                break;

            case sf::Event::EventType::KeyPressed:
                if (event.key.code == sf::Keyboard::LControl)
                    ctrl = true;
                break;

            case sf::Event::EventType::KeyReleased:
                if (event.key.code == sf::Keyboard::LControl)
                {
                    ctrl = false;
                    nodeFrom = nullptr;
                }
                break;

            case sf::Event::EventType::MouseButtonPressed:
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    bool hover = false;
                    for (auto &node : NodeVec)
                        if (!node.get_del() && (hover |= node.get_hover())) break;
                    if (!hover)
                        new_node(
                            sf::Vector2f(event.mouseButton.x, event.mouseButton.y),
                            !NodeVec.empty() ?
                                std::to_string(std::stoi(NodeVec.back().get_name()) + 1)
                              : "0",
                            rand() % 5 + 1
                        );
                }
                break;
            }

            default:
                break;
            }
            UIwindow.handleEvent(event);

            if (readfile_button.isPressed(sf::Mouse::Button::Left))
            {
                load_from_file();
            }
            if (savefile_button.isPressed(sf::Mouse::Button::Left))
            {
                save_to_file();
            }
            if (sortfile_button.isPressed(sf::Mouse::Button::Left))
            {
                std::vector<std::string> res;
                std::string tmp;
                if (sort(res))
                    for (auto &i : res)
                        tmp += i + " ";
                else
                    tmp = "no solve";
                text.setString(tmp);
                image.setViewSize(sf::Vector2f(
                    text.getLocalBounds().width * 2,
                    text.getLocalBounds().height * 2
                ));
            }

            if (ctrl)
                for (auto &node : NodeVec)
                    if (node.get_click())
                    {
                        if (nodeFrom == nullptr)
                            nodeFrom = &node;
                        else
                        {
                            bool exist = false;
                            for (auto &edge : EdgeVec)
                                if (!edge.get_del() &&
                                    (nodeFrom == edge.node(0) && &node == edge.node(1) ||
                                     nodeFrom == edge.node(1) && &node == edge.node(0))
                                )
                                {
                                    exist = true;
                                    edge.del();
                                    break;
                                }
                            if (!exist)
                                new_edge(*nodeFrom, node);
                            
                            nodeFrom = nullptr;
                        }
                        break;
                    }
        }
        event.type = sf::Event::EventType::MouseMoved;
        event.mouseMove.x = sf::Mouse::getPosition(window).x;
        event.mouseMove.y = sf::Mouse::getPosition(window).y;
        UIwindow.handleEvent(event);

        auto repulsion = [](sf::Vector2f delta) {
            float dis = sqrt(pow(delta.x, 2) + pow(delta.y, 2)) - 60;
            if (dis <= 0.01f) dis = 0.01f;
            if (dis > 300.f) return sf::Vector2f(0, 0);

            const float k = 1500000;
            return -delta / dis * (k / dis / dis);
        };

        auto attraction = [](sf::Vector2f delta) {
            float dis = sqrt(pow(delta.x, 2) + pow(delta.y, 2)) - 60;
            if (dis <= 200.f) return sf::Vector2f(0, 0);

            const float k = 5;
            return delta / dis * (k * (float)pow(dis - 200.f, 2));
        };

        for (auto &it : NodeVec)
        {
            if (it.get_del()) continue;
            sf::Vector2f force(0, 0);
            sf::Vector2f delta = windowSize / 2.f - it.get_position();
            float dis = sqrt(pow(delta.x, 2) + pow(delta.y, 2));
            force += delta / 2.f;

            for (auto &other : NodeVec)
            {
                if (&it == &other or other.get_del()) continue;
                force += repulsion(other.get_position() - it.get_position());
            }

            auto edge = it.get_edge();
            for (auto &&e : edge)
            {
                if (e->get_del()) continue;
                Node &other = *e->other(it);
                force += attraction(other.get_position() - it.get_position());
            }
            
            it.set_a(force);
            it.set_a(it.get_a() - it.get_speed() * 2.f);
        }
        
        window.clear(sf::Color::White);

        sf::Time time = clock.restart();
        for (auto &&it : NodeVec) it.update(time);
        window.draw(UIwindow);

        window.display();
        sf::sleep(sf::milliseconds(10));
        
    }
    

    return 0;
}
