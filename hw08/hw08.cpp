#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <queue>
#include <set>
#include <numeric>


/* MAIN */
int main() {
    // Define variables
    int A_min, A_max;
    int C_min, C_max;
    int M_min, M_max;
    int D;
    
    // Load the input
    std::cin >> A_min >> A_max;
    std::cin >> C_min >> C_max;
    std::cin >> M_min >> M_max;
    std::cin >> D;

    std::vector<std::vector<int>> primesM(M_max+1);
    std::vector<int> smallestPrimes(M_max+1);
    // Store lcm of all prime factors of some M
    std::vector<long long> lcmPrimeFactors(M_max+1, 1);
    std::vector<int> dividers(M_max+1);

    // For each M precompute primes using erasthetas siege
    for (int i = 2; i <= M_max; i++) {
        if (smallestPrimes[i] == 0) {
            // Number is prime
            for (int j = i; j <= M_max; j += i) {
                if (smallestPrimes[j] == 0) {
                    smallestPrimes[j] = i;
                }
                primesM[j].push_back(i);
            }
        }
    }
    
    
    // For each M precompute number of dividers
    for (int i = 2; i <= M_max; i++) {
        int M_temp = i;
        int dTemp = 1;
        while (M_temp > 1) {
            int p = smallestPrimes[M_temp];
            int counter = 0;
            while (M_temp % p == 0) {
                M_temp = M_temp / p;
                counter++;
            }
            dTemp = dTemp * (counter + 1);
        }
        dividers[i] = dTemp;
        
        long long lcm = 1;
        for (int p : primesM[i]) {
            lcm = std::lcm(lcm, (long long)p);
        }
        // Give me the smallest number divisible by every prime factor of M.
        lcmPrimeFactors[i] = lcm;
    }

    int numOfGenerators = 0;
    
    bool isDivisible;
    int Mdivided4;

    // Loop from M_min to M_max and select Ms that have at least D dividors
    for (int m = M_min; m <= M_max; m++) {
        
        if(dividers[m] >= D) {
            
            Mdivided4 = m % 4;
            long long lcm = lcmPrimeFactors[m];
            // m is valid
            for(int c = C_min; c <= C_max; c++) {
                if(std::gcd(m, c) != 1) {
                    continue;
                }

                long long step = lcm;
                // if (Mdivided4 == 0) {
                //     step = std::lcm(lcm, 4LL);
                // }

                // Find the first valid A >= A_min such that (A-1) % step == 0
                long long firstA = A_min;
                long long rem = (firstA - 1) % step;
                if (rem != 0) {
                    firstA += (step - rem);
                }

                for (long long a = firstA; a <= A_max; a += step) {
                    if(Mdivided4 == 0 && (a-1) % 4 != 0) {
                        continue;
                    }
                    numOfGenerators++;
                }
            }
        }
    }
    
    std::cout << numOfGenerators << std::endl;
 
    return 0;
}