#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <queue>
#include <set>

void numberOfDividers(const int& M, std::vector<int>& primesM, int& d) {
    int del = 2;
    int M_temp = M;
    int dTemp = 1;
    std::set<int> primesMSet;

    while(del*del <= M) {
        int counter = 0;
        while(M_temp % del == 0) {
            counter++;
            M_temp = M_temp / del;
            primesMSet.insert(del);
        }
        
        del = del + 1;
        if(counter != 0) {
            dTemp = dTemp * (counter + 1);
        }
    }

    if(M_temp > 1) {
        primesMSet.insert(M_temp);
        dTemp = dTemp * 2;
    }

    primesM.assign(primesMSet.begin(), primesMSet.end());

    d = dTemp;
}

bool areNumbersCoprimes(const int& A, const int& B) {
    int A_temp; 
    int B_temp;
    if(A >= B) {
        A_temp = A;
        B_temp = B;
    }else {
        A_temp = B;
        B_temp = A;
    }
    int r;

    while(B_temp != 0) {
        r = A_temp % B_temp;
        A_temp = B_temp;
        B_temp = r;
    }

    if(A_temp == 1) {
        return true;
    }
    return false;
}

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

    std::vector<int> validMs;

    bool isDivisible;
    
    int Mdivided4;

    int numOfGenerators = 0;

    // Loop from M_min to M_max and select Ms that have at least D dividors
    for(int m = M_min; m <= M_max; m++) {
        std::vector<int> primesM;
        int numOfDividers;

        Mdivided4 = m % 4;

        numberOfDividers(m, primesM, numOfDividers);
        if(numOfDividers >= D) {
            // m is valid
            for(int c = C_min; c <= C_max; c++) {
                if(areNumbersCoprimes(m, c) == true) {
                    for(int a = A_min; a <= A_max; a++) {
                        isDivisible = true; 
                        for(int i = 0; i < primesM.size(); i++) {
                            if((a-1) % primesM[i] != 0) {
                                isDivisible = false;
                                break;
                            }
                        }

                        if(isDivisible == true) {
                            if(Mdivided4 == 0) {
                                if((a-1) % 4 == 0) {
                                    numOfGenerators++;
                                }
                            } else {
                                numOfGenerators++;
                            }
                        }

                    }

                }
            }
        }
        // break;
    }

    std::cout << numOfGenerators << std::endl;

 
    return 0;
}