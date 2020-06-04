#include "joker.hpp"

Joker::Joker(char joker): matchPatterns(1100000){
        VertexStruct root;
        root.prev = -1;
        vertexs.push_back(root);
        this->joker = joker;
        countTerminalVertex = 0;
    }

    void Joker::readPattern(std::string& str){
        std::cout << "--------------------------------------------" << std::endl;
        std::cout << "Read pattern processing" << std::endl;
        std:: cout << "Read pattern: " << str << std::endl;
        patternLen = str.size();
        split(str);                         // splitting the patttern in respect to joker

        std::cout << "Subpattern and his index in patterns: " << std::endl;
        for (int i = 0; i < patternArr.size(); i++){
            std::cout << patternArr[i] << " with index " << patternsLength[i] + 1 << std::endl;
        }
        std::cout  << "--------------------------------------------";
        std::cout << std::endl << std::endl <<"Add subpatterns in prefix tree " << std::endl << std::endl << std::endl;

        for (const auto& pattern : patternArr) {
            addPattern(pattern);
        }


    }

    void Joker::search(const std::string& str)
    {
        std::cout << std::endl << "\nSearch for patterns in the line is started" << std::endl << std::endl << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        int curr = 0;
        bool terminalVertexFound;
        for (int i = 0; i < str.size(); i++) {
            std::cout << "Search for symbols " << str[i] << std::endl;
            curr = getGo(curr, str[i]);              // for each character, we move to a new vertex
            std::cout << "\nStart check terminal vertex and check terminal vertex by suffix link" << std::endl;
            terminalVertexFound = false;
            for (int tmp = curr; tmp != 0; tmp = getSuffix(tmp)) {                if (vertexs[tmp].isLeaf) {
                    for (int j = 0; j < vertexs[tmp].number.size(); j++) {
                        if (i + 1 - patternsLength[vertexs[tmp].number[j] - 1] - vertexs[tmp].deep >= 0 &&
                            i + 1 - patternsLength[vertexs[tmp].number[j] - 1] - vertexs[tmp].deep <= str.size() - patternLen){   //if it's not out of range
                            matchPatterns[i + 1 - patternsLength[vertexs[tmp].number[j] - 1] - vertexs[tmp].deep]++;                // add mateches
                            terminalVertexFound = true;
                            std::cout << "Terminal vertex found, found pattern  " << patternArr[vertexs[tmp].number[j] - 1]  << "  with index in text " << i - vertexs[tmp].deep + 2  << std::endl;
                            std::cout << "Count subpatterns in this index " << matchPatterns[i + 1 - patternsLength[vertexs[tmp].number[j] - 1] - vertexs[tmp].deep] << " of " << patternsLength.size() << std::endl;
                            break;
                        }
                    }
                }
            }
            if (!terminalVertexFound){
                std::cout << "Terminal vertex was not found, go to the next symbol" << std::endl;
            }
            std::cout << "-----------------------------------------" << std::endl;
        }

    }

    void Joker::printResult(const std::string& text) const{
        printDetails();
        printMaxArcs();
        std::vector<bool> cutStr(text.size());
        std::string textRest;
        std::cout << "The result of algorithm work:" <<std::endl;
        for (int i = 0; i < matchPatterns.size(); i++) {
            if (matchPatterns[i] == patternsLength.size()) {
                std::cout << i + 1 << "\n";
                for (int j = 0; j < patternLen; j++)
                    cutStr[i + j] = true;
            }
        }
        for (int i = 0; i < cutStr.size(); i++){
            if (!cutStr[i])
                textRest.push_back(text[i]);
        }

        std::cout << std::endl << "String without found patterns:" << std::endl;
        std::cout << textRest << std::endl;
    }


    void Joker::printMaxArcs() const{
        auto current = vertexs.begin();
        int maxArcs = 0;
        while (current != vertexs.end()){
            if (current->next.size() > maxArcs)
                maxArcs = current->next.size();
            current++;
        }
        std::cout << std::endl << "Max arcs of one vertex prefix tree: " << maxArcs << std::endl << std::endl;
    }

    void Joker::split(std::string str){
        std::string buf = "";
        for (int i=0; i<str.size(); i++){
            if (str[i] == joker){
                if (!buf.empty()) {
                    patternArr.push_back(buf);
                    patternsLength.push_back(i - buf.size());
                    buf = "";
                }
            }
            else {
                buf.push_back(str[i]);
                if (i == str.size() - 1){
                    patternArr.push_back(buf);
                    patternsLength.push_back(i - buf.size() + 1);
                }
            }
        }
    }

    void Joker::addPattern(const std::string& str)
    {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Add symbols of new pattern in prefix tree" << std::endl;
        int current = 0;
        for (char i : str) {
            std::cout << "\tReceived character " << i << std::endl;
            if (vertexs[current].next.find(i) == vertexs[current].next.end()) {
                VertexStruct ver;
                ver.suffix = -1;
                ver.prev = current;
                ver.prevChar = i;
                vertexs.push_back(ver);
                vertexs[current].next[i] = vertexs.size() - 1;
                std::cout << "\tAdd new vertex in prefix tree with number " << vertexs.size() - 1 << std::endl;
                std::cout << "\tSymbol path to the vertex " << i << std::endl;
            }
            current = vertexs[current].next[i];
            std::cout << "\tTransition in vertex by symbol " << i << std::endl << std::endl;
        }
        countTerminalVertex++;
        vertexs[current].number.push_back(countTerminalVertex);
        vertexs[current].isLeaf = true;
        vertexs[current].deep = str.size();
        std::cout << "New terminal vertex is " << vertexs[current].prevChar << std::endl;
        std::cout << "Deep of terminal vertex " << vertexs[current].deep << std::endl;
        std::cout << "-----------------------------------------" << std::endl;

    }

    int Joker::getSuffix(int index)
    {
        if (vertexs[index].suffix == -1) {
            if (index == 0 || vertexs[index].prev == 0) {
                vertexs[index].suffix = 0;
                std::cout << "Suffix link follow to the root" << std::endl;
            }
            else {
                std::cout << "Start find path by suffix " << std::endl;
                vertexs[index].suffix = getGo(getSuffix(vertexs[index].prev), vertexs[index].prevChar);
            }
        }
        return vertexs[index].suffix;
    }

    int Joker::getGo(int index, char ch)
    {
        if (vertexs[index].go.find(ch) == vertexs[index].go.end()) {
            if (vertexs[index].next.find(ch) != vertexs[index].next.end()) {
                vertexs[index].go[ch] = vertexs[index].next[ch];
                std::cout << "\nGo to the vertex " << vertexs[index].go[ch] << " by symbol " << ch << std::endl;
                std::cout <<"Add the transition to the paths of machine" << std::endl;
            }
            else {
               if (index == 0){
                   vertexs[index].go[ch] = 0;
                   std::cout << "\nGo to the root " << std::endl;
               }
               else{
                   std::cout << "\nFollow the new suffix link and add the transition to the path of machine" << std::endl;
                   vertexs[index].go[ch] = getGo(getSuffix(index), ch);
                   std::cout << "\nReceived the path to the vertex " << vertexs[index].go[ch] << " through the suffix link" << std::endl;
               }
            }
        }
        else{
            std::cout << "Go to the vertex " << vertexs[index].go[ch] << " by path from vectors of paths" << std::endl;
        }
        return vertexs[index].go[ch];
    }

    void Joker::printDetails() const {             
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Details of the algorithm" << std::endl << std::endl;
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
