#include <iostream>
#include <vector>
#include <tuple>
#include <sstream>
#include <string>
#include <cctype>
#include <unordered_map>
#include <queue>
#include <algorithm>

enum StateType {
  INPUT,
  NORMAL,
  FINAL
};

class State {
    private:
        size_t N;
        size_t S;
        StateType type;
        std::unordered_map<char, std::vector<int>> transitions;
    
    public:
        State(size_t N, size_t S, const std::unordered_map<char, std::vector<int>>& allTransitions, StateType stateType) 
            : N(N), S(S), transitions(allTransitions), type(stateType) {}
        StateType getStateType() {
            return type;
        }

        void setStateType(StateType value) {
            type = value;
        }       

        std::unordered_map<char, std::vector<int>> getTransitions() {
            return transitions;
        }

        void pushToTransition(const char symbol, const int stateId) {
            transitions[symbol].push_back(stateId);
        }

        void printTransitions() {
            // For all symbols
            for(size_t i = 0; i < transitions.size(); i++) {
                char symbol = 'a' + i;
                std::vector<int> nums = transitions[symbol];
                
                std::cout << symbol << " ";
                for(int j = 0; j < nums.size(); j++) {
                    std::cout << nums[j] << " ";
                }
            }

            std::cout << " | " << transitions.size() << std::endl;
        }
};

class StateIntersection {
    private:
        size_t N;
        size_t S;
        StateType type;
        std::unordered_map<char, std::vector<std::tuple<int, int>>> transitions;
    
    public:
        StateIntersection(size_t N, size_t S, const std::unordered_map<char, std::vector<std::tuple<int, int>>>& allTransitions, StateType stateType) 
            : N(N), S(S), transitions(allTransitions), type(stateType) {}
        
            StateType getStateType() {
            return type;
        }

        void setStateType(StateType value) {
            type = value;
        }       

        std::unordered_map<char, std::vector<std::tuple<int, int>>> getTransitions() {
            return transitions;
        }

        void pushToTransition(const char symbol, const std::tuple<int, int> stateId) {
            transitions[symbol].push_back(stateId);
        }

        void printTransitions() {
            // For all symbols
            for(size_t i = 0; i < transitions.size(); i++) {
                char symbol = 'a' + i;
                std::vector<std::tuple<int, int>> nums = transitions[symbol];
                
                std::cout << symbol << " ";
                for(int j = 0; j < nums.size(); j++) {
                    std::cout << "(" << std::get<0>(nums[j]) << "," << std::get<1>(nums[j]) << ") ";
                }
            }

            std::cout << " | " << transitions.size() << std::endl;
        }
};

StateType inverseStateType(StateType state) {
    if(state == INPUT) {
        return FINAL;
    } else if(state == FINAL) {
        return INPUT;
    } else {
        return NORMAL;
    }
}
 
bool isInteger(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit(c) && c != '-') return false;
    return true;
}

void loadInput(const size_t N, const size_t S, std::vector<State>& states, std::vector<State>& statesInversed) {
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;

        int stateId;
        ss >> stateId;  // first number

        std::string final_flag;
        ss >> final_flag; // "F" or "-"

        // std::cout << stateId << " " << final_flag << std::endl;
        StateType stateType;
        if(final_flag == "F") {
            stateType = FINAL;
        }else if(stateId == 0) {
            stateType = INPUT;
        }else {
            stateType = NORMAL;
        }

        std::unordered_map<char, std::vector<int>> transitions;

        // Now parse repeating: char <ints...>
        while (ss >> token) {
            char symbol = token[0];  // e.g., 'a', 'b', 'c', 'd'

            std::vector<int> nums;

            std::streampos pos;
            std::string next;

            while (true) {
                pos = ss.tellg();       // save position
                if (!(ss >> next)) break; // no more tokens

                if (!isInteger(next)) {
                    ss.seekg(pos);      // rollback – it's next character
                    break;
                }
                
                // stoi --> converts string to integer
                int nextInt = std::stoi(next);
                nums.push_back(nextInt);
                statesInversed[nextInt].pushToTransition(symbol, stateId);
            }

            if(!nums.empty()) {
                transitions[symbol] = nums;
            }
        }

        // Print parsed result (for debugging)
        State state(N, S, transitions, stateType);
        states.push_back(state);
        statesInversed[stateId].setStateType(inverseStateType(stateType));
    }
}

