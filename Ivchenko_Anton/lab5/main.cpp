#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

struct Vertex
{
	std::vector<int> next;
	bool is_leaf = false;
	std::vector<size_t> str_nums;
	std::vector<int> go;
};

Vertex BorVertex(int from, char how)
{
	Vertex vert;
	vert.from = from;
	vert.how = how;

	return vert;
}

void addToBor(std::string& str, std::vector<Vertex>& bor, std::map<char, int>& alphabet, int str_num)
{
	int borInd = 0;
	for (auto c : str)
	{
		char cInd = alphabet[c];

		if (bor[borInd].next[cInd] == -1)
		{
			bor.push_back(BorVertex(borInd, cInd));
			bor[borInd].next[cInd] = bor.size() - 1;
		}

		borInd = bor[borInd].next[cInd];
	}

	bor[borInd].is_leaf = true;
	bor[borInd].str_nums.push_back(str_num);
}

int next(int v, char c, std::vector<Vertex>& bor);

int get_link(int v, std::vector<Vertex>& bor)
{
	if (bor[v].link == -1)
	{
		if (v == 0 || bor[v].from == 0)
			bor[v].link = 0;
		else
			bor[v].link = go(get_link(bor[v].from, bor), bor[v].how, bor);
	}

	return bor[v].link;
}

int next(int v, char c, std::vector<Vertex>& bor)
{
	if (bor[v].go[c] == -1)
	{
		if (bor[v].next[c] != -1)
			bor[v].go[c] = bor[v].next[c];
		else
		{
			if (v == 0)
				bor[v].go[c] = 0;
			else
				bor[v].go[c] = go(get_link(v, bor), c, bor);
		}
	}

	return bor[v].go[c];
}

int main()
{
	std::map<char, int> my_map;
	my_map['A'] = 0;
	my_map['C'] = 1;
	my_map['G'] = 2;
	my_map['T'] = 3;
	my_map['N'] = 4;

	std::string text;
	std::string pattern;
	char J;

	std::cin >> text;
	std::cin >> pattern;
	std::cin >> J;

	pattern += J;

	std::vector<std::string> q;
	std::vector<size_t> l;

	std::string cur;
	for (int i = 0; i < pattern.length(); ++i)
	{
		if (pattern[i] == J)
		{
			if (!cur.empty())
			{
				q.push_back(cur);
				l.push_back(i - cur.size() + 1);
			}
			cur.clear();
		}
		else
			cur += pattern[i];
	}

	std::vector<Vertex> bor;
	bor.push_back(make_bor_vertex(0, 0));

	for (size_t i = 0; i < q.size(); ++i)
		addToBor(q[i], bor, alphabet, i);

	std::vector<size_t> c(text.size());

	int u = 0;
	for (int i = 0; i < text.length(); ++i)
	{
		u = go(u, alphabet[text[i]], bor);
		for (int v = u; v != 0; v = get_link(v, bor))
			if (bor[v].is_leaf)
			{
				for (auto& str_num : bor[v].str_nums)
				{
					int j = i - q[str_num].length() + 1;
					if (j >= l[str_num] - 1)
						++c[j - l[str_num] + 1];
				}
			}
	}


	for (int i = 0; i < text.size(); ++i)
		if (c[i] == q.size())
		{
			if (i + pattern.size() - 1 <= text.size())
				std::cout << i + 1 << "\n";
		}
}

