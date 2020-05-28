#include <iostream>
#include <vector>
#include <algorithm>

class Path {
public:
    Path(char nameFrom, char nameOut, int bandwidth) : nameFrom(nameFrom), nameOut(nameOut), width(bandwidth) {}
    void setFlow(int flow_) {
        flow = flow_;
    }
    char GetSourceTop() const {
        return nameFrom;
    }
    char GetDrainTop() const {
        return nameOut;
    }
    int GetWidth() const {
        return width;
    }
    int GetFlow() const {
        return flow;
    }
private:
    char nameFrom;
    char nameOut;
    int width;
    int flow = 0;
};

void findMin(std::vector<Path*>* local, int* maxFlow) {
    int Min = local->front()->GetWidth();
    for (Path* path : *local) {
        if (Min > (path->GetWidth() - path->GetFlow())) {
            Min = path->GetWidth() - path->GetFlow();
        }
    }
    for (Path* path : *local) {
        path->setFlow(path->GetFlow() + Min);
    }
    *maxFlow = *maxFlow + Min;
}

bool SortByAlphabet(Path a, Path b) {
    if (a.GetSourceTop() != b.GetSourceTop())
        return a.GetSourceTop() < b.GetSourceTop();
    else
        return a.GetDrainTop() < b.GetDrainTop();
}

bool SortByWidght(Path* a, Path* b) {
    return (a->GetWidth() - a->GetFlow()) <= (b->GetWidth() - b->GetFlow());
}

bool isVisitedPath(std::vector<Path*>* local, char element) {
    for (Path* path : *local) {
        if (element == path->GetSourceTop()) {
            return false;
        }
    }
    return true;
}

bool findPath(std::vector<Path>* paths, std::vector<Path*>* local, std::vector<Path*>* local2, char myPoint, char* endPoint) {
    if (myPoint == *endPoint) {
        return true;
    }

    std::vector<Path*> localPaths;
    localPaths.reserve(0);
    for (auto& path : *paths) {
        if (path.GetSourceTop() == myPoint) {
            char sc = path.GetSourceTop();
            localPaths.emplace_back(&path);
        }
    }

    std::sort(localPaths.begin(), localPaths.end(), SortByWidght);

    for (Path* path : localPaths) {
        if (path->GetFlow() < path->GetWidth()) {
            if (isVisitedPath(local2, path->GetDrainTop())) {
                local2->emplace_back(path);
                if (findPath(paths, local, local2, path->GetDrainTop(), endPoint)) {
                    local->emplace_back(path);
                    return true;
                }
                else {
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
    int count, weight;
    int maxFlow = 0;

    std::vector<Path*> local;
    std::vector<Path*> local2;
    std::vector<Path> paths;

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

    std::sort(paths.begin(), paths.end(), SortByAlphabet);

    for (Path path : paths) {
        std::cout << path.GetSourceTop() << " " << path.GetDrainTop() << " " << path.GetFlow() << "\n";
    }
    return 0;
}