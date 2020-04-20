#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>

class Element {
public:

    explicit Element(char name, Element *parent) : name(name), parent(parent) {
        sufLink = nullptr;
        childs.resize(0);
        numberString = 0;
        isTerminal = false;
        isFirstElement = false;
    }

    explicit Element() {
        name = '&';
        parent = nullptr;
        sufLink = this;
        childs.resize(0);
        numberString = 0;
        isTerminal = false;
        isFirstElement = true;
    }


private:
    char name;
    std::vector<Element *> childs;


private:
    Element *sufLink;
    Element *parent;
    bool isTerminal;
    bool isFirstElement;
    int numberString;
public:
    void addChild(Element *child) {
        childs.emplace_back(child);
    }

    void setTerminal(int numberString) {
        isTerminal = true;
        Element::numberString = numberString;
    }

    const std::vector<Element *> &getChilds() const {
        return childs;
    }

    char getName() const {
        return name;
    }

    Element *getSufLink() const {
        return sufLink;
    }

    Element *getParent() const {
        return parent;
    }

    void setSufLink(Element *sufLink) {
        Element::sufLink = sufLink;
    }

    bool isFirstElement1() const {
        return isFirstElement;
    }

    bool isTerminal1() const {
        return isTerminal;
    }

    int getNumberString() const {
        return numberString;
    }


};


Element *createElement(Element *parent, std::string *string, int countElement, int numberString) {
    auto *element = new Element(string->at(countElement), parent);
    if (countElement == string->length() - 1) {
        element->setTerminal(numberString);
    }
    parent->addChild(element);
    return element;
}

void addString(Element *top, std::string *string, int numberString) {
    int count = 0;
    Element *cur = top;
    while (count < string->length()) {
        char name = cur->getName();
        bool isFind = false;
        std::vector<Element *> childs = cur->getChilds();
        for (auto child : childs) {
            if (child->getName() == string->at(count)) {
                cur = child;
                count++;
                isFind = true;
                if (count == string->length()) {
                    cur->setTerminal(numberString);
                }
                break;
            }
        }
        if (!isFind) {
            cur = createElement(cur, string, count, numberString);
            count++;
        }
    }
}

bool comp(std::pair<int, int> a, std::pair<int, int> b) {
    if (a.first == b.first) {
        return a.second < b.second;
    } else {
        return a.first < b.first;
    }
}

void init(Element *bor, std::vector<std::string> strings, std::string text,
          std::vector<std::pair<int, int>> *vectorAnswers, std::ofstream &stream) {
    std::queue<Element *> queue;
    for (auto local : bor->getChilds()) {
        local->setSufLink(bor);
        for (auto local2 : local->getChilds()) {
            queue.push(local2);
        }
        stream << local->getName() << " in queue" << std::endl;
    }

    while (!queue.empty()) {
        char name = queue.front()->getName();
        stream << name << " in queue" << std::endl;

        Element *element = queue.front()->getParent();

        bool isExistX = false;

        while (!isExistX) {
            element = element->getSufLink();
            for (auto child : element->getChilds()) {
                if (child->getName() == name) {
                    isExistX = true;
                    element = child;
                    break;
                }
            }
            if (element->isFirstElement1() && !isExistX) {
                isExistX = true;
            }

        }
        queue.front()->setSufLink(element);
        for (auto child : queue.front()->getChilds()) {
            queue.push(child);
        }
        queue.pop();
    }

    int count = 0;
    Element *cur = bor;
    while (count < text.length()) {
        bool isExistX = false;
        for (auto child : cur->getChilds()) {
            if (child->getName() == text[count]) {
                cur = child;
                isExistX = true;
                count++;
                if (cur->isTerminal1()) {
                    //goSuffixLinkTerminal(cur->getSufLink(), &strings, count, &vectorAnswers);
                    if (cur->getChilds().empty()) {
                        vectorAnswers->emplace_back(
                                std::make_pair(count - strings.at(cur->getNumberString()).length() + 1,
                                               cur->getNumberString()));
                    } else {

                        bool isNotFindNextChild = true;

                        for (auto child2 : cur->getChilds()) {
                            if (child2->getName() == text[count]) {
                                isNotFindNextChild = false;
                                break;
                            }
                        }

                        if (isNotFindNextChild) {
                            vectorAnswers->emplace_back(
                                    std::make_pair(count - strings.at(cur->getNumberString()).length() + 1,
                                                   cur->getNumberString()));
                        }
                    }

                }
                break;
            }
        }

        if (!isExistX) {
            if (cur->isFirstElement1()) {
                count++;
            }
            cur = cur->getSufLink();
        }

    }
}

int main() {
    std::string text;
    std::vector<std::string> strings;
    strings.resize(0);
    int countStrings = 0;

    std::ifstream fin;
    fin.open("C:\\Users\\Alex\\Desktop\\in.txt");

    int your_choose = 0;
    std::cout << "If you want to enter data from a file, enter \'1\'\n";
    std::cin >> your_choose;

    if (your_choose == 1) {
        fin >> text;
        fin >> countStrings;
    } else {
        std::cin >> text;
        std::cin >> countStrings;
    }

    for (int i = 0; i < countStrings; ++i) {
        std::string string;
        if (your_choose == 1) {
            fin >> string;
        } else {
            std::cin >> string;
        }
        strings.emplace_back(string);
    }

    fin.close();

    auto *bor = new Element;

    for (int j = 0; j < countStrings; ++j) {
        addString(bor, &strings[j], j);
    }

    std::vector<std::pair<int, int>> vectorAnswers;
    vectorAnswers.resize(0);


    std::ofstream ofstream("C:\\Users\\Alex\\Desktop\\out.txt");
    if (your_choose != 1) {
        ofstream.basic_ios<char>::rdbuf(std::cout.rdbuf());
    }

    init(bor, strings, text, &vectorAnswers, ofstream);

    std::sort(vectorAnswers.begin(), vectorAnswers.end(), comp);

    int endElement = vectorAnswers.size();
    int count = 1;
    while (count<endElement){
        int a1 = vectorAnswers[count].first;
        int a2 = (vectorAnswers[count-1].first + strings[vectorAnswers[count-1].second].length());
        int a3 = vectorAnswers[count-1].first;
        int a4 = strings[vectorAnswers[count-1].second].length();

        if(vectorAnswers[count].first < (vectorAnswers[count-1].first + strings[vectorAnswers[count-1].second].length()) ){
            vectorAnswers.erase(vectorAnswers.begin()+count);
            endElement = vectorAnswers.size();
        } else {
            count++;
        }
    }

   /* for (int k = 1; k < vectorAnswers.size(); ++k) {
        if(vectorAnswers[k].first < (vectorAnswers[k-1].first + strings[vectorAnswers[k-1].second - 1].length()) ){
            vectorAnswers.erase(vectorAnswers.begin()+k);
        }
    }*/

    for (auto answer: vectorAnswers) {
        std::cout << answer.first << " " << answer.second + 1 << std::endl;
    }


    if(your_choose==1){
        for (auto answer: vectorAnswers) {
            ofstream<< answer.first << " " << answer.second + 1 << std::endl;
        }
    }

    ofstream.close();

    return 0;
}