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
