 报告

 1. 概要设计

 1.1 总体设计

本程序旨在从文件或键盘输入构建一个带非负权重的无向图，然后使用Prim算法计算最小生成树（MST），并将结果输出到屏幕上。程序主要实现了以下几个功能：

1. 接受用户输入以构建图。
2. 使用Prim算法计算最小生成树。
3. 将最小生成树输出到屏幕。

 1.2 实现内容

- 图的输入：从用户输入的顶点数、边数及每条边的信息（起点、终点和权重）来构建图。
- 图的存储：使用邻接矩阵表示图。
- Prim算法：通过选择不在MST中的最小权重边逐步构建MST。
- 结果输出：将MST的边和对应的权重输出到屏幕。

 1.3图的存储方式

使用二维向量`vector<vector<int>>`作为邻接矩阵来存储图，其中`graph[i][j]`表示从顶点`i`到顶点`j`的边的权重。如果没有直接连接的边，则对应的值为0。


vector<vector<int>> graph;
graph.resize(V, vector<int>(V, 0));


这个表示方法在处理稠密图时非常有效，因为它能够快速地进行边权重的查找和更新。

1.4 运行结果

Enter number of vertices: 6
Enter number of edges: 10
Enter edges (source destination weight):
0 1 6
0 2 1
0 3 5
1 2 5
2 3 5
1 4 3
2 4 6
2 5 4
4 5 6
3 5 2
Edge    Weight
2 - 1   5 
0 - 2   1 
5 - 3   2 
1 - 4   3 
2 - 5   4 

 2. 主要代码内容及其解释

以下是程序的主要代码及其解释：

 头文件和常量定义


include <iostream>
include <vector>
include <climits>
include <fstream>
include <sstream>

using namespace std;

const int INF = INT_MAX;


- 头文件：包括标准输入输出库、向量库、整数限制库、文件流库和字符串流库。
- 常量定义：定义了一个表示无穷大的常量`INF`，用于初始化和比较权重。

 边的结构和辅助函数


struct Edge {
    int src, dest, weight;
};

void addEdge(vector<vector<int>>& graph, int u, int v, int weight) {
    graph[u][v] = weight;
    graph[v][u] = weight;
}


- 结构定义：定义了一个`Edge`结构，用于存储每条边的起点、终点和权重。
- 辅助函数：`addEdge`函数用于向邻接矩阵中添加边。

 查找最小键值的顶点


int minKey(vector<int>& key, vector<bool>& mstSet, int V) {
    int min = INF, min_index;

    for (int v = 0; v < V; v++)
        if (!mstSet[v] && key[v] < min)
            min = key[v], min_index = v;

    return min_index;
}


- 函数作用：`minKey`函数用于查找不在MST中且具有最小键值的顶点。

 打印最小生成树


void printMST(vector<int>& parent, vector<vector<int>>& graph, int V) {
    cout << "Edge \tWeight\n";
    for (int i = 1; i < V; i++)
        cout << parent[i] << " - " << i << " \t" << graph[i][parent[i]] << " \n";
}


- 函数作用：`printMST`函数用于输出最小生成树的边及其权重。

 Prim算法实现


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


- 初始化：初始化`parent`数组用于存储MST，`key`数组用于选择最小权重边，`mstSet`数组用于标记已包括在MST中的顶点。
- 选择顶点：每次从`key`数组中选择不在MST中且键值最小的顶点，并将其加入MST。
- 更新键值：更新所有与新加入顶点相邻的顶点的键值和父节点。

 主函数


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


- 输入：从用户输入获取图的顶点数、边数以及每条边的详细信息，使用`addEdge`函数将边添加到邻接矩阵中。
- 计算MST：调用`primMST`函数计算最小生成树。
- 输出结果：`primMST`函数内部调用`printMST`函数将结果输出到屏幕。

 结论

本程序通过详细的设计和实现，实现了从用户输入构建无向图、使用Prim算法计算最小生成树并输出结果的功能。图的存储采用邻接矩阵，便于稠密图的处理。整个程序结构清晰，函数职责明确，能够有效地实现预期功能。
