#include <iostream>
#include <vector>
#include <tuple>
 
class Graph {
    private:
        int N;
        int M;
        std::vector<std::tuple<int, int, int>> edges;
        std::vector<std::vector<int>> cableTypesMatrix;
 
    public:
        Graph(int N, int M) : N(N), M(M) {
            // Initialize the matrix with N x N dimensions
            cableTypesMatrix.resize(N, std::vector<int>(N, 0));
        }
 
        void loadInputEdges() {
            for (int i = 0; i < M; i++) {
                int n1, n2, type;
                std::cin >> n1 >> n2 >> type;
                edges.emplace_back(n1, n2, type);
                cableTypesMatrix[n1][n2] = type;
                cableTypesMatrix[n2][n1] = type;
            }         
        }
 
        void printEdges() {
            for (size_t i = 0; i < edges.size(); i++) {
                int n1, n2, cost;
                std::tie(n1, n2, cost) = edges[i];
                std::cout << "(" << n1 << ", " << n2 << ", " << cost << ")" << std::endl;
            }
        }
 
        void printCableTypesMatrix() {
            for(int i = 0; i < cableTypesMatrix.size(); i++) {
                for(int j = 0; j < cableTypesMatrix[i].size(); j++) {
                    std::cout << cableTypesMatrix[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }
 
        // Getter for entire matrix
        const std::vector<std::vector<int>>& getCableTypesMatrix() const {
            return cableTypesMatrix;
        }
 
        // Getter for single element
        int getCableType(int n1, int n2) const {
            return cableTypesMatrix[n1][n2];
        }
 
        void setCableTypesMatrix(const std::vector<std::vector<int>>& matrix) {
            cableTypesMatrix = matrix;
        }
};
 
void printMatrix(const std::vector<std::vector<int>>& matrix) {
    for(int i = 0; i < matrix.size(); i++) {
        for(int j = 0; j < matrix[i].size(); j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
 
bool checkPossibleMapping(
    int node,
    const std::vector<int>& mapping,
    const Graph& graphCTU,
    const Graph& graphComp
) {
    if(node == 0) {
        // We mapped first node but we cannot check with no other nodes
        return true;
    }
 
    const std::vector<std::vector<int>>& cableTypesMatrixCTU = graphCTU.getCableTypesMatrix();
    const std::vector<std::vector<int>>& cableTypesMatrixComp = graphComp.getCableTypesMatrix();
 
    for(int i = (node-1); i >= 0; i--) {
        int nodeForChecking = i;
        int mappedNode = mapping[node];
        int mappedNodeForChecking = mapping[nodeForChecking];
        int typeComp = cableTypesMatrixComp[node][nodeForChecking];
        int typeCTU = cableTypesMatrixCTU[mappedNode][mappedNodeForChecking]; 
        if(typeComp == typeCTU) {
            // Cables are equal and continue to check outher node
            continue;
        } else if(typeCTU != 0) {
            if(typeComp == -1) {
                // Cables are equal and continue to check outher node
                continue;
            }
            return false;
        } else {
            return false;
        }
    }
    return true;
}
 
bool checkIsomorphism(int node, int N, std::vector<int> mapping, std::vector<bool> used, Graph graphCTU, Graph graphComp) {
    if(node == N) {
        // for(int i = 0; i < mapping.size(); i++) {
        //     std::cout << mapping[i] << " ";
        // }
        return true;
    }
 
    bool res = false;
 
    // std::cout << "Node to be mapped: " << node << std::endl; 
 
    for(int candidate = 0; candidate < N; candidate++) {
        if(used[candidate]) {
            continue;
        }
 
        mapping[node] = candidate;
        used[candidate] = true;
        // Fill used
        bool possibleMapping = checkPossibleMapping(node, mapping, graphCTU, graphComp);
        if(possibleMapping) {
            res = checkIsomorphism(node+1, N, mapping, used, graphCTU, graphComp);
            if(res) {
                // Break if response from checkIsomorphism return true
                break;
                // return true;
            }
        }
        mapping[node] = 0;
        used[candidate] = false;
    }
 
    return res;
}
 
/* MAIN */
int main() {
    // N - number of servers
    // M - number of connectios between servers
    // S - number of company schemes
    // M_comp - number of connection between servers in company scheme
    int N, M, S, M_comp;
 
    // Load first line of input data
    std::cin >> N >> M;
     
    Graph graphCTU(N, M);
    graphCTU.loadInputEdges();
 
    std::vector<std::vector<int>> cableTypesCTU = graphCTU.getCableTypesMatrix();
     
    for(int i = 0; i < cableTypesCTU.size(); i++) {
        for(int j = 0; j < cableTypesCTU[i].size(); j++) {
            if(cableTypesCTU[i][j] != 0) {
                cableTypesCTU[i][j] = -1;
            }
        }
    }
 
    // Load number of company schemes
    std::cin >> S;
     
    bool first = true;
    for(int i = 0; i < S; i++) {
        std::cin >> M_comp;
        Graph graphComp(N, M_comp);
        graphComp.setCableTypesMatrix(cableTypesCTU);
        graphComp.loadInputEdges();
        std::vector<int> mapping(N);
        std::vector<bool> used(N, false);
        bool res = checkIsomorphism(0, N, mapping, used, graphCTU, graphComp);
        if(res) {
            if(!first) {
                std::cout << " ";
            }
            std::cout << i+1;
            first = false;
        }
    }
     
    std::cout << std::endl;
 
    return 0;
}
