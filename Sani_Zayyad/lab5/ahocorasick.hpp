#ifndef ahocorasick_hpp
#define ahocorasick_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>

struct Vertex //Vertex structure
{
    std::map<char, int> next; //child of the topvertex
    std::map<char, int> go;   //path
    int prev = 0;        // parent index
    char prevChar = 0;   //symbol of the parent vertex
    int suffix = -1;     //index of the vertex of the suffix transition
    int number = 0;      //number of the terminal vertex
    int deep = 0;        //depth
    bool isLeaf = false; // whether the vertex is a leaf (terminal)
};


class AhoCorasick
{
public:
    AhoCorasick();

    void addPattern(const std::string& str) ;
    void search(const std::string& str);
    void printResult(const std::string& text) const;

private:
    void printMaxArcs() const;
    int getSuffix(int index);
    int getGo(int index, char ch);
    void printDetails() const;
    
private:
    std::vector<Vertex> vertexs;
    std::vector<int> result;
    int countVertex;
    int currentVertex;        
    std::vector<std::string> patternsArr;
};
#endif /* ahocorasick_hpp */
