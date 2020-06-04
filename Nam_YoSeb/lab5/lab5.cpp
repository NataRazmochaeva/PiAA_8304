#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

#define LONGLINE "-------------------------------------------------------------------------------------\n"

struct Vertex {
	std::map<char, int> next;   //Контейнер переходов по символу char в вершину с номером int
	std::map<char, int> jump;     //массив переходов (запоминаем переходы в ленивой рекурсии), используемый для вычисления суффиксных ссылок
	bool is_terminal = false;    //Является ли терминальной (конечной) вершиной (на которой заканчивается паттерн)
	int prev;                   //Номер предыдущей вершины (родителя)
	char prev_char;              //Символ, по которому пришли в вершину
	int suffix;                 //Суффиксная ссылка
	int number;                 //Какой по счёту считанный паттерн заканчивается на этой вершине (только для вывода)
	int deep;                   //глубина в боре, равная длине строки-паттерна, заканчивающегося в этой терминальной вершине
};

int jump(int index, char symb, std::vector<Vertex>& arr_vertex, std::ostream& out);

void addString(const std::string& str, std::vector<Vertex>& arr_vertex, int& count, std::ostream& out) {
	out << "Adding string \"" << str << "\" in the bohr" << std::endl;
	int current = 0;

	for (int i = 0; i < str.size(); i++)
	{
		out << "\tCurrent symbol: \'" << str[i] << "\'\n";
		out << "\tCurrent vertex: " << current << std::endl;
		if (arr_vertex[current].next.find(str[i]) == arr_vertex[current].next.end())     //Если переход по символу не обнаружен и итератор указывает на конец мэпа
		{
			out << "\tWay through \'" << str[i] << "\' wasn't found. Adding new vertex with number " << arr_vertex.size() << std::endl;
			Vertex ver;                                                                 //Создаём новую вершину
			ver.suffix = -1;
			ver.prev = current;
			out << "\t*previous vertex is " << current << ", the symbol of incoming path \'" << str[i] << "\'\n";
			ver.prev_char = str[i];
			arr_vertex.push_back(ver);
			arr_vertex[current].next[str[i]] = arr_vertex.size() - 1;
		}
		else
			out << "The way through the symbol exist\n";

		out << std::endl;
		current = arr_vertex[current].next[str[i]];      //Переход к следующей вершине
	}

	out << "The number of this pattern is " << count + 1 << std::endl;
	out << "Vertex " << current << " is terminal, deep of the vertex is " << str.size() << "\n\n";

	arr_vertex[current].number = ++count;                //Устанавливаем номер считанного паттерна,
	arr_vertex[current].is_terminal = true;               //Терминальную вершину
	arr_vertex[current].deep = str.size();               //Глубину
}


int getSuffix(int index, std::vector<Vertex>& arr_vertex, std::ostream& out)    //Функция поиска суффиксной ссылки для вершины index
{
	out << "\t\t\tGetting suffix-link from vertex " << index << std::endl;
	if (arr_vertex[index].suffix == -1)                     //Если суффиксная ссылка ещё не была найдена
	{
		if (index == 0 || arr_vertex[index].prev == 0)      //Если вершина - корень или сын корня
		{
			arr_vertex[index].suffix = 0;
			(index == 0) ? out << "\t\t\tThis is root, suffix-link vertex = 0" << std::endl : out << "\t\t\tThis is a vertex with deep = 1, suffix-link = 0" << std::endl;
		}
		else                                               //Рекурсивный поиск суфф. ссылки. Получаем ссылку родителя и выполняем
		{
			out << "\t\t\tFinding suffix-link from suffix of parent-vertex (" << arr_vertex[index].prev << ") through " << arr_vertex[index].prev_char << std::endl;
			arr_vertex[index].suffix = jump(getSuffix(arr_vertex[index].prev, arr_vertex, out), arr_vertex[index].prev_char, arr_vertex, out);
		}                                                   //из неё переход по символу, по которому попали в вершину, для
	}                                                       //которой и ищется суфф. ссылка

	out << "\t\t\tSuffix-link from vertex " << index << " is " << arr_vertex[index].suffix << "\n\n";
	return arr_vertex[index].suffix;
}


int jump(int index, char symb, std::vector<Vertex>& arr_vertex, std::ostream& out) {     //Функция перехода из вершины index по символу symb. Если прямой переход                                                          
	out << "\t\t\t*Finding the way from " << index << " through \'" << symb << "\'\n"; //невозможен, перейдёт по ссылке

	if (arr_vertex[index].jump.find(symb) == arr_vertex[index].jump.end())             //Если путь ещё не был найден
	{
		if (arr_vertex[index].next.find(symb) != arr_vertex[index].next.end())     //Если найден прямой переход по символу в боре
			arr_vertex[index].jump[symb] = arr_vertex[index].next[symb];              //Добавляем в контейнер возможных переходов
		else                                                                    //Если прямого перехода нет,  получаем суфф. ссылку
		{																		//и ищем переход из суффиксной ссылки по заданному символу		
			if (index == 0)
				out << "\t\t\t*This is root\n";
			else
				out << "\t\t\t*No straight path. Finding the way from suffix-link of this vertex through \'" << symb << "\'\n";

			arr_vertex[index].jump[symb] = (index == 0 ? 0 : jump(getSuffix(index, arr_vertex, out), symb, arr_vertex, out));
		}
	}

	out << "\t\t\t*Found way from " << index << " through \'" << symb << "\' is " << arr_vertex[index].jump[symb] << std::endl;
	return arr_vertex[index].jump[symb];
}


