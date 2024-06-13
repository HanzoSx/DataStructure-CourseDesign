#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <string>
#include <algorithm>

using namespace std;

// 边的结构体，包含通过时间和费用
struct Edge {
    int time;
    int cost;

    Edge() : time(0), cost(0) {} // 默认构造函数，初始化 time 和 cost
    Edge(int t, int c) : time(t), cost(c) {}
};

// 图的类
class Graph {
private:
    unordered_map<string, unordered_map<string, Edge>> adjList; // 邻接表存储图

public:
    // 添加节点
    void addNode(const string& node) {
        if (adjList.find(node) == adjList.end()) {
            adjList[node] = unordered_map<string, Edge>();
        }
    }

    // 添加边
    void addEdge(const string& from, const string& to, int time, int cost) {
        addNode(from);
        addNode(to);
        adjList[from][to] = Edge(time, cost);
    }

    // 删除节点及其相关边
    void removeNode(const string& node) {
        adjList.erase(node);
        for (auto& pair : adjList) {
            pair.second.erase(node);
        }
    }

    // 删除边
    void removeEdge(const string& from, const string& to) {
        if (adjList.find(from) != adjList.end() && adjList[from].find(to) != adjList[from].end()) {
            adjList[from].erase(to);
        }
    }

    // 修改边的通过时间
    void updateTime(const string& from, const string& to, int newTime) {
        if (adjList.find(from) != adjList.end() && adjList[from].find(to) != adjList[from].end()) {
            adjList[from][to].time = newTime;
        }
    }

    // 修改边的过路费用
    void updateCost(const string& from, const string& to, int newCost) {
        if (adjList.find(from) != adjList.end() && adjList[from].find(to) != adjList[from].end()) {
            adjList[from][to].cost = newCost;
        }
    }

    // Dijkstra算法求最短路径
    pair<vector<string>, int> shortestPath(const string& start, const string& end, bool byTime) {
        unordered_map<string, int> dist; // 记录最短距离或最少费用
        unordered_map<string, pair<string, Edge>> parent; // 记录路径

        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [currentDist, current] = pq.top();
            pq.pop();

            if (current == end) {
                vector<string> path;
                int total = currentDist;
                while (current != start) {
                    path.push_back(current);
                    auto& [prev, edge] = parent[current];
                    total += (byTime ? edge.time : edge.cost);
                    current = prev;
                }
                path.push_back(start);
                reverse(path.begin(), path.end());
                return {path, total};
            }

            if (currentDist > dist[current]) continue;

            for (auto& [neighbor, edge] : adjList[current]) {
                int newDist = currentDist + (byTime ? edge.time : edge.cost);
                if (!dist.count(neighbor) || newDist < dist[neighbor]) {
                    dist[neighbor] = newDist;
                    parent[neighbor] = {current, edge};
                    pq.push({newDist, neighbor});
                }
            }
        }

        return {{}, numeric_limits<int>::max()};
    }

    // 保存图到文件
    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (auto& [from, neighbors] : adjList) {
                for (auto& [to, edge] : neighbors) {
                    outFile << from << " " << to << " " << edge.time << " " << edge.cost << endl;
                }
            }
            outFile.close();
            cout << "Graph saved to " << filename << endl;
        } else {
            cerr << "Unable to open file for saving." << endl;
        }
    }

    // 从文件读取图
    void readFromFile(const string& filename) {
        adjList.clear(); // 清空当前图

        ifstream inFile(filename);
        if (inFile.is_open()) {
            string from, to;
            int time, cost;
            while (inFile >> from >> to >> time >> cost) {
                addEdge(from, to, time, cost);
            }
            inFile.close();
            cout << "Graph loaded from " << filename << endl;
        } else {
            cerr << "Unable to open file for reading." << endl;
        }
    }
};

