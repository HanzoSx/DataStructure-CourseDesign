#include <iostream>
#include <vector>
#include <climits>
#include <fstream>
#include <sstream>

using namespace std;

const int INF = INT_MAX;

struct Edge {
    int src, dest, weight;
};

void addEdge(vector<vector<int>>& graph, int u, int v, int weight) {
    graph[u][v] = weight;
    graph[v][u] = weight;
}

int minKey(vector<int>& key, vector<bool>& mstSet, int V) {
    int min = INF, min_index;

    for (int v = 0; v < V; v++)
        if (!mstSet[v] && key[v] < min)
            min = key[v], min_index = v;

    return min_index;
}

void printMST(vector<int>& parent, vector<vector<int>>& graph, int V) {
    cout << "Edge \tWeight\n";
    for (int i = 1; i < V; i++)
        cout << parent[i] << " - " << i << " \t" << graph[i][parent[i]] << " \n";
}

void primMST(vector<vector<int>>& graph, int V) {
    vector<int> parent(V); // Array to store constructed MST
    vector<int> key(V);    // Key values used to pick minimum weight edge in cut
    vector<bool> mstSet(V);  // To represent set of vertices included in MST

    for (int i = 0; i < V; i++)
        key[i] = INF, mstSet[i] = false;

    key[0] = 0;     // Make key 0 so that this vertex is picked as first vertex
    parent[0] = -1; // First node is always root of MST

    for (int count = 0; count < V - 1; count++) {
        int u = minKey(key, mstSet, V);
        mstSet[u] = true;

        for (int v = 0; v < V; v++)
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v];
    }

    printMST(parent, graph, V);
}

int main() {
    int V, E;
    vector<vector<int>> graph;

    cout << "Enter number of vertices: ";
    cin >> V;
    graph.resize(V, vector<int>(V, 0));

    cout << "Enter number of edges: ";
    cin >> E;

    cout << "Enter edges (source destination weight):\n";
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        addEdge(graph, u, v, w);
    }

    primMST(graph, V);

    return 0;
}
