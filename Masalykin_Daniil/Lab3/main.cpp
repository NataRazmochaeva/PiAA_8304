#include <iostream>
#include <map>
#include <cmath>
#include <set>
#include <queue>
#include <fstream>


class FordFalkGraph {
public:
    std::map< char, std::map< char, int > > graph;

    void input_graph(int count) {
        char src;
        char dst;
        int stream;
        std::cin >> src >> dst >> stream;
        for(int i = 0; i < count; i++) {
            graph[src][dst] = stream;
            std::cin >> src >> dst >> stream;
        }
    }

    void input_from_file(int count, char* file){
        std::ifstream input(file);
        char src;
        char dst;
        int stream;
        input >> src >> dst >> stream;
        for(int i = 0; i < count; i++) {
            graph[src][dst] = stream;
            input >> src >> dst >> stream;
        }
        input.close();
    }

    void maxStream(char source, char estuary){
        std::map< char, std::map< char, int > > network = graph;
        std::map< char, std::pair< char, int > > way = BFS(network, source, estuary);

        while( !way.empty() ){
            update_network(network, way, source, estuary);
            way = BFS(network, source, estuary);
        }

        int max_stream = 0;

        for(auto i : graph[source]){
            max_stream += i.second - network[source][i.first];
        }

        std::cout << max_stream << std::endl;

        for(const auto& neighbour : graph)
            for(auto node : neighbour.second){
                std::cout << neighbour.first << " " << node.first << " ";
                if(node.second - network[neighbour.first][node.first] >= 0)
                    std::cout << node.second - network[neighbour.first][node.first] << std::endl;
                else
                    std::cout << 0 << std::endl;
            }
    }
    int get_min_stream(std::map< char, std::pair< char, int > > way, char source, char stock){
        int min_flow = way[stock].second;
        for(char node = stock; node != source; node = way[node].first) {
            if (min_flow > way[node].second)
                min_flow = way[node].second;
        }
        return min_flow;
    }

    //перестройка графа после очередного прохода
    void update_network(std::map< char, std::map< char, int > > &network, std::map< char, std::pair< char, int > > way, char source, char stock){
        int min_flow = get_min_stream(way, source, stock);
        for(char node = stock; node != source; node = way[node].first){
            if(way[node].second - min_flow == 0)
                network[way[node].first].erase(node);
            else
                network[way[node].first][node] -= min_flow;
            network[node][way[node].first] += min_flow;
        }
    }
    //Поиск в ширину
    std::map< char, std::pair< char, int > > BFS(std::map< char, std::map< char, int > > net, char source, char stock){
        std::set<char> visited_nodes;//множество посещенных вершин
        std::queue<char> queue_nodes;//Очередь вершин
        std::map< char, std::pair< char, int > > way;//путь
        std::map< char, std::map< char, int > >::iterator current;//Текущий обрабатывваемый элемент
        visited_nodes.insert(source);
        queue_nodes.push(source);
        while(queue_nodes.front() != stock && !queue_nodes.empty()){
            current = net.find(queue_nodes.front());
            queue_nodes.pop();
            for(auto elem : current->second)
                if(visited_nodes.find(elem.first) == visited_nodes.end()){
                    queue_nodes.push(elem.first);
                    visited_nodes.insert(elem.first);
                    way[elem.first] = std::pair< char, double >(current->first, elem.second);
                }
        }
        if(visited_nodes.find(stock) == visited_nodes.end())
            return std::map<char,std::pair< char, int > >();
        return way;
    }
};

int main(int argc, char* argv[]) {
    int count;
    char source;
    char stock;
    FordFalkGraph graph;
    if(argc == 2){
        std::ifstream input(argv[1]);
        input>>count;
        input>>source;
        input>>stock;
        input.close();
    }
    else {
        std::cin >> count;
        std::cin >> source;
        std::cin >> stock;
        graph.input_graph(count);
    }
    graph.maxStream(source, stock);
    return 0;
}