void createIntersectionAutomaton(const size_t N, const size_t S, const std::vector<State>& states, const std::vector<State>& statesInversed, std::vector<std::vector<StateIntersection>>& stateIntersection, std::vector<std::tuple<int, int>>& initStates) {
    for(size_t i = 0; i < N; i++) {
        State currState = states[i];
        
        for(size_t j = 0; j < N; j++) {
            State currStateInversed = statesInversed[j];
            if(currState.getStateType() == FINAL && currStateInversed.getStateType() == FINAL) {
                stateIntersection[i][j].setStateType(FINAL);
            } else if(currState.getStateType() == INPUT && currStateInversed.getStateType() == INPUT) {
                stateIntersection[i][j].setStateType(INPUT);
                initStates.push_back(std::make_tuple(i, j));
            }

            for(size_t k = 0; k < S; k++) {
                char symbol = 'a' + k;
                std::vector<int> currStateTransitions = currState.getTransitions()[symbol];
                std::vector<int> currStateInversedTransitions = currStateInversed.getTransitions()[symbol];
                
                for(size_t l = 0; l < currStateTransitions.size(); l++) {
                    int nextState1 = currStateTransitions[l];
                    for(size_t m = 0; m < currStateInversedTransitions.size(); m++) {
                        int nextState2 = currStateInversedTransitions[m];
                        std::tuple<int, int> stateId = std::make_tuple(nextState1, nextState2);
                        stateIntersection[i][j].pushToTransition(symbol, stateId);
                    }
                }
            }
        }
    }
}

std::string findShortestWordBFS(const std::vector<std::vector<StateIntersection>>& stateIntersection, const std::vector<std::tuple<int, int>>& initStates) {
    // Init queue for BFS
    std::queue<std::tuple<std::tuple<int, int>, int, std::string>> queue;

    // Add init states to bfs 
    for (size_t i = 0; i < initStates.size(); i++) {
        int idx1 = std::get<0>(initStates[i]);
        int idx2 = std::get<1>(initStates[i]);
       
        queue.emplace(initStates[i], 0, std::string{});
    }


    std::string finalWord = "";
    int finalLength = -1;
    
    while(!queue.empty()) {
        // Get first element on the top of queue
        std::tuple<std::tuple<int, int>, int, std::string> topValue = queue.front();
        queue.pop();
        std::tuple<int, int> currStateIdxs = std::get<0>(topValue);
        int idx1 = std::get<0>(currStateIdxs);
        int idx2 = std::get<1>(currStateIdxs);
        StateIntersection currState = stateIntersection[idx1][idx2];
        int currLength = std::get<1>(topValue);
        std::string currWord = std::get<2>(topValue);
        
        // Check if we go over all words that have length of shortest final word
        if(finalLength != -1 && finalLength == currLength-1) {
            break;
        }
        
        // Check if currect state is not final
        if(currState.getStateType() == FINAL) {
            if(finalWord.empty()) {
                finalWord = currWord;
                finalLength = currLength;
            } else if(finalWord > currWord) {
                finalWord = currWord;
            }
        }
        
        
        std::unordered_map<char, std::vector<std::tuple<int, int>>> transitions = currState.getTransitions();
        for(size_t i = 0; i < transitions.size(); i++) {
            char symbol = 'a' + i;
            std::vector<std::tuple<int, int>> nextStates = transitions[symbol];
            for(size_t j = 0; j < nextStates.size(); j++) {
                int nextIdx1 = std::get<0>(nextStates[j]);
                int nextIdx2 = std::get<1>(nextStates[j]);
                std::string newWord = currWord + symbol;
                int newLength = currLength + 1;
                queue.emplace(std::make_tuple(nextIdx1, nextIdx2), newLength, newWord);
            }
        }
    }
    return finalWord;
}

/* MAIN */
int main() {
    // N - number of states
    // S - number of characters in alphabet
    size_t N, S;
 
    // Load first line of input data
    std::cin >> N >> S;

    // Define states of input NFA automata
    std::vector<State> states;
    states.reserve(N);
    
    // Define empty transitions for states of inversed NFA automata
    std::unordered_map<char, std::vector<int>> emptyTransitions;
    emptyTransitions.reserve(S);
    State defaultState(N, S, emptyTransitions, NORMAL);
    // Define states of inversed NFA automata
    std::vector<State> statesInversed;
    statesInversed.resize(N, defaultState);
    
    loadInput(N, S, states, statesInversed);
    
    // Define empty transitions for states of intersection NFA automata
    std::unordered_map<char, std::vector<std::tuple<int, int>>> emptyTransitionsIntersection;
    StateIntersection defaultStateIntersection(N, S, emptyTransitionsIntersection, NORMAL);
    // Define states of intersection NFA automata
    std::vector<std::vector<StateIntersection>> stateIntersection(
        N,
        std::vector<StateIntersection>(N, defaultStateIntersection)
    );
    // Define states that are init in intersection NFA automata to save it for BFS queue
    std::vector<std::tuple<int, int>> initStates;

    createIntersectionAutomaton(N, S, states, statesInversed, stateIntersection, initStates);

    std::string finalWord = findShortestWordBFS(stateIntersection, initStates);

    std::cout << finalWord;

    return 0;
}