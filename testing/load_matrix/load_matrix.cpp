#include <iostream>
#include <vector>

/* MAIN */
int main() {
    
    int N, K, L, S;

    std::cin >> N >> K >> L >> S;

    std::cout << N << std::endl;

    std::vector<std::vector<int>> matrix(N, std::vector<int>(N));

    
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            int num;
            std::cin >> num;
            matrix[i][j] = num;
        }
    }
    
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}