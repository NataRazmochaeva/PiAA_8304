#include <iostream>
#include <locale>
#include <ctime>
#include <algorithm>
#include <vector>
#include <map>

struct NodeInfo
{
    /* массив-вектор пар вида <вершина для перехода, вес ребра> */
    std::vector<std::pair<char, double>> edges;
    /* цена пути (изначально равна условной бесконечности) */
    double startLenght = INT_MAX;
    /* имя родительской вершины */
    char parentNode;
};

void dijkstra(std::map<char, NodeInfo>& Graph)
{
    /* Создается массив вершин Vertex, который понадобится для учета обработанных и необработанных вершин графа. */
    std::vector<char> Vertex;
    Vertex.reserve(Graph.size());
    for (auto& i : Graph)
        Vertex.push_back(i.first);
    /* Пока все вершины не обработаны цикл продолжается*/
    while (!Vertex.empty())
    {
        /* Буфферные значения для имени вершины, ее индентификатора*/
        char curNode = '0';
        size_t eraseInd = 0;
        double minPriority = -1;
        /* Ведется поиск вершины (из необработанных) для обработки алгоритмом (релаксации исходящих ребер)
         * с наибольшим приоритетом (наименьшим расстоянием). */
        std::cout << "* поиск необработанной вершины для проведения релаксации *" << std::endl;
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
        std::cout << "* вершина для релаксации -- <" << Vertex[eraseInd] << "> *" << std::endl;
        if (minPriority == -1) break;
        /* Вершина обработана -- убираем ее из массива, что означает, что алгоритм её больше не посетит */
        Vertex.erase(Vertex.begin() + eraseInd);
        /* Релаксация ребер графа -- просматриваются все вершины исходящие из текущей и для каждой такой
         * вершины алгоритм пытается улучшить(т.е. уменьшить) значение длины кратчайшего перехода из на-
         * чальной вершины в выбранную вершину. */
        std::cout << "* релаксация ребер, исходящих из вершины *" << std::endl;
        for (auto& childNode : Graph[curNode].edges)
        {

            double oldLenght = Graph[childNode.first].startLenght;
            double newLenght = Graph[curNode].startLenght + childNode.second;
            std::cout << " ** путь из "<< curNode << " в " << childNode.first << " ** " << std::endl;
            if (newLenght < oldLenght)
            {
                std::cout << " ** успешная релаксация: " << oldLenght << " -> " << newLenght << " ** " << std::endl;
                Graph[childNode.first].startLenght = newLenght;
                Graph[childNode.first].parentNode = curNode;
            }
        }
        std::cout << "* конец релаксации *" << std::endl;
    }
}

void print(char start, char finish, std::map<char, NodeInfo>& Graph)
{
    /* Заполнение строки ответа путем возврата из конечной вершины в начальную по родительским именам. */
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
    /* Создается словарь Graph вида <начальная вершина, структура NodeInfo>> */
    std::map<char, NodeInfo> Graph;

    char startVertex;
    char finishVertex;

    std::cout << "________АЛГОРИТМ ДЕЙКСТРЫ________" << std::endl;
    /* Считывается вершина, из которой начинается поиск пути */
    std::cout << "Введите начальную вершину : ";
    std::cin >> startVertex;
    /* Считывается вершина, в которой заканчивается поиск пути */
    std::cout << "Введите конечную вершину :  ";
    std::cin >> finishVertex;
    char from = 0;
    char to = 0;
    double edge = 0;

    std::cout << "Ожидается ввод формата <откуда> <куда> <сколько>" << std::endl;
    std::cout << "Ввод закончится control + D" << std::endl;
    std::cout << "_________________________________" << std::endl;
    /* Ведется считывание вида:  <начальная вершина> <конечная вершина> <вес ребра> */
    while(std::cin >> from >> to >> edge)
    {
        /* Для начальной вершины стартовый вес равен нулю */
        if (from == startVertex)
            Graph[from].startLenght = 0;
        Graph[from].edges.emplace_back(to, edge);
    }
    std::cout << "_________________________________" << std::endl;

    auto time = clock();
    std::cout << "_________РАБОТА АЛГОРИТМА________" << std::endl;
    dijkstra(Graph);
    std::cout << "_________________________________" << std::endl;
    std::cout << "Время работы алгоритма : " << (double)(clock() - time) / CLOCKS_PER_SEC << std::endl;
    std::cout << "Сложность алгоритма для графа G(V,E) : O(V * V + E) ~ O(V * V)" << std::endl;
    std::cout << "Оптимальный путь : ";
    print(startVertex, finishVertex, Graph);
}
