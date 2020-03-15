import copy
from queue import Queue
from operator import itemgetter


class Graph:
    def __init__(self, start_position, end_position, graph):
        self._start_position = start_position
        self._end_position = end_position
        self._graph = graph

        self._residual_throughput = copy.deepcopy(self._graph)
        for vertex in list(self._graph.keys()):
            for value in self._residual_throughput[vertex]:
                if value in self._residual_throughput:
                    if vertex in self._residual_throughput[value]:
                        continue
                    else:
                        self._residual_throughput[value].update({vertex: 0})
                else:
                    self._residual_throughput[value] = {vertex: 0}

        self.max_flow = 0
        self._came_from = {}
        self.edges_weight = []

    def path_exists(self):
        queue = Queue()
        queue.put(self._start_position)
        visited = {self._start_position: True}

        while not queue.empty():
            current_elem = queue.get()
            if current_elem == self._end_position:
                return True

            for neighbour in sorted(list(self._residual_throughput[current_elem].keys())):
                if self._residual_throughput[current_elem][neighbour] > 0 and neighbour not in visited:
                    queue.put(neighbour)
                    visited[neighbour] = True
                    self._came_from[neighbour] = current_elem

        return False

    def ford_falkerson(self):
        while self.path_exists():
            path = self._end_position
            while path[0] != self._start_position:
                path = self._came_from[path[0]] + path

            print(f"current path: {path}")

            min_flow = float('inf')
            for i in range(len(path) - 1):
                min_flow = min(min_flow, self._residual_throughput[path[i]][path[i + 1]])

            for i in range(len(path) - 1):
                print(f"\told width {path[i]} --> {path[i+1]}: {self._residual_throughput[path[i]][path[i + 1]]}")
                self._residual_throughput[path[i]][path[i + 1]] -= min_flow
                print(f"\tnew width {path[i]} --> {path[i + 1]}: {self._residual_throughput[path[i]][path[i + 1]]}")
                print(f"\told width {path[i]} <-- {path[i + 1]}: {self._residual_throughput[path[i + 1]][path[i]]}")
                self._residual_throughput[path[i + 1]][path[i]] += min_flow
                print(f"\tnew width {path[i]} <-- {path[i + 1]}: {self._residual_throughput[path[i + 1]][path[i]]}")
            print()
            self.max_flow += min_flow

        for vertex in self._graph:
            for dest_vertex in self._graph[vertex]:
                if self._graph[vertex][dest_vertex] - self._residual_throughput[vertex][dest_vertex] < 0:
                    self.edges_weight.append((vertex, dest_vertex, 0))
                else:
                    self.edges_weight.append((vertex, dest_vertex,
                                              self._graph[vertex][dest_vertex] - self._residual_throughput[vertex][
                                                  dest_vertex]))

        self.edges_weight.sort(key=itemgetter(0, 1))


if __name__ == "__main__":
    N = int(input())
    start_position = input()
    end_position = input()
    graph = {}

    for i in range(N):
        vertex1, vertex2, weight = input().split()
        if vertex1 in graph:
            graph[vertex1].update({vertex2: int(weight)})
        else:
            graph[vertex1] = {vertex2: int(weight)}

    graph = Graph(start_position, end_position, graph)
    graph.ford_falkerson()

    print(graph.max_flow)
    for i in graph.edges_weight:
        print(i[0], i[1], i[2])
