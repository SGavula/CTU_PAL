#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cctype>

/* MAIN */
int main() {
    
    std::string line;

    // while (std::getline(std::cin, line)) {
    //     std::cout << "Read line: [" << line << "]\n";
    // }
    std::getline(std::cin, line);
    std::stringstream ss(line);
    std::streampos pos;
    std::streampos pos2;
    
    pos = ss.tellg();
    std::cout << "Position 1: " << pos << std::endl;

    size_t stateId;
    ss >> stateId;

    pos = ss.tellg();
    std::cout << "Position 2: " << pos << std::endl;  
    
    std::string finalFlag;
    ss >> finalFlag;

    pos2 = ss.tellg();
    std::cout << "Position but 2 var: " << pos2 << std::endl;

    ss.seekg(pos);
    
    pos = ss.tellg();
    std::cout << "Position 3: " << pos << std::endl;  


    // pos = ss.tellg();
    // std::cout << "Position 3: " << pos << std::endl;
    
    // std::cout << stateId << std::endl;
    // std::cout << finalFlag << std::endl;
    
    // char token;
    // ss >> token;
    // std::cout << "Token: " << token << std::endl;
    
    // pos = ss.tellg();
    // std::cout << "Position 4: " << pos << std::endl;

    // std::string next;
    // ss >> next;
    // std::cout << "Next: " << next << std::endl;
    // pos = ss.tellg();
    // std::cout << "Position: " << pos << std::endl;
    // ss.seekg(pos);
    // pos = ss.tellg();
    // std::cout << "Position: " << pos << std::endl;

    return 0;
}