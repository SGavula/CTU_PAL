#include <iostream>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

struct Edge {
    int weight;
    int to;
    int parent;
};

/* MAIN */
int main() {
    int n = 4;  // number of vertices

    vector<vector<pair<int,int>>> adj(n);

    // Undirected graph pair{next, weight}
    adj[0].push_back({1, 1});
    adj[1].push_back({0, 1});

    adj[0].push_back({2, 4});
    adj[2].push_back({0, 4});

    adj[1].push_back({3, 2});
    adj[3].push_back({1, 2});

    adj[2].push_back({3, 3});
    adj[3].push_back({2, 3});

    vector<bool> visited(n, false);

    // Min-heap: (weight, to, parent)
    priority_queue<
        Edge,
        vector<Edge>,
        function<bool(Edge, Edge)>
    > pq([](Edge a, Edge b) {
        return a.weight > b.weight;
    });

    // Start from vertex 0
    pq.push({0, 0, -1});

    int totalWeight = 0;
    vector<pair<int,int>> mst;

    while (!pq.empty() && mst.size() < n - 1) {
        Edge e = pq.top();
        pq.pop();

        if (visited[e.to])
            continue;

        visited[e.to] = true;
        totalWeight += e.weight;

        if (e.parent != -1)
            mst.push_back({e.parent, e.to});

        for (auto [next, w] : adj[e.to]) {
            if (!visited[next]) {
                pq.push({w, next, e.to});
            }
        }
    }

    // Output
    cout << "MST edges:\n";
    for (auto [u, v] : mst) {
        cout << u << " - " << v << "\n";
    }

    cout << "Total weight: " << totalWeight << endl;

    return 0;
}
