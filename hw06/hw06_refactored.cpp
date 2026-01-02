#include <iostream>
#include <vector>

void generateSequence(const size_t A, const size_t C, const size_t M, std::vector<size_t>& generatedSequence) {
    generatedSequence.reserve(M);
    size_t x = 0;
    do {
        generatedSequence.push_back(x);
        x = (A * x + C) % M;
    } while (x != 0);
}

void calculatePrimeFactorCount(const size_t M, std::vector<size_t>& primeFactorCountAll, std::vector<size_t>& primeFactorCount) {
    for(size_t i = 2; i < M; i++) {
        if(primeFactorCount[i] == 0) {
            // number is primer
            for (size_t j = i; j < M; j += i) { 
                primeFactorCount[j]++;
                size_t x = j;
                while (x % i == 0) {
                    primeFactorCountAll[j]++;
                    x /= i;
                }
            }
        }
    }
}

void calculateValidSeqNumbers(const size_t M, const size_t K, std::vector<size_t>& primeFactorCountAll, std::vector<size_t>& primeFactorCount, std::vector<char>& good) {
    for(size_t i = 2; i < M; i++) {
        if((primeFactorCountAll[i] == primeFactorCount[i]) && (primeFactorCountAll[i] == K)) {
            good[i] = 1;
        }
    }
}

void chooseOptimalSeedAndCount(const size_t N, const size_t M, std::vector<size_t>& generatedSequence, std::vector<char>& good, size_t& S, size_t& I) {

    size_t currentCount = 0;

    for(size_t i = 0; i < N; i++) {
        size_t genNumber = generatedSequence[i];
        if(good[genNumber] == 1) {
            currentCount++;
        }
    }

    for(size_t i = 1; i < M; i++) {
        size_t prevIdx = (i-1) % M;
        size_t nextIdx = (i+N-1) % M;
        size_t prev = generatedSequence[prevIdx];
        size_t next = generatedSequence[nextIdx];

        if(good[prev] == 1) {
            currentCount--;
        }

        if(good[next] == 1) {
            currentCount++;
        }

        if(currentCount > I) {
            S = generatedSequence[i];
            I = currentCount;
        }
    }
}

void printGeneratedSequence(const std::vector<size_t>& sequence) {
    for(size_t i = 0; i < sequence.size(); i++) {
        std::cout << sequence[i] << " ";
    }
    std::cout << std::endl;
}

/* MAIN */
int main() {
    // Define variables
    size_t A, C, M, K, N;
    
    // Load the input
    std::cin >> A >> C >> M >> K >> N;
    std::vector<size_t> generatedSequence;

    // Generate sequence of pseudorandom numbers based on xi+1 = (A × xi + C) mod M 
    generateSequence(A, C, M, generatedSequence);

    std::vector<size_t> primeFactorCountAll(M, 0);
    std::vector<size_t> primeFactorCount(M, 0);
    
    calculatePrimeFactorCount(M, primeFactorCountAll, primeFactorCount);
    
    std::vector<char> good(M, 0);
    calculateValidSeqNumbers(M, K, primeFactorCountAll, primeFactorCount, good);

    size_t S = 0;
    size_t I = 0;
    chooseOptimalSeedAndCount(N, M, generatedSequence, good, S, I);

    std::cout << S << " " << I << std::endl;

    return 0;
}