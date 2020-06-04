#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ahocorasick.hpp"
#include "joker.hpp"

int main() {
    int choice;
    std::cout << "Choose Aho-Corrasick method- 1(without joker), 2(with joker): " << std::endl;
    std::cin >> choice;
    
    if(choice == 1){
        std::string str;
        int count = 0;
        std::cout << "Enter text string:" << std::endl;
        std::cin >> str;
        std::cout << "Enter count patterns:" << std::endl;
        std::cin >> count;

        std::string pattern;
        std::vector<std::string> patterns;
        std::cout << "Enter all patterns:" << std::endl;
        for (int i = 0; i < count; i++){
            std::cin >> pattern;
            patterns.push_back(pattern);
        }

        auto* ahoCorasick = new AhoCorasick();
        for (int i = 0; i < count; i++) {
            ahoCorasick->addPattern(patterns[i]); //filling the bor with patterns
        }

        ahoCorasick->search(str); // search
        ahoCorasick->printResult(str); //printing result and details
    }else if (choice == 2){
        std::string str;
        std::string pattern;
        char joker;
        std::cout << "Enter text string:" << std::endl;
        std::cin >> str;
        std::cout << "Enter pattern:" << std::endl;
        std::cin >> pattern;
        std::cout << "Enter joker:" << std::endl;
        std::cin >> joker;
        
        auto* ahoCorasick = new Joker(joker);
        ahoCorasick->readPattern(pattern); //processing pattern
        ahoCorasick->search(str); //searching result
        ahoCorasick->printResult(str);  // printing result with details
    }else{
      return 0;
    }
    
   

    return 0;
}