void search(const std::string& text, std::vector<Vertex>& arr_vertex, std::vector<std::pair<int, int>>& res, const std::vector<std::string>& arr_pattern, std::ostream& out)
{
	out << "Searching begin" << std::endl;
	int curr = 0;

	for (int i = 0; i < text.size(); i++)
	{
		out << "\tCurrent symbol is \'" << text[i] << "\' from text..." << std::endl;
		out << "\tCurrent vertex is " << curr << std::endl;
		curr = jump(curr, text[i], arr_vertex, out);
		out << "\tAchieved vertex " << curr << std::endl;
		out << "\tFinding possible entrance with end suffix-links:" << std::endl;

		for (int tmp = curr; tmp != 0; tmp = getSuffix(tmp, arr_vertex, out))
		{
			out << "\t\tCurrent suffix-link vertex: " << tmp << std::endl;
			if (arr_vertex[tmp].is_terminal)
			{
				res.push_back(std::make_pair(i + 2 - arr_vertex[tmp].deep, arr_vertex[tmp].number));
				out << "\t\tThe vertex is terminal (end suffix-link). The entrance found, index = " <<
					i + 2 - arr_vertex[tmp].deep << " (pattern = \"" << arr_pattern[arr_vertex[tmp].number - 1] << "\")";
			}
			else
				out << "\t\tIt's not terminal vertex, getting suffix-link from this vertex\n\n";
		}

		out << "\t\tRoot is arrived, reading new symbol from the text" << std::endl;
		out << "\t" << LONGLINE;
		out << "\t" << LONGLINE;
	}

	out << LONGLINE;
	out << LONGLINE;
}


void printRes(const std::vector<std::pair<int, int>>& res, const std::vector<std::string>& arr_pattern, const std::string& text, std::string& cutted_text, std::ostream& out)
{
	std::vector<bool> cut_str(text.size());          //Индексы символов в строке, которые будут вырезаны

	for (int i = 0; i < res.size(); i++)
	{
		out << res[i].first << " " << res[i].second << std::endl;
		for (int j = 0; j < arr_pattern[res[i].second - 1].size(); j++)
			cut_str[res[i].first - 1 + j] = true;
	}

	for (int i = 0; i < cut_str.size(); i++)
	{
		if (!cut_str[i])
			cutted_text.push_back(text[i]);            //Сохраняем только неудалённые символы
	}
}

void readPattern(std::vector<Vertex>& arr_vertex, int& count, std::vector<std::string>& arr_pattern, std::istream& in, std::ostream& out)
{
	Vertex root;
	root.prev = -1;
	root.suffix = -1;
	arr_vertex.push_back(root);
	count = 0;

	int pattern_num;
	out << "Enter pattern count:" << std::endl;
	in >> pattern_num;

	for (int i = 0; i < pattern_num; i++)
	{
		out << "Enter pattern for searching in:" << std::endl;
		std::string pattern;
		in >> pattern;
		arr_pattern.push_back(pattern);
		addString(pattern, arr_vertex, count, out);
	}
}

//Функция поиска максимального числа исходящих дуг из одной вершины бора
int findMaxSons(std::vector<Vertex> arr_vertex)
{
	int max = arr_vertex[0].next.size();

	for (int i = 1; i < arr_vertex.size(); i++)
	{
		if (arr_vertex[i].next.size() > max)
			max = arr_vertex[i].next.size();
	}
	return max;
}

void printAutomate(std::vector <Vertex> arr_vertex, std::ostream& out)
{
	out << LONGLINE;
	out << "Total automate:" << std::endl;

	for (int i = 0; i < arr_vertex.size(); i++)
	{
		out << "Connections from vertex " << i << ":\n";
		auto iter = arr_vertex[i].jump.begin();
		for (int j = 0; j < arr_vertex[i].jump.size(); j++)
		{
			out << "\t" << i << "  --" << iter->first << "->  " << iter->second << std::endl;
			iter++;
		}
	}
}


void dialog(std::istream& in, std::ostream& out) {
	out << LONGLINE;
	out << "Enter text:" << std::endl;
	std::string text, cutted_text;
	in >> text;

	std::vector<Vertex> arr_vertex;
	std::vector<std::string> arr_pattern;
	std::vector<std::pair<int, int>> res;   //<Индекс паттерна в тексте, номер паттерна>
	int count;

	readPattern(arr_vertex, count, arr_pattern, in, out);
	search(text, arr_vertex, res, arr_pattern, out);
	printRes(res, arr_pattern, text, cutted_text, out);

	out << "Rest string from text after cutting patterns from it: " << cutted_text << std::endl;

	int max_cnt_sons = findMaxSons(arr_vertex);
	out << "Max count of sons: " << max_cnt_sons << "\n\n";


	printAutomate(arr_vertex, out);
}

int main() {
	int choise;
	std::cout << "Read from: 1 - file, 2 - console" << std::endl;
	std::cin >> choise;
	
	if (choise == 1)
	{
		std::ifstream in("input.txt");
		std::ofstream out("output.txt");
		dialog(in, out);
	}
	else if (choise == 2)
		dialog(std::cin, std::cout);
	else
		std::cout << "durak" << std::endl;

	return 0;
}
