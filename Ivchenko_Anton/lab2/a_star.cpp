#include<iostream>
#include <string>
#include<vector>
#include<cstdlib>
#include<math.h>
#include<map>
#include <algorithm>
#include <stack>

using namespace std;

struct Vertex {
	int id;
	double G, F;
};


void RECONSTRUCT_PATH(std::map<char, char>& from, char start, char end){

	
	std::vector<char> pathset;
	char current = end;
	pathset.push_back(current);

	do{

		current = from[current];
		pathset.push_back(current);
	
		
	} while (from[current] != start);
	
	pathset.push_back(start);

	std::reverse(pathset.begin(), pathset.end());

	for (auto a : pathset)
		cout << a;

	std::cout << "\n";
	
}
void tentative_is_better(Vertex *neighbour, int t, std::map<char, int>& cmp) {//обновление характеристик соседней вершины

	neighbour->G = t;
	neighbour->F = neighbour->G + cmp[neighbour->id];
	//cout << neighbour->F;

}

void A(Vertex start, Vertex end, int **m, int n, std::map<int, int> &cmp)
{
	vector <Vertex> closedset;
	vector <Vertex> openset;
	std::map<char, char> fromset;

	start.G = 0;
	start.F = start.G + cmp[start.id];
	openset.push_back(start);

	while (openset.size() != 0) {

		Vertex current;
		
		int index = 0;
		int min_F = 1000;
		for (int i = 0; i < openset.size(); i++) {// нахождение элемента openset с мин. оценкой F;
			if (min_F > openset[i].F)
				min_F = openset[i].F;
		}
		for (int i = openset.size() - 1; i >= 0; i--) {
			if (min_F == openset[i].F) {
				index = i;
				break;
			}
		}

		current = openset[index];
		
		
		if (current.id == end.id) {
				RECONSTRUCT_PATH(fromset, start.id, end.id);
				return;

		}

		closedset.push_back(openset[index]);
		openset.erase(openset.begin() + index);

		for (int i = 0; i < n; i++) {//прохождение по соседям
			if (m[current.id - 97][i] != 0) {//в матрице смежности

				Vertex *neighbour;
				bool in_cs = 0;

				for (int j = 0; j < closedset.size(); j++)
					if (i + 97 == closedset[j].id)
						in_cs = 1;

				if(in_cs) continue;

				int tentative_g_score = m[current.id - 97][i] + current.G;//предв оценка g эл (A*)

				int count = 0;

				for (int j = 0; j < openset.size(); j++) {

					if (i + 97 == openset[j].id) {

						neighbour = &openset[j];
						if (tentative_g_score < neighbour->G) {

							fromset[i + 97] = current.id;
							neighbour->G = tentative_g_score;
							neighbour->F = tentative_g_score + cmp[i + 97];

						}
						else break;
					}
					else count++;

				}
				if(count == openset.size()) {
						
						fromset[i + 97] = current.id;
						neighbour = new Vertex;

						neighbour->id = i + 97;

						neighbour->G = tentative_g_score;
						neighbour->F = tentative_g_score + cmp[i+97];

						openset.push_back(*neighbour);
					
					}

			}
		}


	}
}
 
int main() {

	char start, end, a, b;
	double c, h;
	
	std::map<int,int> cmp;

	std::cin >> start >> end;
	double n = end - 96;

	int** m = new int*[n];
	for (int i = 0; i < n; i++) {
		m[i] = new int[n];
		for (int j = 0; j < n; j++)
			m[i][j] = 0;
	}
	do {

		std::cin >> a >> b >> c >> h;
		m[a - 97][b - 97] = c;
		int m = static_cast<int>(a);
		cmp[m] = h;
	
	}
	while (std::cin);

	Vertex v_start, v_end;

	v_start.id = static_cast<int>(start);
	v_end.id = static_cast<int>(end);

	A(v_start, v_end, m, n, cmp);

	delete[] m;
}
