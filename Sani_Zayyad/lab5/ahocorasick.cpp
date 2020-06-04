#include "ahocorasick.hpp"

AhoCorasick::AhoCorasick(){
    Vertex root;
    root.prev = -1;
    vertexs.push_back(root);
    countVertex = 0;
}

void AhoCorasick::addPattern(const std::string& str)
{
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "Add symbols of new pattern in prefix tree" << std::endl;
    currentVertex = 0;
    for (char i : str) {
        std::cout << "\tReceived character " << i << std::endl;
        if (vertexs[currentVertex].next.find(i) == vertexs[currentVertex].next.end()) {  //if there is no character transition for the current vertex (perexoda)
            Vertex vertex; //vertex is created and added
            vertex.prev = currentVertex;
            vertex.prevChar = i;
            vertexs.push_back(vertex);
            vertexs[currentVertex].next[i] = vertexs.size() - 1;
            std::cout << "\tAdd new vertex in prefix tree with number " << vertexs.size() - 1 << std::endl;
            std::cout << "\tSymbol path to the vertex " << i << std::endl;
        }
        currentVertex = vertexs[currentVertex].next[i];
        std::cout << "\tTransition in vertex by symbol " << i << std::endl << std::endl;
    }
    
    countVertex++;
    patternsArr.push_back(str);
    vertexs[currentVertex].number = countVertex; // number of terminal vcertex
    vertexs[currentVertex].isLeaf = true;                                   //the vertex is declared terminal
    vertexs[currentVertex].deep = str.size();
    std::cout << "New terminal vertex is " << vertexs[currentVertex].prevChar << std::endl;
    std::cout << "Count of terminal vertex " << vertexs[currentVertex].number << std::endl;
    std::cout << "Deep of terminal vertex " << vertexs[currentVertex].deep << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
}

void AhoCorasick::search(const std::string& str)
{
    std::cout << std::endl << "\nSearch for patterns in the line is started" << std::endl << std::endl << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    int curr = 0;
    bool terminalVertexFound;
    for (size_t i = 0; i < str.size(); i++) {
        std::cout << "Search for symbols " << str[i] << std::endl;
        curr = getGo(curr, str[i]);                                         // for each character, we move to a new vertex
        std::cout << "\nStart check terminal vertex and check terminal vertex by suffix link" << std::endl;
        terminalVertexFound = false;
        for (int tmp = curr; tmp != 0; tmp = getSuffix(tmp)) {
            if (vertexs[tmp].isLeaf) {                                      // if the symbol is terminal, we add the index to the result array
                result.push_back(i - vertexs[tmp].deep + 2);
                result.push_back(vertexs[tmp].number);
                terminalVertexFound = true;
                std::cout << "Terminal vertex found, found pattern with index " << i - vertexs[tmp].deep + 2 << " and number " << vertexs[tmp].number << std::endl;
                break;
            }
        }
        if (!terminalVertexFound){
            std::cout << "Terminal vertex was not found, go to the next symbol" << std::endl;
        }
        std::cout << "-----------------------------------------" << std::endl;
    }
}

void AhoCorasick::printResult(const std::string& text) const{
    printDetails();
    printMaxArcs();
    std::vector<bool> cutStr(text.size());  // vector for matches
    std::string textRest;
    std::cout << "The result of algorithm work:" <<std::endl;
    for (size_t i = 0; i < result.size(); i += 2) {
        std::cout << result[i] << " " << result[i + 1] << std::endl;
        for (int j = 0; j < patternsArr[result[i+1] - 1].size(); j++)
            cutStr[result[i] - 1 + j] = true;
    }
    
    for (int i = 0; i < cutStr.size(); i++){
        if (!cutStr[i])
            textRest.push_back(text[i]);
    }
    
    std::cout << std::endl << "String without found patterns:" << std::endl;
    std::cout << textRest << std::endl;
}

void AhoCorasick::printMaxArcs() const{      //max number or arcs from a single vertex
    auto current = vertexs.begin();
    int maxArcs = 0;
    while (current != vertexs.end()){
        if (current->next.size() > maxArcs)
            maxArcs = current->next.size();
        current++;
    }
    std::cout << std::endl << "Max arcs of one vertex prefix tree: " << maxArcs << std::endl << std::endl;
}

int AhoCorasick::getSuffix(int index){
    if (vertexs[index].suffix == -1) {  // if the suffix link has not yet been defined(opredelena)
        if (index == 0 || vertexs[index].prev == 0) {
            vertexs[index].suffix = 0;            // if the root or parent is the root, then the suffix link leads to the root
            std::cout << "Suffix link follow to the root" << std::endl;
        }
        else {
            std::cout << "Start find path by suffix " << std::endl;
            vertexs[index].suffix = getGo(getSuffix(vertexs[index].prev), vertexs[index].prevChar); // else search for another suffix
        }
    }
    
    
    return vertexs[index].suffix;
}

int AhoCorasick::getGo(int index, char ch)                               // get path from current vertex
{
    if (vertexs[index].go.find(ch) == vertexs[index].go.end()) {                // if no symbol from current vertex
        if (vertexs[index].next.find(ch) != vertexs[index].next.end()) {
            vertexs[index].go[ch] = vertexs[index].next[ch];
            std::cout << "\nGo to the vertex " << vertexs[index].go[ch] << " by symbol " << ch << std::endl;
            std::cout <<"Add the transition to the paths of machine" << std::endl;
        }
        else {
            if (index == 0) {
                vertexs[index].go[ch] = 0;
                std::cout << "\nGo to the root " << std::endl;
            }
            else {
                std::cout << "\nFollow the new suffix link and add the transition to the path of machine" << std::endl;
                vertexs[index].go[ch] = getGo(getSuffix(index),ch);
                std::cout << "\nReceived the path to the vertex " << vertexs[index].go[ch] << " through the suffix link" << std::endl;
            }
        }
    }
    else{
        std::cout << "Go to the vertex " << vertexs[index].go[ch] << " by path from vectors of paths" << std::endl;
    }
    
    return vertexs[index].go[ch];
}

void AhoCorasick::printDetails() const {                                             //print details during the course of work
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "Details of the  algorithm" << std::endl << std::endl;
    for (int i=0; i<vertexs.size(); i++){
        std::cout << "Vertex " << i << " with possible path: " << std::endl;
        auto cur = vertexs[i].go.begin();
        while (cur != vertexs[i].go.end()){
            std::cout << "\tVertex " << cur->second << " with path " << cur->first << std::endl;
            cur++;
        }
        std::cout << std::endl;
    }
    std::cout << "-----------------------------------------" << std::endl;
}
