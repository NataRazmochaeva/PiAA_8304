#ifndef ahowithjoker_hpp
#define ahowithjoker_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>

struct VertexStruct //Vertex structure
{
    std::map<char, int> next;
    std::map<char, int> go;
    std::vector<int> number;
    int prev = 0;
    int deep = 0;
    int suffix = -1;
    bool isLeaf = false;
    char prevChar = 0;
};

class Joker
{
public:
    explicit Joker(char joker);
    void readPattern(std::string& str);
    void search(const std::string& str);
    void printResult(const std::string& text) const;
    
private:
    void printMaxArcs() const;
    void split(std::string str);
    void addPattern(const std::string& str);
    int getSuffix(int index);
    int getGo(int index, char ch);
    void printDetails() const;
    
private:
    std::vector<VertexStruct> vertexs;
    char joker;
    int countTerminalVertex;
    std::vector<std::string> patternArr;
    int patternLen{};
    std::vector<int> matchPatterns;
    std::vector<int> patternsLength;
};

#endif /* ahowithjoker_hpp */
