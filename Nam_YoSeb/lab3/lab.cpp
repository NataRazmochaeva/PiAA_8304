#include <iostream>
#include <algorithm>
#include <vector>


class Neighbour {
public:
	char vertex;
	int start_flow;
	int flow;
	int final_flow;

	Neighbour(char vertex, int start_flow) : vertex(vertex), start_flow(start_flow), flow(start_flow) {}
	Neighbour(char vertex) : vertex(vertex) {}
};

class Vertex {
public:
	char vertex;
	char prev_vertex;
	std::vector<Neighbour> neighbours;

	Vertex() {}
	Vertex(char vertex) : vertex(vertex) {}
};

bool isExist(std::vector<Vertex> vector, char vertex) {
	for (int i = 0; i < vector.size(); ++i)
		if (vector[i].vertex == vertex)
			return true;

	return false;
}

int findVertex(std::vector<Vertex> vector, char vertex) {
	for (int i = 0; i < vector.size(); i++)
		if (vertex == vector[i].vertex)
			return i;

	return -1;
}

int findNeighbour(Vertex vertex, char neighbour) {
	if (vertex.neighbours.empty())
		return -1;

	for (int i = 0; i < vertex.neighbours.size(); ++i) 
		if (vertex.neighbours[i].vertex == neighbour)
			return i;

	return -1;
}

int maxFlowCount(std::vector<Vertex> vector_of_vertex, Vertex stock) {		// Данная функция считает максимальный поток в сквозном пути
	int flow, min_flow, index_prev, index_neighbour;
	std::vector<int> flows;
	Vertex current = stock;		// Текущая вершина - сток

	while (current.prev_vertex != '0')														// Пока не дошли до истока,
	{																						// находим вершину предыдущую текущей
		index_prev = findVertex(vector_of_vertex, current.prev_vertex);						// в сквозном пути, а в ней величину потока
		index_neighbour = findNeighbour(vector_of_vertex[index_prev], current.vertex);		// до текущей вершины. Добавляем найденное
		flow = vector_of_vertex[index_prev].neighbours[index_neighbour].flow;				// значение в вектор с потоками. Текущей
		flows.push_back(flow);																// вершиной становится предыдущая в
		current = vector_of_vertex[index_prev];												// сквозном пути.
	}																						

	min_flow = flows[0];

	for (int i = 1; i < flows.size(); ++i)		// Находим минимальный
		if (flows[i] < min_flow)				// из потоков сквозного пути
			min_flow = flows[i];

	return min_flow;		// Возвращаем максимальный поток сквозного пути
}

void recountFlow(std::vector<Vertex>& vector_of_vertex, Vertex stock, int max_flow) {		// Данная функция производит пересчет
	int index_current, index_prev, index_neighbour_current, index_neighbour_prev;			// потока по сквозному пути
	Vertex current = stock;		// Текущая вершина - сток

	while (current.prev_vertex != '0')																		// Пока не дошли до истока,
	{																										// находим текущую и предыдущую
		index_current = findVertex(vector_of_vertex, current.vertex);										// текущей вершины, а в них ребра
		index_prev = findVertex(vector_of_vertex, current.prev_vertex);										// друг до друга.
		index_neighbour_current = findNeighbour(vector_of_vertex[index_current], current.prev_vertex);		
		index_neighbour_prev = findNeighbour(vector_of_vertex[index_prev], current.vertex);

		if (index_neighbour_current == -1)											// Если из текущей вершины нет ребра
		{																			// до предыдущей в сквозном пути
			Neighbour neighbour = Neighbour(current.prev_vertex);					// вершины, то оно создается и
			neighbour.flow = max_flow;												// его пропускная способность
			neighbour.start_flow = 0;												// инициализируется максимальным
			vector_of_vertex[index_current].neighbours.push_back(neighbour);		
		}
		else																							// Иначе пропускная способность
			vector_of_vertex[index_current].neighbours[index_neighbour_current].flow += max_flow;		// увеличивается на величину максимального
																										// потока данного свозного пути
		vector_of_vertex[index_prev].neighbours[index_neighbour_prev].flow -= max_flow;		// Пропускная способность из предыдущей в сквозном
		current = vector_of_vertex[index_prev];												// пути вершины до текущей уменьшается на величину
	}
}

bool cmpVertex(const Vertex& a, const Vertex& b) {
	if (a.vertex < b.vertex)
		return true;
	else
		return false;
}

bool cmpNeighbours(const Neighbour& a, const Neighbour& b) {
	if (a.vertex < b.vertex)
		return true;
	else
		return false;
}


