1 需求分析

2 概要
2.1 用户文档
	编辑有向图：右键节点删除节点；右键空白处新建节点；按住左键拖拽节点位置;按住左Ctrl点击节点创建/删除边
	保存/加载 文件：保存当前图到文件/从文件加载图
	计算序列
	
2.2 概要设计
	节点分布使用了力导向算法，使得有边相连的节点尽可能聚集，而所有节点之间保持合适的距离，同时整体在屏幕中心
	图保存为文件时保存节点数和边数，随后列举节点信息和边信息
	
3 代码
3.1 结点与边

#include <deque>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include "node.hpp"

class MapData {
public:
    struct MapNode {
        std::string name;
        int score;
    };

    struct MapEdge {
        size_t from;
        size_t to;
    };

    MapData() {}

    // Convert from deques to MapData
    void fromDeques(const std::deque<Node> &nodes, const std::deque<Edge> &edges) {
        nodeMap.clear();
        edgeList.clear();
        nodeIndex.clear();

        // Add nodes to nodeMap
        for (const auto &node : nodes) {
            if (!node.get_del()) {
                size_t index = nodeMap.size();
                nodeMap.push_back({node.get_name(), node.score});
                nodeIndex[&node] = index;
            }
        }

        // Add edges to edgeList
        for (const auto &edge : edges) {
            if (!edge.get_del()) {
                size_t from = nodeIndex[edge.node(0)];
                size_t to = nodeIndex[edge.node(1)];
                edgeList.push_back({from, to});
            }
        }
    }

    // Convert from MapData to deques using provided lambda functions
    void toDeques(std::deque<Node> &nodes, std::deque<Edge> &edges, 
                  std::function<void(const sf::Vector2f&, const std::string&, const int&)> new_node,
                  std::function<void(Node&, Node&)> new_edge) const {
        nodes.clear();
        edges.clear();
        std::unordered_map<size_t, Node*> indexToNode;

        // Create nodes
        for (const auto &mapNode : nodeMap) {
            new_node(sf::Vector2f(rand() % 10 + 500, rand() % 10 + 500), mapNode.name, mapNode.score); // Position will be set later
            Node &node = nodes.back();
            indexToNode[indexToNode.size()] = &node;
        }

        // Create edges
        for (const auto &mapEdge : edgeList) {
            Node &fromNode = *indexToNode.at(mapEdge.from);
            Node &toNode = *indexToNode.at(mapEdge.to);
            new_edge(fromNode, toNode);
        }
    }

    // Read from file
    void readFromFile(const std::string &filename) {
        std::ifstream infile(filename);
        if (!infile) {
            throw std::runtime_error("Cannot open file");
        }

        nodeMap.clear();
        edgeList.clear();

        size_t nodeCount, edgeCount;
        infile >> nodeCount >> edgeCount;

        nodeMap.resize(nodeCount);
        for (size_t i = 0; i < nodeCount; ++i) {
            infile >> nodeMap[i].name >> nodeMap[i].score;
        }

        edgeList.resize(edgeCount);
        for (size_t i = 0; i < edgeCount; ++i) {
            infile >> edgeList[i].from >> edgeList[i].to;
        }

        infile.close();
    }

    // Write to file
    void writeToFile(const std::string &filename) const {
        std::ofstream outfile(filename);
        if (!outfile) {
            throw std::runtime_error("Cannot open file");
        }

        outfile << nodeMap.size() << " " << edgeList.size() << "\n";
        for (const auto &mapNode : nodeMap) {
            outfile << mapNode.name << " " << mapNode.score << "\n";
        }

        for (const auto &mapEdge : edgeList) {
            outfile << mapEdge.from << " " << mapEdge.to << "\n";
        }

        outfile.close();
    }

    // Topological sort
    bool topologicalSort(std::vector<std::string> &result) const {
        std::unordered_map<size_t, std::vector<size_t>> adjList;
        std::vector<int> inDegree(nodeMap.size(), 0);

        for (const auto &edge : edgeList) {
            adjList[edge.from].push_back(edge.to);
            ++inDegree[edge.to];
        }

        std::vector<size_t> zeroInDegreeNodes;
        for (size_t i = 0; i < nodeMap.size(); ++i) {
            if (inDegree[i] == 0) {
                zeroInDegreeNodes.push_back(i);
            }
        }

        std::random_device rd;
        std::mt19937 g(rd());

        std::vector<size_t> topoOrder;
        while (!zeroInDegreeNodes.empty()) {
            std::shuffle(zeroInDegreeNodes.begin(), zeroInDegreeNodes.end(), g);
            size_t node = zeroInDegreeNodes.back();
            zeroInDegreeNodes.pop_back();
            topoOrder.push_back(node);

            for (size_t neighbor : adjList[node]) {
                if (--inDegree[neighbor] == 0) {
                    zeroInDegreeNodes.push_back(neighbor);
                }
            }
        }

        if (topoOrder.size() != nodeMap.size()) {
            // throw std::runtime_error("Graph has at least one cycle");
            return false;
        }

        result.clear();
        for (size_t index : topoOrder) {
            result.push_back(nodeMap[index].name);
        }

        return true;
    }

    const std::vector<MapNode>& getNodeMap() const { return nodeMap; }
    const std::vector<MapEdge>& getEdgeList() const { return edgeList; }

private:
    std::vector<MapNode> nodeMap;
    std::vector<MapEdge> edgeList;
    std::unordered_map<const Node*, size_t> nodeIndex;
};

3.2 文件操作
#ifndef FILE_SELECTOR_HPP
#define FILE_SELECTOR_HPP

#include <string>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#else
#include <gtk/gtk.h>
#endif

enum class FileDialogType {
    Open,
    Save
};

class FileSelector {
public:
    static std::string select_file(FileDialogType type, const std::string &title = "Select File") {
        std::string filename;
        
#ifdef _WIN32
        OPENFILENAME ofn;
        char szFile[260] = {0};
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrTitle = title.c_str();
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (type == FileDialogType::Open) {
            if (GetOpenFileName(&ofn) == TRUE) {
                filename = ofn.lpstrFile;
            }
        } else {
            if (GetSaveFileName(&ofn) == TRUE) {
                filename = ofn.lpstrFile;
            }
        }
#else
        filename = "test.txt";
        return filename;

        gtk_init(0, nullptr);
        GtkWidget *dialog;
        GtkFileChooserAction action = (type == FileDialogType::Open) 
                                      ? GTK_FILE_CHOOSER_ACTION_OPEN 
                                      : GTK_FILE_CHOOSER_ACTION_SAVE;
        dialog = gtk_file_chooser_dialog_new(title.c_str(),
                                             NULL,
                                             action,
                                             ("_Cancel"), GTK_RESPONSE_CANCEL,
                                             ("_Open"), GTK_RESPONSE_ACCEPT,
                                             NULL);
        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
            char *file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            filename = file;
            g_free(file);
        }
        gtk_widget_destroy(dialog);
#endif

        return filename;
    }
};

#endif // FILE_SELECTOR_HPP

3.3 渲染循环

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
    
4 测试数据
	见演示
	
