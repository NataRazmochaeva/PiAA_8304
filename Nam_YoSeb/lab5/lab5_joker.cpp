#include <iostream>
#include <string>
#include <vector>
#include <map>

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

int jump(int index, char symb, std::vector<Vertex>& arr_vertex);

void addString(const std::string& str, std::vector<Vertex>& arr_vertex, int& count) {
	std::cout << "Adding string \"" << str << "\" in the bohr" << std::endl;
	int current = 0;

	for (int i = 0; i < str.size(); i++) 
	{
		std::cout << "\tCurrent symbol: \'" << str[i] << "\'\n";
		std::cout << "\tCurrent vertex: " << current << std::endl;
		if (arr_vertex[current].next.find(str[i]) == arr_vertex[current].next.end())     //Если переход по символу не обнаружен и итератор указывает на конец мэпа
		{
			std::cout << "\tWay through \'" << str[i] << "\' wasn't found. Adding new vertex with number " << arr_vertex.size() << std::endl;
			Vertex ver;                                                                 //Создаём новую вершину
			ver.suffix = -1;
			ver.prev = current;
			std::cout << "\t*previous vertex is " << current << ", the symbol of incoming path \'" << str[i] << "\'\n";
			ver.prev_char = str[i];
			arr_vertex.push_back(ver);
			arr_vertex[current].next[str[i]] = arr_vertex.size() - 1;
		}
		else 
			std::cout << "The way through the symbol exist\n";

		std::cout << std::endl;
		current = arr_vertex[current].next[str[i]];      //Переход к следующей вершине
	}

	std::cout << "The number of this pattern is " << count + 1 << std::endl;
	std::cout << "Vertex " << current << " is terminal, deep of the vertex is " << str.size() << "\n\n";

	arr_vertex[current].number = ++count;                //Устанавливаем номер считанного паттерна,
	arr_vertex[current].is_terminal = true;               //Терминальную вершину
	arr_vertex[current].deep = str.size();               //Глубину
}


int getSuffix(int index, std::vector<Vertex>& arr_vertex)    //Функция поиска суффиксной ссылки для вершины index
{
	std::cout << "\t\t\tGetting suffix-link from vertex " << index << std::endl;
	if (arr_vertex[index].suffix == -1)                     //Если суффиксная ссылка ещё не была найдена
	{
		if (index == 0 || arr_vertex[index].prev == 0)      //Если вершина - корень или сын корня
		{
			arr_vertex[index].suffix = 0;
			(index == 0) ? std::cout << "\t\t\tThis is root, suffix-link vertex = 0" << std::endl : std::cout << "\t\t\tThis is a vertex with deep = 1, suffix-link = 0" << std::endl;
		}
		else                                               //Рекурсивный поиск суфф. ссылки. Получаем ссылку родителя и выполняем
		{
			std::cout << "\t\t\tFinding suffix-link from suffix of parent-vertex (" << arr_vertex[index].prev << ") through " << arr_vertex[index].prev_char << std::endl;
			arr_vertex[index].suffix = jump(getSuffix(arr_vertex[index].prev, arr_vertex), arr_vertex[index].prev_char, arr_vertex);
		}                                                   //из неё переход по символу, по которому попали в вершину, для
	}                                                       //которой и ищется суфф. ссылка

	std::cout << "\t\t\tSuffix-link from vertex " << index << " is " << arr_vertex[index].suffix << "\n\n";
	return arr_vertex[index].suffix;
}


int jump(int index, char symb, std::vector<Vertex>& arr_vertex) {     //Функция перехода из вершины index по символу symb. Если прямой переход                                                          
	std::cout << "\t\t\t*Finding the way from " << index << " through \'" << symb << "\'\n"; //невозможен, перейдёт по ссылке

	if (arr_vertex[index].jump.find(symb) == arr_vertex[index].jump.end())             //Если путь ещё не был найден
	{
		if (arr_vertex[index].next.find(symb) != arr_vertex[index].next.end())     //Если найден прямой переход по символу в боре
			arr_vertex[index].jump[symb] = arr_vertex[index].next[symb];              //Добавляем в контейнер возможных переходов
		else                                                                    //Если прямого перехода нет,  получаем суфф. ссылку
		{																		//и ищем переход из суффиксной ссылки по заданному символу		
			if (index == 0)                                                       
				std::cout << "\t\t\t*This is root\n";
			else
				std::cout << "\t\t\t*No straight path. Finding the way from suffix-link of this vertex through \'" << symb << "\'\n";

			arr_vertex[index].jump[symb] = (index == 0 ? 0 : jump(getSuffix(index, arr_vertex), symb, arr_vertex));
		}
	}

	std::cout << "\t\t\t*Found way from " << index << " through \'" << symb << "\' is " << arr_vertex[index].jump[symb] << std::endl;
	return arr_vertex[index].jump[symb];
}