// 打印菜单
void printMenu() {
    cout << "\n===== Menu =====" << endl;
    cout << "1. Add node" << endl;
    cout << "2. Add edge" << endl;
    cout << "3. Remove node" << endl;
    cout << "4. Remove edge" << endl;
    cout << "5. Update edge time" << endl;
    cout << "6. Update edge cost" << endl;
    cout << "7. Find shortest path by time" << endl;
    cout << "8. Find shortest path by cost" << endl;
    cout << "9. Save graph to file" << endl;
    cout << "10. Load graph from file" << endl;
    cout << "0. Exit" << endl;
    cout << "================" << endl;
    cout << "Enter your choice: ";
}

int main() {
    Graph trafficNetwork;
    int choice;

    do {
        printMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除输入缓冲区

        switch (choice) {
            case 1: {
                string node;
                cout << "Enter node name: ";
                getline(cin, node);
                trafficNetwork.addNode(node);
                cout << "Node " << node << " added." << endl;
                break;
            }
            case 2: {
                string from, to;
                int time, cost;
                cout << "Enter from node: ";
                getline(cin, from);
                cout << "Enter to node: ";
                getline(cin, to);
                cout << "Enter travel time: ";
                cin >> time;
                cout << "Enter travel cost: ";
                cin >> cost;
                trafficNetwork.addEdge(from, to, time, cost);
                cout << "Edge from " << from << " to " << to << " added." << endl;
                break;
            }
            case 3: {
                string node;
                cout << "Enter node name to remove: ";
                getline(cin, node);
                trafficNetwork.removeNode(node);
                cout << "Node " << node << " removed." << endl;
                break;
            }
            case 4: {
                string from, to;
                cout << "Enter from node: ";
                getline(cin, from);
                cout << "Enter to node: ";
                getline(cin, to);
                trafficNetwork.removeEdge(from, to);
                cout << "Edge from " << from << " to " << to << " removed." << endl;
                break;
            }
            case 5: {
                string from, to;
                int newTime;
                cout << "Enter from node: ";
                getline(cin, from);
                cout << "Enter to node: ";
                getline(cin, to);
                cout << "Enter new travel time: ";
                cin >> newTime;
                trafficNetwork.updateTime(from, to, newTime);
                cout << "Travel time from " << from << " to " << to << " updated to " << newTime << endl;
                break;
            }
            case 6: {
                string from, to;
                int newCost;
                cout << "Enter from node: ";
                getline(cin, from);
                cout << "Enter to node: ";
                getline(cin, to);
                cout << "Enter new travel cost: ";
                cin >> newCost;
                trafficNetwork.updateCost(from, to, newCost);
                cout << "Travel cost from " << from << " to " << to << " updated to $" << newCost << endl;
                break;
            }
            case 7: {
                string start, end;
                cout << "Enter starting node: ";
                getline(cin, start);
                cout << "Enter destination node: ";
                getline(cin, end);
                auto [path, totalTime] = trafficNetwork.shortestPath(start, end, true);
                if (path.empty()) {
                    cout << "No path found." << endl;
                } else {
                    cout << "Shortest time from " << start << " to " << end << ": " << totalTime << " minutes" << endl;
                    cout << "Path: ";
                   
                    for (const auto& node : path) {
                        cout << node << " ";
                    }
                    cout << endl;
                }
                break;
            }
            case 8: {
                string start, end;
                cout << "Enter starting node: ";
                getline(cin, start);
                cout << "Enter destination node: ";
                getline(cin, end);
                auto [path, totalCost] = trafficNetwork.shortestPath(start, end, false);
                if (path.empty()) {
                    cout << "No path found." << endl;
                } else {
                    cout << "Least cost from " << start << " to " << end << ": $" << totalCost << endl;
                    cout << "Path: ";
                    for (const auto& node : path) {
                        cout << node << " ";
                    }
                    cout << endl;
                }
                break;
            }
            case 9: {
                string filename;
                cout << "Enter filename to save graph: ";
                getline(cin, filename);
                trafficNetwork.saveToFile(filename);
                break;
            }
            case 10: {
                string filename;
                cout << "Enter filename to load graph: ";
                getline(cin, filename);
                trafficNetwork.readFromFile(filename);
                break;
            }
            case 0:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a number from the menu." << endl;
        }
    } while (choice != 0);

    return 0;
}
