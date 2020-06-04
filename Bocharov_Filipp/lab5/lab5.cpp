#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>


std::map<char, int> alphabet;
const int k = 5;
std::vector<int> pos;

struct bohr_vrtx {
	bohr_vrtx(int p, char c) {
		this->par = p;
		this->symb = c;
	}
	int next_vrtx[k] = { -1,-1,-1,-1,-1 };
	std::vector<int> pat_num;
	int suff_link = -1;
	int auto_move[k] = { -1,-1,-1,-1,-1 };
	int par = -1;
	int suff_flink = -1;
	bool flag = false;
	char symb = 0;
};

std::vector<bohr_vrtx> bohr = { bohr_vrtx(-1, 0) };
std::vector<std::string> pattern;

int get_move(int v, char ch);

int get_suf_link(int v) {
	if (bohr[v].suff_link == -1)
		if (v == 0 || bohr[v].par == 0)
			bohr[v].suff_link = 0;
		else
			bohr[v].suff_link = get_move(get_suf_link(bohr[v].par), bohr[v].symb);
	return bohr[v].suff_link;
}

int get_move(int cur, char ch) {// перехода по состояниям автомата
	if (bohr[cur].auto_move[ch] == -1)
		if (bohr[cur].next_vrtx[ch] != -1)//если есть ребро
			bohr[cur].auto_move[ch] = bohr[cur].next_vrtx[ch];//то пройдем по нему
		else
			if (cur == 0)
				bohr[cur].auto_move[ch] = 0;
			else//иначе идем по суфф ссылке
				bohr[cur].auto_move[ch] = get_move(get_suf_link(cur), ch);
	return bohr[cur].auto_move[ch];
}

int get_suf_flink(int v) {//вычисление хорошей суфф ссылки
	if (bohr[v].suff_flink == -1) {
		int u = get_suf_link(v);
		if (u == 0)
			bohr[v].suff_flink = 0;
		else
			bohr[v].suff_flink = (bohr[u].flag) ? u : get_suf_flink(u);
	}
	return bohr[v].suff_flink;
}

void check(int v, int i, std::vector<int>& C, std::ostream& output) {//хождение по хорошим суфф ссылкам
	for (int u = v; u != 0; u = get_suf_flink(u)) {
		if (bohr[u].flag) {
			for (int k : bohr[u].pat_num) {
				int j = i - pattern[k].size() + 1;
				int che = j - pos[k] + 1;
				if (che > 0 && che < C.size()) { //проверка выхода за рамки
					++C[che];//увеличиваем значение
				}
			}
		}
	}
}

void add_str_to_bohr(std::string& s) {//добавление строки в бор
	int num = 0;
	for (char i : s) {//проходим по всей строке
		char ch = alphabet[i];//получаем номер элемента в алфавите
		if (bohr[num].next_vrtx[ch] == -1) {//если нет ребра
			bohr.push_back(bohr_vrtx(num, ch));//добавляем его
			bohr[num].next_vrtx[ch] = bohr.size() - 1;
		}
		num = bohr[num].next_vrtx[ch];
	}
	bohr[num].flag = true;
	pattern.push_back(s);
	bohr[num].pat_num.push_back(pattern.size() - 1);
}

void find_all(std::istream& input, std::ostream& output) {//функция поиска
	std::string T;
	std::string temp;
	std::string cur;
	char joker ;
	input >> T >> temp >> joker ;
	for (int i = 0; i < temp.size(); ++i) {//разделение образца на куски
		if (temp[i] == joker) {
			if (cur.empty())
				continue;
			else {
				output << "Found substring: " << cur << std::endl;
				add_str_to_bohr(cur);// добавляем в бор
				pos.push_back(i - cur.size());// записываем индекс
				cur = "";
				continue;
			}
		}
		else if (i == temp.size() - 1) {
			cur += temp[i];// добавляем в бор
			output << "Found substring: " << cur << std::endl;
			add_str_to_bohr(cur);// оставшийся кусок
			pos.push_back(i - cur.size() + 1);
		}
		cur += temp[i];
	}
	output << std::endl;
	int v = 0;
	std::vector<int> F(T.size());//вектор для хранения количества подстрок без джокера
	output << "State changes:" << std::endl;
	for (int i = 0; i < T.length(); i++) {
		output << "From " << v;
		v = get_move(v, alphabet[T[i]]);
		output << " to " << v << std::endl;
		check(v, i, F, output);
	}
	std::vector<int> inter;//вектор для кранения ответов
	for (int k = 0; k < F.size(); ++k)
		if (F[k] == pos.size()) {
			if (k + temp.size() - 1 <= T.size()) {
				output << std::endl;
				output << "Found result at " << k << " position" << std::endl
					<< "Result is \"" << T.substr(k - 1, temp.size()) << "\"" << std::endl;
				for (int h : inter) {
					if ((k - h) < temp.size()) {
						output << "\"" << T.substr(h - 1, temp.size()) << "\""
							<< " from pos = " << h << " has intersection with " << "\""
							<< T.substr(k - 1, temp.size()) << "\"" << " from pos = "
							<< k << std::endl;//то значит есть пересечение
					}
				}
				inter.push_back(k);//заносим ответ в вектор
			}
		}
	output << std::endl;
	output << "Coincidences was found at: ";//вывод индексов
	for (int res : inter)//совпадений
		output << res << " ";
	output << std::endl << std::endl;
	output << "Number of vertices: " << bohr.size() << std::endl;//количество вершин
}

int main() {
	alphabet['A'] = 0;//инициализация алфавита
	alphabet['C'] = 1;
	alphabet['G'] = 2;
	alphabet['T'] = 3;
	alphabet['N'] = 4;

		find_all(std::cin, std::cout);
				
	return 0;
}