void search(const std::string& text, std::vector<Vertex>& arr_vertex, std::vector<std::pair<int, int>>& res, const std::vector<std::string>& arr_pattern)
{
	std::cout << "Searching begin" << std::endl;
	int curr = 0;

	for (int i = 0; i < text.size(); i++) 
	{
		std::cout << "\tCurrent symbol is \'" << text[i] << "\' from text..." << std::endl;
		std::cout << "\tCurrent vertex is " << curr << std::endl;
		curr = jump(curr, text[i], arr_vertex);
		std::cout << "\tAchieved vertex " << curr << std::endl;
		std::cout << "\tFinding possible entrance with end suffix-links:" << std::endl;

		for (int tmp = curr; tmp != 0; tmp = getSuffix(tmp, arr_vertex)) 
		{
			std::cout << "\t\tCurrent suffix-link vertex: " << tmp << std::endl;
			if (arr_vertex[tmp].is_terminal) 
			{
				res.push_back(std::make_pair(i + 2 - arr_vertex[tmp].deep, arr_vertex[tmp].number));
				std::cout << "\t\tThe vertex is terminal (end suffix-link). The entrance found, index = " <<
					i + 2 - arr_vertex[tmp].deep << " (pattern = \"" << arr_pattern[arr_vertex[tmp].number - 1] << "\")";
			}
			else
				std::cout << "\t\tIt's not terminal vertex, getting suffix-link from this vertex\n\n";
		}

		std::cout << "\t\tRoot is arrived, reading new symbol from the text" << std::endl;
		std::cout << "\t" << LONGLINE;
		std::cout << "\t" << LONGLINE;
	}

	std::cout << LONGLINE;
	std::cout << LONGLINE;
}


void printRes(const std::vector<std::pair<int, int>>& res, const std::vector<std::string>& arr_pattern, const std::string& text, std::string& cutted_text)
{
	std::vector<bool> cut_str(text.size());          //Индексы символов в строке, которые будут вырезаны

	for (int i = 0; i < res.size(); i++) 
	{
		std::cout << res[i].first << " " << res[i].second << std::endl;
		for (int j = 0; j < arr_pattern[res[i].second - 1].size(); j++)
			cut_str[res[i].first - 1 + j] = true;
	}

	for (int i = 0; i < cut_str.size(); i++) 
	{
		if (!cut_str[i])
			cutted_text.push_back(text[i]);            //Сохраняем только неудалённые символы
	}
}

void readPattern(std::vector<Vertex>& arr_vertex, int& count, std::vector<std::string>& arr_pattern)
{
	Vertex root;
	root.prev = -1;
	root.suffix = -1;
	arr_vertex.push_back(root);
	count = 0;

	int pattern_num;
	std::cout << "Enter pattern count:" << std::endl;
	std::cin >> pattern_num;

	for (int i = 0; i < pattern_num; i++)
	{
		std::cout << "Enter pattern for searching in:" << std::endl;
		std::string pattern;
		std::cin >> pattern;
		arr_pattern.push_back(pattern);
		addString(pattern, arr_vertex, count);
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

void printAutomate(std::vector <Vertex> arr_vertex)
{
	std::cout << LONGLINE;
	std::cout << "Total automate:" << std::endl;

	for (int i = 0; i < arr_vertex.size(); i++)
	{
		std::cout << "Connections from vertex " << i << ":\n";
		auto iter = arr_vertex[i].jump.begin();
		for (int j = 0; j < arr_vertex[i].jump.size(); j++)
		{
			std::cout << "\t" << i << "  --" << iter->first << "->  " << iter->second << std::endl;
			iter++;
		}
	}
}

int main() {
	std::cout << LONGLINE;
	std::cout << "Enter text:" << std::endl;
	std::string text, cutted_text;
	std::cin >> text;

	std::vector<Vertex> arr_vertex;
	std::vector<std::string> arr_pattern;
	std::vector<std::pair<int, int>> res;   //<Индекс паттерна в тексте, номер паттерна>
	int count;

	readPattern(arr_vertex, count, arr_pattern);
	search(text, arr_vertex, res, arr_pattern);
	printRes(res, arr_pattern, text, cutted_text);

	std::cout << "Rest string from text after cutting patterns from it: " << cutted_text << std::endl;

	int max_cnt_sons = findMaxSons(arr_vertex);
	std::cout << "Max count of sons: " << max_cnt_sons << "\n\n";


	printAutomate(arr_vertex);

	return 0;
}
