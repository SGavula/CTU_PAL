#include <iostream>
#include <vector>

class DisjointSet {
    private:
        std::vector<int> parents;
        std::vector<int> rank;

    public:
        DisjointSet(int n) {
            parents.resize(n+1);
            rank.resize(n+1, 0);

            for(int i = 0; i <= n; i++) {
                parents[i] = i;
            }
        }

        int find(int node_id) {
            if(parents[node_id] == node_id) {
                return node_id;
            }

            parents[node_id] = find(parents[node_id]);
            return parents[node_id]; 
        }

        void union_sets(int n1, int n2) {
            int n1_par = find(n1);
            int n2_par = find(n2);

            if(n1_par != n2_par) {
                if(rank[n1_par] > rank[n2_par]) {
                    parents[n2_par] = n1_par;
                    rank[n1_par] = rank[n1_par] + rank[n2_par] + 1;
                } else {
                    parents[n1_par] = n2_par;
                    rank[n2_par] = rank[n2_par] + rank[n1_par] + 1;
                }
            }
        }
};


/* MAIN */
int main() {
    int size = 5;
    DisjointSet ds(size);
    ds.union_sets(1, 2);
    ds.union_sets(3, 4);
    bool inSameSet = (ds.find(1) == ds.find(2));
    std::cout << "Are 1 and 2 in the same set? " 
         << (inSameSet ? "Yes" : "No") << std::endl;
    return 0;
}