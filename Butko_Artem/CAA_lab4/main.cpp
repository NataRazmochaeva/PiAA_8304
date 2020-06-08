#include <iostream>
#include <string>
#include <vector>

void getPrefix(std::string pattern, std::vector<int>& prefix)
{
    int index = 0;
    for (int i = 1; i < pattern.size(); ++i)
    {
        while (index != -1 && pattern[index] != pattern[i]) index--;
        index++;
        prefix.push_back(index);
    }
}

void algorithmKMP(std::string pattern, std::vector<int>& result)
{
    int index = 0;
    char symb;
    int counter = 0;
    
    std::vector<int> prefix{0};
    getPrefix(pattern, prefix);

    while (std::cin >> symb)
    {
        counter++;
        while (index > 0 && pattern[index] != symb) index = prefix[index - 1];
        if (pattern[index] == symb) index++;
        if (index == pattern.size()) result.push_back(counter - index + 1);
    }
    if (result.empty()) result.push_back(-1);
}

void output(std::vector<int> result)
{
    for(int i = 0; i < result.size(); ++i)
    {
        std::cout << result[i];
        if (i + 1 != result.size()) std::cout << " ,";
    }
    std::cout << std::endl;
}

int main()
{
    std::string prefix;
    std::vector<int> result;

    std::getline(std::cin, prefix);

    algorithmKMP(prefix, result);
    output(result);
}
/*
defabc
abcdef
 */
