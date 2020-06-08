#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

std::map<char, int> alphabet = { {'A', 1},{'C', 2},{'G', 3},{'T', 4}, {'N', 5} };
std::map <std::string, int> pattern;

struct Vertex {
	
	bool root = false;
	bool leaf = false;
	std::vector<char> suffix;
	std::map<char, Vertex*> next;
	std::vector<Vertex*> next_dfs;
	Vertex* auto_move = nullptr;
	Vertex* suff_link = nullptr;
	Vertex* parent = nullptr;

};
Vertex* automatic(Vertex* a, char ch);

Vertex* setSuffixLink(Vertex *cur) {

	if (cur->root) return cur;

	if (cur->suff_link == nullptr) {
		if (cur->parent->root)
			cur->suff_link = cur->parent;
		else
			cur->suff_link = automatic(setSuffixLink(cur->parent), cur->suffix.back());
	}
	return cur->suff_link;
}

std::map<Vertex*, bool> used;

void dfs(Vertex* a) {
	used[a] = 1;

	for (auto i : a->next_dfs) {
		if (!used[i]) {
			//a->suff_link = setSuffixLink(a);
			dfs(i);
		}
	}
}

Vertex* makeTrieVertex(Vertex* par, char a) {

	Vertex* vrtx = new Vertex;
	vrtx->parent = par;
	vrtx->suffix = par->suffix;
	vrtx->suffix.push_back(a);
	return vrtx;
}

void addToTrie(std::string& sample, Vertex* root) {

	Vertex* ptr = root;
	for (int i = 0; i < sample.length(); i++) {

		if (alphabet[sample[i]]) {
			
			if (!ptr->next[sample[i]]) {
				Vertex* nextVertex = makeTrieVertex(ptr, sample[i]);
				ptr->next[sample[i]] = nextVertex;
				ptr->next_dfs.push_back(nextVertex);
				ptr = nextVertex;
			}
			else ptr = ptr->next[sample[i]];

		}
		else std::cout << "Incorrect";

	}ptr->leaf = true;
}

Vertex* automatic(Vertex *a, char ch) {


	if (a->auto_move == nullptr || a->root) {
		if (a->next[ch]) {
			a->auto_move = a->next[ch];
			return a->auto_move;
		}
		else {
			if (a->root)
				a->auto_move = a;
			else
				a->auto_move = automatic(setSuffixLink(a), ch);
		}
	}
	return a->auto_move;
}

Vertex* disjoint(Vertex* a, char ch, Vertex* root) {


	if (a->auto_move == nullptr || a->root) {
		if (a->next[ch]) {
			a->auto_move = a->next[ch];
			return a->auto_move;
		}
		else {
			if (a->root)
				a->auto_move = a;
			else
				a->auto_move = disjoint(root, ch, root);//ссылка в корень
		}
	}
	return a->auto_move;
}
void check(Vertex* a, int i, std::vector<int>& res) {

	if(a != nullptr) {

		if (a->leaf) {
			std::string s(a->suffix.begin(), a->suffix.end());
			res.push_back(i - s.length() + 1);
			res.push_back(pattern[s] + 1);
		}
	}
}


void AhoCorasik(std::string& s, Vertex* root, std::ostream& out) {

	std::vector<int> result;
	Vertex* u = root;
	
	for (int i = 0; i < s.length() ; i++) {
		
		if (alphabet[s[i]]) {

			//u = disjoint(u, s[i], root);
			u = automatic(u, s[i]);
			check(u, i + 1, result);
		
		}
		
	}
	for (int a = 0; a < result.size(); a += 2) {
		out << result[a] << ' ' << result[a + 1] << std::endl;
	}
}
int main(){

	int n;
	std::vector<int> result;
	std::string text;

	int input_ch, output_ch;

	std::cout << "Input: console = 0; file = 1\n";
	std::cin >> input_ch;

	Vertex* root = new Vertex;
	root->root = true;

	std::istream* input;

	if (input_ch == 1) {
		
		std::ifstream infile("input.txt");
		input = &infile;

	}
	else
		input = &std::cin;


	*input >> text;
	*input >> n;

	for (int i = 0; i < n; i++) {
		std::string str;
		*input >> str;
		pattern[str] = i;
		addToTrie(str, root);
	}

	std::cout << "Output:\nconsole = 0; file =  1" << std::endl;
	std::cin >> output_ch;

	dfs(root);//устанавливаем суффиксные ссылки в боре методом обхода в глубину

	if (output_ch == 0) {
		AhoCorasik(text, root, std::cout);
	}
	else if (output_ch == 1) {
		std::ofstream file;
		file.open("result.txt");
		if (!file.is_open()) {
			std::cout << "Incorrect!\n";
			return 0;
		}
		AhoCorasik(text, root, file);
	}
	
	return 0;
}
