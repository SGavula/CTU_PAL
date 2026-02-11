#include <iostream>
#include <algorithm>
#include <vector>

/* MAIN */
int main() {
    // SORT SIMPLE NUMBERS
    std::vector<int> nums = {5, 2, 8, 1, 9};
    
    // Sort in ascending order
    std::sort(nums.begin(), nums.end());
    
    // Print: 1 2 5 8 9
    for (int n : nums) {
        std::cout << n << " ";
    }

    std::cout << std::endl;
    
    // SORT VECTORS WITH PAIRS
    std::vector<std::pair<int, int>> pairs = {
        {3, 100},
        {1, 200},
        {2, 150},
        {1, 50}
    };
    
    // Sort by first element (default behavior)
    std::sort(pairs.begin(), pairs.end());
    
    // Result: (1,50) (1,200) (2,150) (3,100)
    for (auto& p : pairs) {
        std::cout << "(" << p.first << "," << p.second << ") ";
    }
    std::cout << std::endl;

    // First ascending, second descending
    std::sort(pairs.begin(), pairs.end(), 
        [](const auto& a, const auto& b) {
            if (a.first != b.first)
                return a.first < b.first;
            return a.second > b.second;
        });
    // Result: (1,200) (1,75) (1,50) (2,150) (3,100)
    
    for (auto& p : pairs) {
        std::cout << "(" << p.first << "," << p.second << ") ";
    }
    std::cout << std::endl;
    
    std::sort(pairs.begin(), pairs.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        }
    );
    // Result: (1,50) (3,100) (2,150) (1,200)
    
    for (auto& p : pairs) {
        std::cout << "(" << p.first << "," << p.second << ") ";
    }
    std::cout << std::endl;
    
    return 0;
}