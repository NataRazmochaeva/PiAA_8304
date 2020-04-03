#include <iostream>
#include <vector>
#include <algorithm>

class Path {
public:
    Path(char nameFrom, char nameOut, int bandwidth) : nameFrom(nameFrom), nameOut(nameOut), bandwidth(bandwidth) {}

    void setFlow(int flow) {
        Path::flow = flow;
    }

    char getNameFrom() const {
        return nameFrom;
    }

    char getNameOut() const {
        return nameOut;
    }

    int getBandwidth() const {
        return bandwidth;
    }

    int getFlow() const {
        return flow;
    }

private:
    char nameFrom;
    char nameOut;
    int bandwidth;
    int flow = 0;
};

void findMin(std::vector<Path *> *local, int *maxFlow) {
    int Min = local->front()->getBandwidth();
    for (Path *path : *local) {
        if (Min > (path->getBandwidth() - path->getFlow())) {
            Min = path->getBandwidth() - path->getFlow();
        }
    }

    for (Path *path : *local) {
        path->setFlow(path->getFlow() + Min);
    }

    *maxFlow = *maxFlow + Min;
}

bool comp(Path a, Path b) {
    if (a.getNameFrom() != b.getNameFrom()) {

        return a.getNameFrom() < b.getNameFrom();
    } else {
        return a.getNameOut() < b.getNameOut();
    }
}

bool comp2(Path *a, Path *b) {
    return (a->getBandwidth() - a->getFlow()) <= (b->getBandwidth() - b->getFlow());
}

bool isVisitedPath(std::vector<Path *> *local, char element){
    for (Path *path : *local){
        if (element == path->getNameFrom()) {
            return false;
        }
    }

    return true;
}

bool findPath(std::vector<Path> *paths, std::vector<Path *> *local, std::vector<Path *> *local2, char myPoint, char *endPoint) {
    if (myPoint == *endPoint) {
        return true;
    }

    std::vector<Path *> localPaths;
    localPaths.reserve(0);
    for (auto &path: *paths) {
        if (path.getNameFrom() == myPoint) {
            char sc = path.getNameFrom();
            localPaths.emplace_back(&path);
        }
    }



    std::sort(localPaths.begin(), localPaths.end(), comp2);



    for (Path *path : localPaths) {
        if (path->getFlow() < path->getBandwidth()) {
            if (isVisitedPath(local2, path->getNameOut())) {
                local2->emplace_back(path);
                if (findPath(paths, local,local2, path->getNameOut(), endPoint)) {
                    local->emplace_back(path);
                    return true;
                } else{
                    local2->pop_back();
                }
            }
        }
    }

    return false;

}

int main() {

    char startPoint, endPoint;

    char start, end;
    int weight;
    int count = 16;

    startPoint = 'a';
    endPoint = 'e';

    std::vector<Path *> local;
    local.reserve(0);

    std::vector<Path *> local2;
    local.reserve(0);

    std::vector<Path> paths;
    paths.reserve(0);

    int maxFlow = 0;



     std::cin >> count;
     std::cin >> startPoint;
     std::cin >> endPoint;
     while (count != 0) {
         std::cin >> start >> end >> weight;
         paths.emplace_back(Path(start, end, weight));
         count--;
     }



    while (findPath(&paths, &local, &local2, startPoint, &endPoint)) {
        findMin(&local, &maxFlow);


        local.clear();
        local2.clear();
    }


    std::cout << maxFlow << '\n';

    std::sort(paths.begin(), paths.end(), comp);

    for (Path path : paths) {
        std::cout << path.getNameFrom() << " " << path.getNameOut() << " " << path.getFlow() << "\n";
    }


    return 0;
}