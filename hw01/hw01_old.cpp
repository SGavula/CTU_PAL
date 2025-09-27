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
            parent.resize(n+1);
            rank.resize(n+1, 0);

            for(int i = 0; i <= n; i++) {
                parent[i] = i;
            }
        }

        int find(int node_id) {
            if(parent[node_id] == node_id) {
                return node_id;
            }

            parent[node_id] = find(parent[node_id]);
            return parent[node_id]; 
        }

        void unionSets(int n1, int n2) {
            int n1_par = find(n1);
            int n2_par = find(n2);

            if(n1_par != n2_par) {
                if(rank[n1_par] > rank[n2_par]) {
                    parent[n2_par] = n1_par;
                    rank[n1_par] = rank[n1_par] + rank[n2_par] + 1;
                } else {
                    parent[n1_par] = n2_par;
                    rank[n2_par] = rank[n2_par] + rank[n1_par] + 1;
                }
            }
        }
};

void load_input_edges(std::vector<std::tuple<int, int, int>>& edges, int R) {
    for(int i = 0; i < R; i++) {
        int n1, n2, cost;
        std::cin >> n1 >> n2 >> cost;
        edges.emplace_back(n1, n2, cost);
    }
}

void print_edges(const std::vector<std::tuple<int, int, int>>& edges) {
    for(auto [n1, n2, cost] : edges) {
        std::cout << "(" << n1 << ", " << n2 << ", " << cost << ")" << std::endl;
    }
}

void create_adjacency_list(
    const std::vector<std::tuple<int, int, int>>& edges,
    std::vector<std::vector<int>>& adjacency_list
) {
    for(auto [n1, n2, cost] : edges) {
        adjacency_list[n1].push_back(n2);
        adjacency_list[n2].push_back(n1);
    }
}
 
void print_adjacency_list(const std::vector<std::vector<int>>& adjacency_list) {
    for (int i = 1; i < adjacency_list.size(); i++) {
        std::cout << i << ": ";
        for (int neighbor : adjacency_list[i]) {
            std::cout << neighbor << " ";
        }
        std::cout << "\n";
    }
}

void run_bfs_search(std::vector<int>& district_of, int T, int D, const std::vector<std::vector<int>>& adjacency_list) {
    std::queue<std::pair<int, int>> queue;
    std::vector<bool> visited(T + 1, false);
    
    // Add district nodes to queue
    for(int i = 1; i < D+1; i++) {
        queue.emplace(i, i);
        district_of[i] = i;
        visited[i] = true;
    }

    while(!queue.empty()) {
        // Get first element on the top of queue
        std::pair<int, int> top_value = queue.front();
        queue.pop();
        int node = top_value.first;
        int district = top_value.second;

        const auto& neighbours = adjacency_list[node];
        
        for(auto neighbour : neighbours) {
            // Check if neighbour is visited
            if(visited[neighbour] == true) {
                continue;
            }

            // Add to queue
            queue.push(std::make_pair(neighbour, district));
            // Assign id of district node to neighbour
            district_of[neighbour] = district;
            // Set as visited
            visited[neighbour] = true;
        }
    }
}

void print_district_of(const std::vector<int>& district_of) {
    std::cout << "DISTRICT NODES" << std::endl;
    for(int i = 1; i < district_of.size(); i++) {
        std::cout << "For node: " << i << " district node is: " << district_of[i] << std::endl;
    }
}

int compute_min_weight(std::vector<std::tuple<int, int, int>>& edges, const std::vector<int>& district_of, int T, int D) {
    int min_weight = 0;
    DisjointSet ds_nodes_in_district(T);
    DisjointSet ds_district_nodes(D);

    // Sort edges
    std::sort(edges.begin(), edges.end(),
        [](const auto& a, const auto& b) {
            return std::get<2>(a) < std::get<2>(b);
        }
    );

    // Use Kruskal's algorithm for finding minimum spanning tree for both nodes in one district graphs and between district graphs
    for(auto [n1, n2, cost] : edges) {
        int n1_district = district_of[n1];
        int n2_district = district_of[n2];

        if(n1_district != n2_district) {
            // Nodes are in different district graphs
            if(ds_district_nodes.find(n1_district) != ds_district_nodes.find(n2_district)) {
                // Find edge that is not in graph
                ds_district_nodes.unionSets(n1_district, n2_district);
                min_weight = min_weight + cost;
            }

        } else {
            // Nodes are in the same district graph
            if(ds_nodes_in_district.find(n1) != ds_nodes_in_district.find(n2)) {
                // Find edge that is not in graph
                ds_nodes_in_district.unionSets(n1, n2);
                min_weight = min_weight + cost;
            }
        }
    }

    return min_weight;
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

    std::vector<std::vector<int>> adjacency_list(T+1);

    // Load edges from input to the vector
    load_input_edges(edges, R);
    // print_edges(edges);

    // Convert graph from inputs triplets to adjacency list for BFS
    create_adjacency_list(edges, adjacency_list);
    // print_adjacency_list(adjacency_list);

    // BFS
    std::vector<int> district_of(T + 1);
    run_bfs_search(district_of, T, D, adjacency_list);
    // print_district_of(district_of);

    // MSP
    int min_weight = compute_min_weight(edges, district_of, T, D);
    
    std::cout << min_weight << std::endl; 
    
    return 0;
}