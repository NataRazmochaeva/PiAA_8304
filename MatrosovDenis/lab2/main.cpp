#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <fstream>

using namespace std;

struct Rib
{
    char begin;
    char end;
    double weight;

    friend std::istream& operator>> (std::istream& in, Rib& point);
};


class Graph
{
private:
    vector <Rib> graph;
    vector <char> res;
    vector <char> path;
    char src;
    char dst;

public:

    Graph(){}

    void input();

    bool is_visible(char value);

    bool Search(char value);

    void init_search();

    void Print();
};

void Graph::input() {
    cin >> src >> dst;
    Rib r;
    while (cin >> r)
    {
        graph.push_back(r);
    }
}

bool Graph::is_visible(char value)
{
    for (char i : path)
        if (i == value)
            return true;
    return false;
}

bool Graph::Search(char value)
{
    if (src != dst) {
        if (value == dst)
        {
            res.push_back(value);
            return true;
        }

        path.push_back(value);

        for (auto& i : graph)
        {
            if (value == i.begin)
            {
                if (is_visible(i.end))
                    continue;
                res.push_back(i.begin);
                bool flag = Search(i.end);
                if (flag)
                    return true;
                res.pop_back();
            }
        }
    }
    return false;
}

void Graph::init_search() {
    Search(src);
}

void Graph::Print()
{
    for (auto i : res)
        cout << i;
}

std::istream& operator>> (std::istream& in, Rib& r)
{
    in >> r.begin;
    in >> r.end;
    in >> r.weight;
    return in;
}

int main()
{
    Graph g;
    g.input();
    g.init_search();
    g.Print();
    return 0;
}





struct Step
{
    string path;
    double length;
    char estuary;
};

class Graph {
private:
    vector<Rib> graph;
    vector<Step> res;
    vector<char> curr;
    char source;
    char estuary;

public:
    Graph() {};

    void input() {
        char tmp;
        Rib elem;
        cin >> source >> estuary;
        while (cin >> elem) {
            graph.push_back(elem);
        }
        string buf = "";
        buf += source;
        for (auto& i : graph) {
            if (i.begin == source) {
                buf += i.end;
                res.push_back({ buf, i.weight });
                res.back().estuary = estuary;
                buf.resize(1);
            }
        }
        curr.push_back(source);
    }

    size_t min_elem()
    {
        double min;
        min = DBL_MAX;
        size_t temp = -1;
        for (size_t i(0); i < res.size(); i++) {
            if (res.at(i).length + abs(estuary - res.at(i).path.back()) < min) {
                if (is_visible(res.at(i).path.back())) {
                    res.erase(res.begin() + i);
                }
                else {
                    min = res.at(i).length + abs(estuary - res.at(i).path.back());
                    temp = i;
                }
            }
        }
        return temp;
    }

    bool is_visible(char value)
    {
        for (char i : curr) {
            if (i == value) {
                return true;
            }
        }
        return false;
    }

    void Search() {
        sort(res.begin(), res.end(), [](const Step& a, const Step& b) -> bool {
            return a.length + a.estuary - a.path.back() > b.length + b.estuary - b.path.back();
            });
        while (true) {
            size_t min = min_elem();
            if (min == -1) {
                break;
            }
            if (res.at(min).path.back() == estuary) {
                cout << res.at(min).path;
                return;
            }
            for (auto& i : graph) {
                if (i.begin == res.at(min).path.back()) {
                    string buf = res.at(min).path;
                    buf += i.end;
                    res.push_back({ buf, i.weight + res.at(min).length });
                }
            }
            curr.push_back(res.at(min).path.back());
            res.erase(res.begin() + min);
        }
    }
};



int main(int argc, char* argv[])
{
    Graph element;   
    element.input();
    element.Search();
    return 0;
}