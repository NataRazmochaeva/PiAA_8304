//
//  main.cpp
//  dijkstra_algorithm
//
//  Created by Artem Butko on 12/03/2020.
//  Copyright © 2020 ETU 'LETI'. All rights reserved.
//
 
#include <iostream>
#include <locale>
#include <ctime>
#include <algorithm>
#include <vector>
#include <map>

char startVertex;
char finishVertex;

struct NodeInfo
{
    std::vector<std::pair<char, double>> edges;
    double startLenght = INT_MAX;
    char parentNode;
};

void dijkstra(std::map<char, NodeInfo>& Graph)
{
    std::vector<char> Vertex;
    Vertex.reserve(Graph.size());
    for (auto& i : Graph)
        Vertex.push_back(i.first);

    while (!Vertex.empty())
    {
        char curNode = '0';
        size_t eraseInd = 0;
        double minPriority = -1;

        for(size_t i = 0; i < Vertex.size(); ++i)
        {
            double curPriority = Graph[Vertex[i]].startLenght;
            if (minPriority == -1 || curPriority < minPriority)
            {
                eraseInd = i;
                curNode = Vertex[eraseInd];
                minPriority = curPriority;
            }
        }

        if (minPriority == -1) break;

        Vertex.erase(Vertex.begin() + eraseInd);

        for (auto& childNode : Graph[curNode].edges)
        {
            double oldLenght = Graph[childNode.first].startLenght;
            double newLenght = Graph[curNode].startLenght + childNode.second;
            if (newLenght < oldLenght)
            {
                Graph[childNode.first].startLenght = newLenght;
                Graph[childNode.first].parentNode = curNode;
            }
        }
    }
}

void print(char start, char finish, std::map<char, NodeInfo>& Graph)
{
    std::string answer(1, finish);
    while (true)
    {
        if (answer.back() == start) break;
        answer += Graph[answer.back()].parentNode;
    }

    std::reverse(answer.begin(), answer.end());
    std::cout << answer << std::endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    
    std::map<char, NodeInfo> Graph;

    std::cout << "________АЛГОРИТМ ДЕЙКСТРЫ________" << std::endl;
    std::cout << "Введите начальную вершину : ";
    std::cin >> startVertex;
    std::cout << "Введите конечную вершину :  ";
    std::cin >> finishVertex;

    char from = 0;
    char to = 0;
    double edge = 0;
    
    std::cout << "Ожидается ввод формата <откуда> <куда> <сколько>" << std::endl;
    std::cout << "Ввод закончится control + D" << std::endl;
    std::cout << "_________________________________" << std::endl;
    while(std::cin >> from >> to >> edge)
    {
        if (from == startVertex)
            Graph[from].startLenght = 0;
        Graph[from].edges.emplace_back(to, edge);
    }
    std::cout << "_________________________________" << std::endl;
    auto time = clock();
    dijkstra(Graph);
    std::cout << "Время работы алгоритма : " << (double)(clock() - time) / CLOCKS_PER_SEC << std::endl;
    std::cout << "Сложность алгоритма для графа G(V,E) : O(V * V + E) ~ O(V * V)" << std::endl;
    std::cout << "Оптимальный путь : ";
    print(startVertex, finishVertex, Graph);
}

