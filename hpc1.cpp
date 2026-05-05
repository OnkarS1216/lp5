#include <iostream>     // for input-output
#include <vector>       // for adjacency list
#include <queue>        // for BFS queue
#include <omp.h>        // for OpenMP parallelization
#include <algorithm>    // for fill()
using namespace std;

// Adjacency list representation of graph
vector<int> graph[100];

// Visited array to track visited nodes
bool visited[100];


// 🔵 Parallel BFS Function
void bfs(int start) {
    queue<int> q;              // queue for BFS
    visited[start] = true;     // mark starting node as visited
    q.push(start);             // push start node into queue

    cout << "BFS Traversal: ";

    // Loop until queue becomes empty
    while (!q.empty()) {
        int node = q.front();  // get front element
        q.pop();               // remove it from queue

        cout << node << " ";   // print node

        // Parallel loop to process all neighbors
        #pragma omp parallel for
        for (int i = 0; i < graph[node].size(); i++) {
            int neighbor = graph[node][i];  // get neighbor

            // If neighbor is not visited
            if (!visited[neighbor]) {

                // Critical section to avoid race condition
                #pragma omp critical
                {
                    if (!visited[neighbor]) {   // check again inside critical
                        visited[neighbor] = true; // mark visited
                        q.push(neighbor);        // push into queue
                    }
                }
            }
        }
    }
    cout << endl;
}


// 🔴 Parallel DFS Function
void dfs(int node) {
    visited[node] = true;      // mark node as visited
    cout << node << " ";       // print node

    // Parallel loop for neighbors
    #pragma omp parallel for
    for (int i = 0; i < graph[node].size(); i++) {
        int neighbor = graph[node][i];  // get neighbor

        // If not visited, call DFS recursively
        if (!visited[neighbor]) {
            dfs(neighbor);   // recursive DFS call
        }
    }
}


// 🔹 Main Function
int main() {
    int n, e;

    cout << "Enter number of nodes: ";
    cin >> n;   // input number of nodes

    cout << "Enter number of edges: ";
    cin >> e;   // input number of edges

    cout << "Enter edges (u v):\n";

    // Input edges
    for (int i = 0; i < e; i++) {
        int u, v;
        cin >> u >> v;

        // Since graph is undirected, add both ways
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    int startNode;
    cout << "Enter starting node: ";
    cin >> startNode;   // input starting node

    // Initialize visited array to false
    fill(visited, visited + n, false);

    bfs(startNode);     // call BFS

    // Reset visited array for DFS
    fill(visited, visited + n, false);

    cout << "DFS Traversal: ";
    dfs(startNode);     // call DFS

    cout << endl;

    return 0;
}