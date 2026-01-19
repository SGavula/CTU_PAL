#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <queue>

void printGraph(const std::vector<std::vector<int>>& graph, const int& M) {
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < M; j++) {
            std::cout << graph[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void printRanks(const std::vector<int>& ranks, const int& N) {
    for(int i = 0; i < N; i++) {
        std::cout << ranks[i] << " ";
    }

    std::cout << std::endl;
}

void printAdjacencyMatrix(const std::vector<std::vector<int>>& adjList, const int& N) {
    for(int i = 0; i < N; i++) {
        int size = adjList[i].size();
        std::cout << i << " | ";
        for(int j = 0; j < size; j++) {
            std::cout << adjList[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void doMapping(const int& graph1Node, const int& N, const std::vector<int>& ranksGraph1, const std::vector<int>& ranksGraph2, std::vector<int>& mapping) {
    if(graph1Node == N) {
        // Found mapping
        // Return smallest nodes on cycle 
        return;
    }
    
    for(int i = 0; i < N; i++) {
        if(ranksGraph1[graph1Node] == ranksGraph2[i]) {
            // Same degree can be mapped
            mapping[graph1Node] = i;
            // Go deeper
            doMapping(graph1Node+1, N, ranksGraph1, ranksGraph1, mapping);
            
        }
    }
}

void findRoot(const int& N, const std::vector<int>& ranksGraph, const std::vector<std::vector<int>>& adjListGraph1, int& root) {
    std::queue<int> queue;
    std::vector<bool> visited(N, false);

    for(int i = 0; i < N; i++) {
        if(ranksGraph[i] == 1) {
            queue.emplace(i);
            visited[i] = true;
        }
    } 

    while(true) {
        int curr = queue.front();
        queue.pop();

        if(queue.empty()) {
            // std::cout << curr << std::endl;
            root = curr;
            break;
        }

        for(int i = 0; i < adjListGraph1[curr].size(); i++) {
            int neighbour = adjListGraph1[curr][i];
            if(visited[neighbour] == true) {
                continue;
            }
            
            queue.emplace(neighbour);
            visited[neighbour] = true;
        }
    }
}

void findCycle(const int& N, std::vector<int>& ranksGraph, const std::vector<std::vector<int>>& adjListGraph1, std::vector<int>& cycle) {
    std::queue<int> queue2;
    std::vector<bool> visited2(N, false);
    std::vector<int> ranksGraphTemp = ranksGraph;
    
    for(int i = 0; i < N; i++) {
        if(ranksGraphTemp[i] == 1) {
            queue2.emplace(i);
            visited2[i] = true;
            ranksGraphTemp[i]--;
        }
    } 

    while(true) {
        int curr = queue2.front();
        queue2.pop();

        // std::cout << curr << " rank: " << ranksGraph1[curr] << std::endl;

        if(ranksGraphTemp[curr] > 1) {
            // std::cout << "Node in cycle: " << curr << std::endl;
            cycle.push_back(curr);
            if(queue2.empty()) {
                // std::cout << curr << std::endl;
                break;
            }

            continue;
        }

        for(int i = 0; i < adjListGraph1[curr].size(); i++) {
            int neighbour = adjListGraph1[curr][i];
            if(visited2[neighbour] == true) {
                continue;
            }

            // std::cout << "Push to queue: " << neighbour << " from node: " << curr << std::endl;
            queue2.emplace(neighbour);
            visited2[neighbour] = true;
            ranksGraphTemp[neighbour]--;
        }
    }
}



/* MAIN */
int main() {
    // Define variables
    int N, M;
    
    // Load the input
    std::cin >> N >> M;
    // std::cout << N << " " << M << std::endl;

    // Define vector for storing edges of graphs
    std::vector<std::vector<int>> graph1(M, std::vector<int>(M, -1));
    std::vector<std::vector<int>> graph2(M, std::vector<int>(M, -1));
    // Define rank for each graphs
    std::vector<int> ranksGraph1(N, 0);
    std::vector<int> ranksGraph2(N, 0);
    // Define adjacency list
    std::vector<std::vector<int>> adjListGraph1(N);
    std::vector<std::vector<int>> adjListGraph2(N);

    // Load edges for first graph
    int edge1;
    int edge2;
    for(int i = 0; i < M; i++) {
        std::cin >> edge1 >> edge2;
        // std::cout << edge1 << " " << edge2 << std::endl;
        // Write edges to the adjacency matrix
        graph1[edge1][edge2] = 1;
        graph1[edge2][edge1] = 1;
        // Write ranks
        ranksGraph1[edge1] = ranksGraph1[edge1] + 1;
        ranksGraph1[edge2] = ranksGraph1[edge2] + 1;
        // Define adjacency list
        adjListGraph1[edge1].push_back(edge2);
        adjListGraph1[edge2].push_back(edge1);
    }

    // RUN BFS
    int root1;
    std::vector<int> cycle1;
    findRoot(N, ranksGraph1, adjListGraph1, root1);
    findCycle(N, ranksGraph1, adjListGraph1, cycle1);

    // std::queue<int> queue;
    // std::vector<bool> visited(N, false);
    // int root;

    // for(int i = 0; i < N; i++) {
    //     if(ranksGraph1[i] == 1) {
    //         queue.emplace(i);
    //         visited[i] = true;
    //         ranksGraph1[i]--;
    //     }
    // } 

    // while(true) {
    //     int curr = queue.front();
    //     queue.pop();

    //     if(queue.empty()) {
    //         // std::cout << curr << std::endl;
    //         root = curr;
    //         break;
    //     }

    //     for(int i = 0; i < adjListGraph1[curr].size(); i++) {
    //         int neighbour = adjListGraph1[curr][i];
    //         if(visited[neighbour] == true) {
    //             continue;
    //         }
            
    //         queue.emplace(neighbour);
    //         visited[neighbour] = true;
    //         ranksGraph1[neighbour]--;
    //     }
    // }

    // std::queue<int> queue2;
    // std::vector<bool> visited2(N, false);
    // std::vector<int> cycle;
    
    // for(int i = 0; i < N; i++) {
    //     if(ranksGraph1[i] == 1) {
    //         queue2.emplace(i);
    //         visited2[i] = true;
    //         ranksGraph1[i]--;
    //     }
    // } 

    // while(true) {
    //     int curr = queue2.front();
    //     queue2.pop();

    //     // std::cout << curr << " rank: " << ranksGraph1[curr] << std::endl;

    //     if(ranksGraph1[curr] > 1) {
    //         // std::cout << "Node in cycle: " << curr << std::endl;
    //         cycle.push_back(curr);
    //         if(queue2.empty()) {
    //             // std::cout << curr << std::endl;
    //             break;
    //         }

    //         continue;
    //     }

    //     for(int i = 0; i < adjListGraph1[curr].size(); i++) {
    //         int neighbour = adjListGraph1[curr][i];
    //         if(visited2[neighbour] == true) {
    //             continue;
    //         }

    //         // std::cout << "Push to queue: " << neighbour << " from node: " << curr << std::endl;
    //         queue2.emplace(neighbour);
    //         visited2[neighbour] = true;
    //         ranksGraph1[neighbour]--;
    //     }
    // }


    std::cout << "Root: " << root1 << std::endl;
    
    for(int i = 0; i < cycle1.size(); i++) {
        std::cout << cycle1[i] << " ";
    }
    
    std::cout << std::endl;

    // for(int i = 0; i < leafs.size(); i++) {
    //     std::cout << leafs[i] << " ";
    // }

    // std::cout << std::endl;

    // printAdjacencyMatrix(adjListGraph1, N);
    // std::cout << adjListGraph1[0].pop_back(1) << std::endl;
    // adjListGraph1[0].pop_back();
    // printAdjacencyMatrix(adjListGraph1, N);
    // printRanks(ranksGraph1, N);

    
    for(int i = 0; i < M; i++) {
        std::cin >> edge1 >> edge2;
        // std::cout << edge1 << " " << edge2 << std::endl;
        graph2[edge1][edge2] = 1;
        graph2[edge2][edge1] = 1;
        // Write ranks
        ranksGraph2[edge1] = ranksGraph2[edge1] + 1;
        ranksGraph2[edge2] = ranksGraph2[edge2] + 1;
        
        adjListGraph2[edge1].push_back(edge2);
        adjListGraph2[edge2].push_back(edge1);
    }
    // printRanks(ranksGraph2, N);
    // printAdjacencyMatrix(adjListGraph2, N);

    int root2;
    std::vector<int> cycle2;
    findRoot(N, ranksGraph2, adjListGraph2, root2);
    findCycle(N, ranksGraph2, adjListGraph2, cycle2);

    std::cout << "Root 2: " << root2 << std::endl;
    
    for(int i = 0; i < cycle2.size(); i++) {
        std::cout << cycle2[i] << " ";
    }
    
    std::cout << std::endl;

    // Mapping
    // Indexes in mapping vector respresent the nodes in first graph, values represents nodes in second graph
    std::vector<int> mapping(N, -1);
    mapping[root1] = root2;

    // For node 0 in graph 1
    for(int i = 0; i < N; i++) {
        if(i == root1) {
            continue;
        }

        if(mapping[i] == -1) {
            // If node is not mapped
            if(i ) {

            }
            mapping[i] = i;
            // Zanorit sa

        }
        // if(mapping[node1Idx] != -1) {
        //     for(int node2Idx = 0; node) {
        //         mapping[0] = i;

        //     }
        // }
    }

    return 0;
}