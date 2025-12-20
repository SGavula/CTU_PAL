#include <iostream>
#include <vector>
#include <tuple>
#include <string>

class BasicUnit {
    private:
        size_t cost;
        std::string sequence;

    public:
        BasicUnit(size_t cost, const std::string& sequence) 
        : cost(cost), sequence(sequence) {}
        
        size_t getCost() const {
            return cost;
        }
        std::string getSequence() const {
            return sequence;
        }
};

void loadBasicUnits(const size_t N, std::vector<BasicUnit>& basicUnits) {
    for(size_t i = 0; i < N; i++) {
        size_t cost;
        std::string sequence;

        std::cin >> cost >> sequence;
        basicUnits.push_back(BasicUnit(cost, sequence));
    }
}

void updateDP(std::vector<std::pair<int, int>>& dp, int targetIndex,
    int newCost, int unitCount) {
    const int INF = 1e9;
    // Check if new cost is smaller than saved cost or if they are equal check if count of basic units is smaller that saved
    if (newCost < dp[targetIndex].first ||
        (newCost == dp[targetIndex].first && unitCount < dp[targetIndex].second)) {
        dp[targetIndex].first = newCost;
        dp[targetIndex].second = unitCount;
    }
}

void compareTwoSequences(const std::string& targetSequence, const BasicUnit& basicUnit, std::vector<std::pair<int, int>>& dp, const size_t Dmax, const int jTargetOriginal) {
    // Decompose basic unit
    int basicUnitCost = basicUnit.getCost();
    std::string basicUnitSequence = basicUnit.getSequence();

    // Define sizes of sequences
    int sizeTarget = targetSequence.size();
    int sizeBasic = basicUnitSequence.size();

    // Define indexes
    int jTarget = jTargetOriginal;
    // jBasic always starts from 0
    int jBasic = 0;

    // Define variables for counting how many characters are matched and how many are removed
    int matchCount = 0; 
    int removedCount = 0;

    // Define about how many character should to be moved after character match
    int movedIndex;
    int cost, newCost, oldCost;

    const int INF = 1e9;

    while(true) {
        // Check if one of the index is at the end of the sequence
        if(jTarget >= sizeTarget || jBasic >= sizeBasic) {
            break;
        }
        // Check if characters at current indexes are the same
        if(targetSequence[jTarget] == basicUnitSequence[jBasic]) {
            // Move indexes
            jTarget++;
            jBasic++;

            // Coumpute number of matches and removed characters
            matchCount++;
            removedCount = sizeBasic - matchCount;
            if(removedCount > Dmax) {
                // Continue to compare next two characters 
                continue;
            }


            movedIndex = jTargetOriginal + matchCount;
            
            cost = removedCount + basicUnitCost;
            newCost = dp[movedIndex].first == INF ? INF : cost + dp[movedIndex].first;
            oldCost = dp[jTargetOriginal].first;

            int unitCount = 1 + dp[movedIndex].second;
            
            updateDP(dp, jTargetOriginal, newCost, unitCount);
        } else {
            jBasic++;
        }
    }
}

void printDPTable(const std::vector<std::pair<int, int>>& dp) {
    for(size_t i = 0; i < dp.size(); i++) {
        std::cout << "dp[" << i << "] = (" << dp[i].first << ", " << dp[i].second << ")" << std::endl;
    }
}


/* MAIN */
int main() {
    // Define variables
    std::string targetSequence;
    size_t N;
    size_t Dmax;
    std::vector<BasicUnit> basicUnits;

    // Read target sequence
    std::cin >> targetSequence;

    // Read the second line
    std::cin >> N >> Dmax;
    
    basicUnits.reserve(N);

    // Load the basic units
    loadBasicUnits(N, basicUnits);

    int targetLength = targetSequence.size();
    const int INF = 1e9;
    // Define dp array (cost, count of basic units)
    std::vector<std::pair<int, int>> dp(targetLength + 1, {INF, 0});
    dp[targetSequence.size()] = {0, 0};

    for(int targetIndex = targetLength-1; targetIndex >= 0; targetIndex--) {
        for(size_t i = 0; i < basicUnits.size(); i++) {
            const BasicUnit& basicUnit = basicUnits[i];
            compareTwoSequences(targetSequence, basicUnit, dp, Dmax, targetIndex);
        }
    }

    std::cout << dp[0].first << " " << dp[0].second << std::endl;

    return 0;
}