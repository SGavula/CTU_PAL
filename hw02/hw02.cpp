#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include <stack>
#include <queue>

void loadStarts(std::vector<int>& starts, int A) {
    for(size_t i = 0; i < A; i++) {
        std::cin >> starts[i];
    }
}

void loadInputEdges(std::vector<std::tuple<int, int>>& edges, int M) {
    for (size_t i = 0; i < M; i++) {
        int n1, n2;
        std::cin >> n1 >> n2;
        edges.emplace_back(n1, n2);
    }
}

void printEdges(const std::vector<std::tuple<int, int>>& edges) {
    for (size_t i = 0; i < edges.size(); i++) {
        int n1, n2;
        std::tie(n1, n2) = edges[i];
        std::cout << n1 << ", " << n2 << std::endl;
    }
}

void createAdjacencyList(std::vector<std::vector<int>>& adjacencyList, std::vector<std::vector<int>>& adjacencyListReverse, const std::vector<std::tuple<int, int>>& edges, const int M) {
    for(size_t i = 0; i < M; i++) {
        int n1, n2;
        std::tie(n1, n2) = edges[i];
        // Save nodes in the correct direction
        adjacencyList[n1].push_back(n2);
        // Save nodes in the reverse direction to create reversed graph
        adjacencyListReverse[n2].push_back(n1);
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

void firstDFS(const int n, const std::vector<std::vector<int>>& adjacencyList, std::vector<bool>& visited, std::stack<int>& stack) {
    visited[n] = true;

    // Get neighbours of current node
    std::vector<int> neighbours = adjacencyList[n];
    for(size_t i = 0; i < neighbours.size(); i++) {
        int neighbour = neighbours[i];
        if(visited[neighbour] == false) {
            // Neighbour is unvisited
            firstDFS(neighbour, adjacencyList, visited, stack);
        }
    }

    // Close current node 
    stack.emplace(n);
}

void secondDFS(const int n, const std::vector<std::vector<int>>& adjacencyListReverse, std::vector<bool>& visited, std::vector<int>& SCC, int numSCC) {
    visited[n] = true;
    SCC[n] = numSCC;

    // Get neighbours of current node
    std::vector<int> neighbours = adjacencyListReverse[n];
    for(size_t i = 0; i < neighbours.size(); i++) {
        int neighbour = neighbours[i];
        if(visited[neighbour] == false) {
            // Neighbour is unvisited
            secondDFS(neighbour, adjacencyListReverse, visited, SCC, numSCC);
        }
    }
}

void printStack(std::stack<int>& stack) {
    // make a copy, so we don't destroy original
    std::stack<int> temp = stack;

    while (!temp.empty()) {
        std::cout << temp.top() << " ";
        temp.pop();
    }

    std::cout << std::endl;
}

void kosarajuSharirAlgorithm(const std::vector<std::vector<int>>& adjacencyList, const std::vector<std::vector<int>>& adjacencyListReverse, const int N, std::vector<int>& SCC, int& numSCC) {
    std::stack<int> stack;
    std::vector<bool> visited(N + 1, false);
    
    for(size_t i = 1; i <= N; i++) {
        if(visited[i] == false) {
            firstDFS(i, adjacencyList, visited, stack);
        }
    }

    visited.assign(N + 1, false);

    while(!stack.empty()) {
        int node = stack.top();
        stack.pop();

        if(visited[node] == true) {
            continue;
        }
        numSCC++;
        secondDFS(node, adjacencyListReverse, visited, SCC, numSCC);
        SCC[node] = numSCC;
    }
}

void createAdjacencyListSCC(const std::vector<std::vector<int>>& adjacencyList, const std::vector<int>& SCC, std::vector<std::vector<int>>& adjacencyListSCC, std::vector<std::vector<int>>& adjacencyListSCCReverse, const int N, const int numSCC) {
    std::vector<std::set<int>> tempAdj(numSCC + 1);
    std::vector<std::set<int>> tempRev(numSCC + 1);

    for (int i = 1; i <= N; i++) {
        for (int neighbour : adjacencyList[i]) {
            int n1Comp = SCC[i];
            int n2Comp = SCC[neighbour];
            if (n1Comp != n2Comp) {
                tempAdj[n1Comp].insert(n2Comp);
                tempRev[n2Comp].insert(n1Comp);
            }
        }
    }

    // Convert sets to vectors
    for (int i = 1; i <= numSCC; i++) {
        adjacencyListSCC[i].assign(tempAdj[i].begin(), tempAdj[i].end());
        adjacencyListSCCReverse[i].assign(tempRev[i].begin(), tempRev[i].end());
    }
}

void getReachableCompsFromStarts(const std::vector<int>& starts, const std::vector<int>& SCC, const std::vector<std::vector<int>>& adjacencyListSCC, std::vector<int>& reachableFromStarts, const int& numSCC, const int& A) {
    // For start node run bfs to get what components are reachable from that start
    for(size_t i = 0; i < A; i++) {
        int startId = starts[i];
        int startIdSCC = SCC[startId];
        std::vector<bool> visited(numSCC + 1, false);
        std::queue<int> queue;

        queue.push(startIdSCC);
        visited[startIdSCC] = true;
        
        while(!queue.empty()) {
            int nodeSCC = queue.front();
            queue.pop();
                
            reachableFromStarts[nodeSCC]++;
            
            std::vector<int> neighbours = adjacencyListSCC[nodeSCC];
            for(size_t i = 0; i < neighbours.size(); i++) {
                int neighbour = neighbours[i];
                if(visited[neighbour] == false) {
                    visited[neighbour] = true;
                    queue.push(neighbour);
                }
            }
        }
    }
}

void computeSizesOfSCC(const int& N, const std::vector<int>& SCC, std::vector<int>& sizesOfSCC) {
    for(size_t i = 1; i <= N; i++) {
        int idSCC = SCC[i];
        sizesOfSCC[idSCC]++;
    }
}

void computeMaxPath(std::vector<int>& maxPathDP, const int& P, const std::vector<int>& SCC, const std::vector<int>& sizesOfSCC, const int& numSCC, const std::vector<std::vector<int>>& adjacencyListSCCReverse, const std::vector<int>& reachableFromStarts, const int& A) {
    // Set value for goal node
    maxPathDP[SCC[P]] = sizesOfSCC[SCC[P]];
    
    // For each node 
    for(size_t i = numSCC; i > 0; i--) {
        // Check if node has access to the goal node, if not continue to another node
        if(maxPathDP[i] == 0) {
            continue;
        }

        std::vector<int> neighbours = adjacencyListSCCReverse[i];
        for(size_t j = 0; j < neighbours.size(); j++) {
            int neighbour = neighbours[j];
            // Check if node is reachable by all starting nodes
            if(reachableFromStarts[neighbour] == A) {
                // Reachable by all starting nodes
                maxPathDP[neighbour] = std::max(maxPathDP[neighbour], maxPathDP[i] + sizesOfSCC[neighbour]);
            }
            else {
                // Not reachable by all starting nodes
                maxPathDP[neighbour] = std::max(maxPathDP[neighbour], maxPathDP[i]);
            }
        }
    }
}

void selectMaxValue(int& maxValue, const int& A, const std::vector<int>& starts, const std::vector<int>& SCC, const std::vector<int>& maxPathDP) {
    for(size_t i = 0; i < A; i++) {
        int startId = starts[i];
        int startIdSCC = SCC[startId];
        maxValue = std::max(maxValue, maxPathDP[startIdSCC]);
    }
}

void printVector(std::vector<int> vec) {
    for(size_t i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

/* MAIN */
int main() {
    // N - number of sites
    // M - number of direct routes
    // A - number of archeologist 
    // P - the primary excavation site
    int N, M, A, P;
    
    // Load first line of input data
    std::cin >> N >> M >> A >> P;

    std::vector<int> starts(A);
    std::vector<std::tuple<int, int>> edges;
    edges.reserve(M);
    
    // Load ids of nodes where archeologist start 
    loadStarts(starts, A);

    // Load edges
    loadInputEdges(edges, M);

    // Transform edges to adjacency list
    std::vector<std::vector<int>> adjacencyList(N + 1);
    std::vector<std::vector<int>> adjacencyListReverse(N + 1);
    createAdjacencyList(adjacencyList, adjacencyListReverse, edges, M);

    // Kosaraju-Sharir Algorithm for finding SCC
    int numSCC = 0;
    std::vector<int> SCC(N + 1);
    kosarajuSharirAlgorithm(adjacencyList, adjacencyListReverse, N, SCC, numSCC);
    
    // Create condensed graph 
    std::vector<std::vector<int>> adjacencyListSCC(numSCC + 1);
    std::vector<std::vector<int>> adjacencyListSCCReverse(numSCC + 1);
    createAdjacencyListSCC(adjacencyList, SCC, adjacencyListSCC, adjacencyListSCCReverse, N, numSCC);

    // Compute for each component of SCC graph is reachable by all nodes from start
    std::vector<int> reachableFromStarts(numSCC + 1);
    getReachableCompsFromStarts(starts, SCC, adjacencyListSCC, reachableFromStarts, numSCC, A);
    
    // Compute size of each strongly connected component in condensed graph
    std::vector<int> sizesOfSCC(numSCC + 1);
    computeSizesOfSCC(N, SCC, sizesOfSCC);
    
    // Compute the path from goal to starts using dynamic programming table
    std::vector<int> maxPathDP(numSCC + 1);
    computeMaxPath(maxPathDP, P, SCC, sizesOfSCC, numSCC, adjacencyListSCCReverse, reachableFromStarts, A);

    // Select max value from all starts nodes, nodes that are in SCC components, using DP table
    int maxValue = 0;
    selectMaxValue(maxValue, A, starts, SCC, maxPathDP);

    std::cout << maxValue << std::endl;

    return 0;
}