int main() {
	int count, start_flow, max_flow, index;
	int Pmax = 0;
	char source, stock, start, end;
	std::vector<Vertex> vector_of_vertex, visited_vertex;

	std::cin >> count >> source >> stock;

	for (int i = 0; i < count; ++i)		// Создается структура графа
	{		
		std::cin >> start >> end >> start_flow;
		Vertex first;
		Vertex second;

		if (!isExist(vector_of_vertex, start)) 
		{															// Если начальной вершины ещё нет
			first = Vertex(start);									// в векторе вершин, то создаём её,
			Neighbour neighbour = Neighbour(end, start_flow);		// её соседа(конечную вершину) и
			first.neighbours.push_back(neighbour);					// добавляем соседа в вектор соседей.
			vector_of_vertex.push_back(first);						// Затем добавляем вершину в вектор вершин
		}
		else 
		{																	// Иначе если начальная вершина уже есть
			Neighbour neighbour = Neighbour(end, start_flow);				// в векторе вершин, то создаём её соседа,
			index = findVertex(vector_of_vertex, start);					// находим начальную вершину в векторе вершин
			vector_of_vertex[index].neighbours.push_back(neighbour);		// и добавляем соседа в вектор соседей найденной
		}																	// вершины.

		if (!isExist(vector_of_vertex, end))		// Если конечной вершины нет в векторе вершин,
		{											// то она создается и добавляется в вектор вершин
			second = Vertex(end);					
			vector_of_vertex.push_back(second);
		}
	}

	index = findVertex(vector_of_vertex, source);
	vector_of_vertex[index].prev_vertex = '0';
	Vertex current = vector_of_vertex[index];		// Текущая вершина - исток
	visited_vertex.push_back(current);

	while (1) {
		int index_min_priority = -1;
		int priority, min_priority;
		bool near_start;

		for (int i = 0; i < current.neighbours.size(); i++)			// Поиск соседа текущей вершины
		{																											
			if (current.neighbours[i].flow > 0 && !isExist(visited_vertex, current.neighbours[i].vertex))		// Если пропускная способность пути до соседа
			{																									// больше нуля и сосед не находится в векторе
				priority = abs(current.neighbours[i].vertex - current.vertex);									// посещенных вершин, то рассчитывается приоритет	

				if (index_min_priority == -1 || priority < min_priority)		// Ищется сосед с минимальным приоритетом и в случае
				{																// равенства приоритетов, выбирается сосед
					min_priority = priority;									// имя которого в алфавите ближайшее к началу алфавита.
					index_min_priority = i;										
																				
					if (current.neighbours[i].vertex < current.vertex)		// Так как при одинаковом приоритете соседи могут находиться
						near_start = true;									// либо до, либо после на одинаковом "расстоянии" от текущей
					else													// вершины, то переменная near_start отвечает за это положение.
						near_start = false;                                
				}
				else if (priority == min_priority) 
					if (current.neighbours[i].vertex < current.vertex && near_start == false) 
					{
						index_min_priority = i;
						near_start = true;
					}
			}
		}

		if (index_min_priority != -1)																	// Если сосед был найден
		{																								// он находится в векторе
			index = findVertex(vector_of_vertex, current.neighbours[index_min_priority].vertex);		// вершин и предыдущей вершиной
			vector_of_vertex[index].prev_vertex = current.vertex;										// к нему указывается текущая
			current = vector_of_vertex[index];															
			visited_vertex.push_back(current);

			if (current.vertex == stock)												// Если на данной итерации дошли до стока
			{																			// рассчитывается максимальный поток,
				max_flow = maxFlowCount(vector_of_vertex, current);						// производится пересчет пропускных способностей пути,
				recountFlow(vector_of_vertex, vector_of_vertex[index], max_flow);		// рассчитывается итоговый максимальный поток
				Pmax += max_flow;														
				visited_vertex.clear();
				index = findVertex(vector_of_vertex, source);							// Текущей вершиной становится исток
				current = vector_of_vertex[index];
				visited_vertex.push_back(current);
			}
		}
		else		// Если сосед не был найден
		{		
			if (current.prev_vertex == '0')		// Если текущая вершина - исток, то больше сквозных путей не построить.
				break;		// Переход к формированию решения
			else		// Иначе, откат к предыдущей вершине
			{				
				index = findVertex(vector_of_vertex, current.prev_vertex);
				current = vector_of_vertex[index];
			}
		}
	}

	for (int i = 0; i < vector_of_vertex.size(); ++i)		// Рассчитываются конечные потоки через все ребра в графе
		for (int j = 0; j < vector_of_vertex[i].neighbours.size(); ++j)
			vector_of_vertex[i].neighbours[j].final_flow = vector_of_vertex[i].neighbours[j].start_flow - vector_of_vertex[i].neighbours[j].flow;

	sort(vector_of_vertex.begin(), vector_of_vertex.end(), cmpVertex);
	// Сортировка графа
	for (int i = 0; i < vector_of_vertex.size(); ++i) 
		sort(vector_of_vertex[i].neighbours.begin(), vector_of_vertex[i].neighbours.end(), cmpNeighbours);

	// Вывод результата
	std::cout << Pmax << std::endl;

	for (int i = 0; i < vector_of_vertex.size(); ++i) 
		for (int j = 0; j < vector_of_vertex[i].neighbours.size(); ++j) 
			if (vector_of_vertex[i].neighbours[j].start_flow != 0)		// Если ребро было задано изначально(а не создано во время алгоритма)
			{		
				std::cout << vector_of_vertex[i].vertex << " " << vector_of_vertex[i].neighbours[j].vertex << " ";

				if (vector_of_vertex[i].neighbours[j].final_flow >= 0)
					std::cout << vector_of_vertex[i].neighbours[j].final_flow;
				else
					std::cout << 0;

				std::cout << std::endl;
			}
	return 0;
}