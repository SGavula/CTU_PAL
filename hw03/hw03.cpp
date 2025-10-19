#include <iostream>
#include <vector>
#include <tuple>

void loadInputEdges(std::vector<std::tuple<int, int, int>>& edgesCTU, std::vector<std::vector<int>>& cableTypesCTU, const int& M) {
    for (int i = 0; i < M; i++) {
        int n1, n2, type;
        std::cin >> n1 >> n2 >> type;
        edgesCTU.emplace_back(n1, n2, type);
        cableTypesCTU[n1][n2] = type;
        cableTypesCTU[n2][n1] = type;
    }
}

void loadCompSchemesEdges(std::vector<std::vector<std::tuple<int, int, int>>>& compSchemes, const int& S) {
    for(int i = 0; i < S; i++) {
        int D;
        std::cin >> D;
        for (int j = 0; j < D; j++) {
            int n1, n2, type;
            std::cin >> n1 >> n2 >> type;
            compSchemes[i].emplace_back(n1, n2, type);
        }
    }
}

void printEdges(const std::vector<std::tuple<int, int, int>>& edges) {
    for (size_t i = 0; i < edges.size(); i++) {
        int n1, n2, cost;
        std::tie(n1, n2, cost) = edges[i];
        std::cout << "(" << n1 << ", " << n2 << ", " << cost << ")" << std::endl;
    }
}

void printCableTypesCTU(const std::vector<std::vector<int>>& cableTypesCTU) {
    for(int i = 0; i < cableTypesCTU.size(); i++) {
        for(int j = 0; j < cableTypesCTU[i].size(); j++) {
            std::cout << cableTypesCTU[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void createAdjacencyList(
    const std::vector<std::tuple<int, int, int>>& edges,
    std::vector<std::vector<std::tuple<int, int>>>& adjacencyList
) {
    for (size_t i = 0; i < edges.size(); i++) {
        int n1, n2, type;
        std::tie(n1, n2, type) = edges[i];
        adjacencyList[n1].emplace_back(n2, type);
        adjacencyList[n2].emplace_back(n1, type);
    }
}

void printAdjacencyList(const std::vector<std::vector<std::tuple<int, int>>>& adjacencyList) {
    for (size_t i = 0; i < adjacencyList.size(); i++) {
        std::cout << i << ": ";
        for (size_t j = 0; j < adjacencyList[i].size(); j++) {
            std::tuple<int, int> element = adjacencyList[i][j];
            std::cout << "(" << std::get<0>(element) << ", " << std::get<1>(element) << ") ";
        }
        std::cout << "\n";
    }
}

bool checkPossibleMapping(int node, int candidate, std::vector<std::vector<std::tuple<int, int>>> compAdjList, std::vector<std::vector<int>> cableTypesCTU, std::vector<int> assigned) {
    int res = true;
    
    // [(neighbour1, cable), (neighbour2, cable), (neighbour3, cable), ...]
    std::vector<std::tuple<int, int>> nodeNeighbours = compAdjList[node];
    for(int i = 0; i < nodeNeighbours.size(); i++) {
        std::tuple<int, int> neighbourElement = nodeNeighbours[i];
        int neighbour = std::get<0>(neighbourElement);
        int cableType = std::get<1>(neighbourElement);
        int neighbourMapped = assigned[neighbour];

        if(neighbourMapped == -1) {
            // Neighbour was not assigned yet, continue
            continue;
        }

        int mappedCableType = cableTypesCTU[candidate][neighbourMapped];
        if(mappedCableType != cableType) {
            // The mapped node suits the cable condition
            res = false;
        }
    }

    return res;
}

bool checkScheme(int depth, int N, std::vector<std::vector<int>> candidates, std::vector<std::vector<std::tuple<int, int>>> compAdjList, std::vector<std::vector<int>> cableTypesCTU, std::vector<int> assigned, std::vector<bool> used) {
    std::cout << "Running for node: " << depth << std::endl;
    
    if(depth == N) {
        // std::cout << "Super" << std::endl;
        return true;
    }
    
    bool res = false;
    int node = depth;
    std::vector<int> candidatesForNode = candidates[node];
    
    for(int i = 0; i < candidatesForNode.size(); i++) {
        std::cout << "Running for node: " << depth << " I: " << i << std::endl;
        int candidate = candidatesForNode[i];
        if(used[candidate] == true) {
            // If candidate is already assign continue
            continue;
        }

        bool isMapPossible = checkPossibleMapping(node, candidate, compAdjList, cableTypesCTU, assigned);

        if(isMapPossible) {
            // std::cout << "Map is possible" << std::endl;
            std::cout << "Node before assign: " << depth << std::endl;
            for(int k = 0; k < assigned.size(); k++) {
                std::cout << assigned[k] << " ";
            }
            std::cout << std::endl;
            // Assigned mapping to node
            assigned[node] = candidate;
            used[candidate] = true;
            // dive in
            std::cout << "Node after assign: " << depth << std::endl;
            for(int k = 0; k < assigned.size(); k++) {
                std::cout << assigned[k] << " ";
            }
            std::cout << std::endl;
            std::cout << std::endl;
            
            res = checkScheme(depth+1, N, candidates, compAdjList, cableTypesCTU, assigned, used);
            if(res == true) {
                return true;
            }
            assigned[node] = -1;
            used[candidate] = false;
        }
    }

    return res;
}

/* MAIN */
int main() {
    // N - number of servers
    // M - number of connectios between servers
    // S - number of company schemes
    int N, M, S;

    // Load first line of input data
    std::cin >> N >> M;

    // Define vector for storing edges
    std::vector<std::tuple<int, int, int>> edgesCTU;
    std::vector<std::vector<int>> cableTypesCTU(N, std::vector<int>(N, 0));
    edgesCTU.reserve(M);
    
    // Load edges from input to the vector
    loadInputEdges(edgesCTU, cableTypesCTU, M);
    // printEdges(edgesCTU);
    // printCableTypesCTU(cableTypesCTU);
    
    // Load number of company schemes
    std::cin >> S;
    std::vector<std::vector<std::tuple<int, int, int>>> compSchemes;
    compSchemes.reserve(S);

    // Load edges of all companies schemes
    loadCompSchemesEdges(compSchemes, S);
    
    // for(int i = 0; i < S; i++) {
    //     printEdges(compSchemes[i]);
    //     std::cout << std::endl;
    // }

    std::vector<std::vector<std::tuple<int, int>>> compAdjList(compSchemes[0].size() + 1);
    // printEdges(compSchemes[0]);
    createAdjacencyList(compSchemes[3], compAdjList);

    printAdjacencyList(compAdjList);
    
    std::vector<std::vector<int>> candidates{
        {0, 1, 2, 3},
        {0, 1, 2, 3},
        {0, 1, 2, 3},
        {0, 1, 2, 3}
    };

    int node = 0;
    std::vector<int> candidatesForNode = candidates[node];
    std::vector<int> assigned(N, -1);
    std::vector<bool> used(N, false);

    bool res = checkScheme(0, N, candidates, compAdjList, cableTypesCTU, assigned, used);

    if(res == true) {
        std::cout << "True" << std::endl;
    } else {
        std::cout << "False" << std::endl;
    }

    // for(int i = 0; i < candidatesForNode.size(); i++) {
    //     int candidate = candidatesForNode[i];
    //     bool isMapPossible = checkPossibleMapping(node, candidate, compAdjList, cableTypesCTU, assigned);

    //     if(isMapPossible) {
    //         // Assigned mapping to node
    //         assigned[node] = candidate;
    //         // dive in
    //     }
    // }
    
    return 0;
}