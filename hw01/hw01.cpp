#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>

class DisjointSet {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    DisjointSet(int n) {
        parent.resize(n + 1);
        rank.resize(n + 1, 0);

        for (int i = 0; i <= n; i++) {
            parent[i] = i;
        }
    }

    int find(int nodeId) {
        if (parent[nodeId] == nodeId) {
            return nodeId;
        }

        parent[nodeId] = find(parent[nodeId]);
        return parent[nodeId]; 
    }

    void unionSets(int n1, int n2) {
        int n1Par = find(n1);
        int n2Par = find(n2);

        if (n1Par != n2Par) {
            if (rank[n1Par] > rank[n2Par]) {
                parent[n2Par] = n1Par;
                rank[n1Par] = rank[n1Par] + rank[n2Par] + 1;
            } else {
                parent[n1Par] = n2Par;
                rank[n2Par] = rank[n2Par] + rank[n1Par] + 1;
            }
        }
    }
};

void loadInputEdges(std::vector<std::tuple<int, int, int>>& edges, int R) {
    for (int i = 0; i < R; i++) {
        int n1, n2, cost;
        std::cin >> n1 >> n2 >> cost;
        edges.emplace_back(n1, n2, cost);
    }
}

void printEdges(const std::vector<std::tuple<int, int, int>>& edges) {
    for (size_t i = 0; i < edges.size(); i++) {
        int n1, n2, cost;
        std::tie(n1, n2, cost) = edges[i];
        std::cout << "(" << n1 << ", " << n2 << ", " << cost << ")" << std::endl;
    }
}

void createAdjacencyList(
    const std::vector<std::tuple<int, int, int>>& edges,
    std::vector<std::vector<int>>& adjacencyList
) {
    for (size_t i = 0; i < edges.size(); i++) {
        int n1, n2, cost;
        std::tie(n1, n2, cost) = edges[i];
        adjacencyList[n1].push_back(n2);
        adjacencyList[n2].push_back(n1);
    }
}
 
void printAdjacencyList(const std::vector<std::vector<int>>& adjacencyList) {
    for (size_t i = 1; i < adjacencyList.size(); i++) {
        std::cout << i << ": ";
        for (size_t j = 0; j < adjacencyList[i].size(); j++) {
            std::cout << adjacencyList[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void runBfsSearch(std::vector<int>& districtOf, int T, int D, const std::vector<std::vector<int>>& adjacencyList) {
    std::queue<std::pair<int, int>> queue;
    std::vector<bool> visited(T + 1, false);
    
    // Add district nodes to queue
    for (int i = 1; i < D + 1; i++) {
        queue.emplace(i, i);
        districtOf[i] = i;
        visited[i] = true;
    }

    while (!queue.empty()) {
        // Get first element on the top of queue
        std::pair<int, int> topValue = queue.front();
        queue.pop();
        int node = topValue.first;
        int district = topValue.second;

        const std::vector<int>& neighbours = adjacencyList[node];
        
        for (size_t j = 0; j < neighbours.size(); j++) {
            int neighbour = neighbours[j];
            // Check if neighbour is visited
            if (visited[neighbour] == true) {
                continue;
            }

            // Add to queue
            queue.push(std::make_pair(neighbour, district));
            // Assign id of district node to neighbour
            districtOf[neighbour] = district;
            // Set as visited
            visited[neighbour] = true;
        }
    }
}

void printDistrictOf(const std::vector<int>& districtOf) {
    std::cout << "DISTRICT NODES" << std::endl;
    for (size_t i = 1; i < districtOf.size(); i++) {
        std::cout << "For node: " << i << " district node is: " << districtOf[i] << std::endl;
    }
}

int computeMinWeight(std::vector<std::tuple<int, int, int>>& edges, const std::vector<int>& districtOf, int T, int D) {
    int minWeight = 0;
    DisjointSet dsNodesInDistrict(T);
    DisjointSet dsDistrictNodes(D);

    // Sort edges
    std::sort(edges.begin(), edges.end(),
        [](const std::tuple<int,int,int>& a, const std::tuple<int,int,int>& b) {
            return std::get<2>(a) < std::get<2>(b);
        }
    );

    // Use Kruskal's algorithm
    for (size_t i = 0; i < edges.size(); i++) {
        int n1, n2, cost;
        std::tie(n1, n2, cost) = edges[i];

        int n1District = districtOf[n1];
        int n2District = districtOf[n2];

        if (n1District != n2District) {
            // Nodes are in different district graphs
            if (dsDistrictNodes.find(n1District) != dsDistrictNodes.find(n2District)) {
                dsDistrictNodes.unionSets(n1District, n2District);
                minWeight = minWeight + cost;
            }

        } else {
            // Nodes are in the same district graph
            if (dsNodesInDistrict.find(n1) != dsNodesInDistrict.find(n2)) {
                dsNodesInDistrict.unionSets(n1, n2);
                minWeight = minWeight + cost;
            }
        }
    }

    return minWeight;
}

/* MAIN */
int main() {
    // T - number of nodes (cities)
    // D - number of district nodes (district cities)
    // R - number of edges
    int T, D, R;

    // Load first line of input data
    std::cin >> T >> D >> R;

    // Define vector for storing edges
    std::vector<std::tuple<int, int, int>> edges;
    edges.reserve(R);

    std::vector<std::vector<int>> adjacencyList(T + 1);

    // Load edges from input to the vector
    loadInputEdges(edges, R);

    // Convert graph from inputs triplets to adjacency list for BFS
    createAdjacencyList(edges, adjacencyList);

    // BFS
    std::vector<int> districtOf(T + 1);
    runBfsSearch(districtOf, T, D, adjacencyList);

    // MST
    int minWeight = computeMinWeight(edges, districtOf, T, D);
    
    std::cout << minWeight << std::endl; 
    
    return 0;
}
