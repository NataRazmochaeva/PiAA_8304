#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

/*
 * Примечание по именованию переменных: v -- сокращение от vertex.
 */

/* Структура, хранящая информацию о потоке и весе ребра. */
struct VertexInfo
{
    int weight;
    int flow;
};

/* Структура, хранящая информацию о родителе, назначениях и участии в пути. */
struct Vertex
{
    bool isVisited;
    char vParent;
    std::vector<std::pair<char, VertexInfo>> edges;
};

/* Функция, которая ищет доступную вершину для перехода. */
char vAvailable(std::vector<std::pair<char, VertexInfo>> const& edges,std::map<char, Vertex> Graph)
{
    char vAvailable = 0;
    for(auto vertex : edges)
        if(!Graph[vertex.first].isVisited && vertex.second.weight > 0)
            vAvailable = vertex.first;
    return vAvailable;
}

/* Функция, которая реализует итеративный поиск в глубину. */
bool iterativeDFS(std::map<char, Vertex> Graph, char vSource, char vSink, std::vector<char>& path)
{
    char vCurrent = vSource;
    char vSelected = vAvailable(Graph[vCurrent].edges, Graph);
    while(vCurrent != vSink && !(vCurrent == vSource && vSelected == 0))
    {
        Graph[vCurrent].isVisited = true;

        if(vSelected != 0)
        {
            Graph[vSelected].vParent = vCurrent;
            vCurrent = vSelected;
            path.push_back(vCurrent);
        }
        else
        {
            path.pop_back();
            vCurrent = Graph[vCurrent].vParent;
        }
        vSelected = vAvailable(Graph[vCurrent].edges, Graph);
    }
    return path == std::vector<char>{vSource};
}

/* Функция, которая реализует алгоритм Форда-Фалкерсона. */
void algorithmFordFulkerson(std::map<char, Vertex>& Graph, char vSource, char vSink)
{
    std::vector<char> path{vSource};
    std::vector<char> tmp;
    int minFlow;
    while(!iterativeDFS(Graph, vSource, vSink, path))
    {
        minFlow = INT_MAX;
        tmp = path;

        while(path.size() != 1)
        {
            for(auto vertex : Graph[path[0]].edges)
                if(vertex.first == path[1] && vertex.second.weight < minFlow)
                    minFlow = vertex.second.weight;
            path.erase(path.begin());
        }

        path = tmp;

        while(path.size() != 1)
        {
            for (auto& vParent : Graph[path[0]].edges)
                if(vParent.first == path[1])
                {
                    vParent.second.flow += minFlow;
                    vParent.second.weight -= vParent.second.flow;
                    for(auto& vChild : Graph[path[1]].edges)
                        if(vChild.first == path[0])
                        {
                            vChild.second.flow -= minFlow;
                            vChild.second.weight -= vChild.second.flow;
                        }
                }
            path.erase(path.begin());
        }

        path[0] = vSource;
    }
}

/* Функция, которая реализует сравнение для сортировки. */
bool compare(std::pair<char, VertexInfo> const& a, std::pair<char, VertexInfo> const& b)
{
    return a.first < b.first;
}

/* Функция для вывода результатов. */
void output(std::map<char, Vertex>& Graph, char vSource)
{
    int maxFlow = 0;
    for(auto i : Graph[vSource].edges)
        maxFlow += i.second.flow;
    std::cout << maxFlow <<std::endl;

    for(auto vFrom : Graph)
    {
        std::sort(vFrom.second.edges.begin(), vFrom.second.edges.end(), compare);
        for(auto vTo : vFrom.second.edges)
            std::cout << vFrom.first << " " << vTo.first << " " << std::max(0, vTo.second.flow) << std::endl;
    }
}

int main()
{
    /* Словарь, реализующий сеть. */
    std::map<char, Vertex> Graph;

    char vSource;
    char vSink;
    int vEdges;

    std::cin >> vEdges;
    std::cin >> vSource;
    std::cin >> vSink;

    char vFrom;
    char vTo;
    int vWeight;

    while(vEdges)
    {
        std::cin >> vFrom >> vTo >> vWeight;
        Graph[vFrom].edges.push_back({vTo,{vWeight, 0}});
        vEdges--;
    }

    algorithmFordFulkerson(Graph, vSource, vSink);
    output(Graph, vSource);
    return 0;
}
