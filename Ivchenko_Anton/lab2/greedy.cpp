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
	char id;
	double G;
};


void RECONSTRUCT_PATH(std::map<char, char>& from, char start, char end) {


	std::vector<char> pathset;
	char current = end;
	pathset.push_back(current);

	do {

		current = from[current];
		pathset.push_back(current);


	} while (from[current] != start);

	pathset.push_back(start);

	std::reverse(pathset.begin(), pathset.end());

	for (auto a : pathset)
		cout << a;

	std::cout << "\n";

}

void A(Vertex start, Vertex end, int** m, int n)
{
	vector <Vertex> closedset;
	vector <Vertex> openset;
	std::map<char, char> fromset;

	start.G = 0;
	openset.push_back(start);

	while (openset.size() != 0) {

		Vertex current;

		vector<char> arr_g;
		for (int i = 0; i < openset.size(); i++) {
			arr_g.push_back(openset[i].G);
			
		}
		int index = std::min_element(arr_g.begin(), arr_g.end()) - arr_g.begin();
		current = openset[index];
			

		if (current.id == end.id) {
			RECONSTRUCT_PATH(fromset, start.id, end.id);
			return;

		}

		openset.erase(openset.begin() + index);
		closedset.push_back(current);

		for (int i = 0; i < n; i++) {//прохождение по соседям
			if (m[current.id - 97][i] != 0) {//в матрице смежности

				Vertex* neighbour;

				for (int j = 0; j < closedset.size(); j++)
					if (i + 97 == closedset[j].id)
						continue;

				double tentative_g_score = m[current.id - 97][i];//предв оценка g эл(Жадный)
				int count = 0;

				for (int j = 0; j < openset.size(); j++) {

					if (i + 97 == openset[j].id) {

						neighbour = &openset[j];
						if (tentative_g_score < neighbour->G) {
							neighbour->G = tentative_g_score;
							fromset[i + 97] = current.id;
						}
					}
					else count++;

				}
				if (count == openset.size()) {

					neighbour = new Vertex;
					neighbour->id = i + 97;
					neighbour->G = tentative_g_score;
					fromset[i + 97] = current.id;
					openset.push_back(*neighbour);
				}

			}
		}


	}
}

int main() {

	char start, end, a, b;
	double c;

	std::cin >> start >> end;

	double n = end - 96;

	int** m = new int* [n];
	for (int i = 0; i < n; i++) {
		m[i] = new int[n];
		for (int j = 0; j < n; j++)
			m[i][j] = 0;
	}
	do {

		std::cin >> a >> b >> c;
		m[a - 97][b - 97] = c;

	} while (std::cin);

	Vertex v_start, v_end;
	v_start.id = start;
	v_end.id = end;

	A(v_start, v_end, m, n);

	delete[] m;
}
