#include <iostream>
#include <vector>
#include <iterator>
#include <map>
#include <fstream>

/* Алфавит, используемый в автомате. */
const std::map<char, int> Alphabet = {{'A', 0}, {'C', 1}, {'G', 2}, {'T', 3}, {'N', 4}};

/* Переменные для логгирования и ввода/вывода. */
char in, out;
std::string path;
std::ofstream fout;

/* Структура, описывающий каждую вершину бора. */
struct Vertex
{
    std::vector<int> next = {-1, -1, -1, -1, -1};
    std::vector<int> move = {-1, -1, -1, -1, -1};
    bool isTerminate = false;
    int parent = -1;
    int link = -1;
    std::vector<int> sIndexes;
    char symbol;
    explicit Vertex(int p = -1, char ch = 0) : parent(p), symbol(ch){}
};

/* Функция добавления строки в бор. */
void addString(std::string const& string, int strIndex, std::vector<Vertex>& Trie)
{
    int vertex = 0;
    for (char symbol : string)
    {
        int index = Alphabet.at(symbol);
        if (Trie[vertex].next[index] == -1)
        {
            Trie[vertex].next[index] = Trie.size();
            Trie.emplace_back(vertex, symbol);
        }
        vertex = Trie[vertex].next[index];
    }
    Trie[vertex].isTerminate = true;
    Trie[vertex].sIndexes.push_back(strIndex);
}

/* Объявление функции для перехода по состояниям автомата. */
int getMove(int vertex, char symbol, std::vector<Vertex>& Trie);

/* Функция получения суффиксной ссылки. */
int getLink(int vertex, std::vector<Vertex>& Trie)
{
    if (Trie[vertex].link == -1)
    {
        if (vertex == 0 || Trie[vertex].parent == 0) Trie[vertex].link = 0;
        else Trie[vertex].link = getMove(getLink(Trie[vertex].parent, Trie), Trie[vertex].symbol, Trie);
    }
    return Trie[vertex].link;
}

/* Функция перехода по состояниям автомата. */
int getMove(int vertex, char symbol, std::vector<Vertex>& Trie)
{
    int index = Alphabet.at(symbol);
    if (Trie[vertex].move[index] == -1)
    {
        if (Trie[vertex].next[index] != -1) Trie[vertex].move[index] = Trie[vertex].next[index];
        else Trie[vertex].move[index] = vertex == 0 ? 0 : getMove(getLink(vertex, Trie), symbol, Trie);
    }
    return Trie[vertex].move[index];
}

/* Функция вывода размера бора. */
void outputTrieSize(std::vector<Vertex>& Trie)
{
    if (out == 'c') std::cout << "Quantity of vertexes in trie: " << Trie.size() << std::endl;
    else
    {
        fout.open(path);
        fout << "Quantity of vertexes in trie: " << Trie.size() << std::endl;
    }
}

/* Функция вывода позиции паттерна в тексте. */
void outputTempIndex(int index)
{
    if (out == 'c') std::cout << "Pattern has been found at the " << index + 1 << " position." << std::endl;
    else
    {
        fout.open(path);
        fout << "Pattern has been found at the " << index + 1 << " position." << std::endl;
    }
}
void outputUnderline()
{
    if (out == 'c') std::cout << "________________________________________________________" << std::endl;
    else
    {
        fout.open(path);
        fout << "________________________________________________________"  << std::endl;
    }
}

/* Функция поиска и вывода пересечения подстрок с паттерном в тексте. */
void searchIntersections(int indexString, const std::string& text, const std::string& string, int size, std::vector<int>& tempCounter)
{
    for(int index = indexString + 1; index < indexString + string.size() - 1; ++index)
        if(tempCounter[index - string.size() + 1] == size)
        {
            if(out == 'c')
                std::cout << "Intersection with string at the position " << index - string.size() + 2 << " - |"
                          << std::string(text.begin() + indexString, text.begin() + indexString + string.size() - 1) << "#"
                          << std::string(text.begin() + index - string.size() + 1, text.begin() + index) << "|;" << std::endl;
            else
            {
                fout.open(path);
                fout << "Intersection with string at the position " << index - string.size() + 2 << " - |"
                     << std::string(text.begin() + indexString, text.begin() + indexString + string.size() - 1) << "#"
                     << std::string(text.begin() + index - string.size() + 1, text.begin() + index) << "|;" << std::endl;
            }
        }
}

/* Функция проверки вершины на конечность, подсчет индексов в счетчик. */
void check(int vertex, int index, std::string& text, std::string& string,std::vector<Vertex>& Trie, std::vector<std::pair<std::string, int>>& substring, std::vector<int>& tempCounter)
{
    int indexString = 0;
    for(int current = vertex; current != 0; current = getLink(current, Trie))
    {
        if(!Trie[current].isTerminate) continue;
        for (auto sIndex : Trie[current].sIndexes)
        {
            indexString += index + 2;
            indexString -= substring[sIndex - 1].first.length() + substring[sIndex - 1].second;
            if (indexString > -1) ++tempCounter[indexString];
            if (tempCounter[indexString] == substring.size())
            {
                outputTempIndex(indexString);
                searchIntersections(indexString, text, string,  substring.size(), tempCounter);
                outputUnderline();
            }
        }
    }

}

/* Функция поиска всех вхождений. */
void findAllPositions(std::string& text, std::string& string, std::vector<std::pair<std::string, int>>& substring, std::vector<Vertex>& Trie)
{
    int vertex = 0;
    std::vector<int> tempCounter(text.size(), 0);
    for(int index = 0; index < text.size(); ++index)
    {
        /* Получение номера вершины в боре. */
        vertex = getMove(vertex, text[index], Trie);
        check(vertex, index, text, string, Trie, substring, tempCounter);
    }
}

/* Подготовка вектора подстрок и строк для выполнения поиска.*/
void preparing(std::string& string, std::vector<Vertex>& Trie, std::vector<std::pair<std::string, int>>& substring, char joker)
{
    string += joker;
    std::string temp;
    for (int i = 0; i < string.size(); ++i)
    {
        if (string[i] == joker)
        {
            if(!temp.empty())
            {
                substring.emplace_back(temp, i + 1 - temp.size());
                temp.clear();
            }
        }
        else temp += string[i];
    }
    for (int index = 0; index < substring.size(); ++index)
        addString(substring[index].first, index + 1, Trie);
}

/* Функция ввода данных в программу. */
bool input()
{
    std::string text;
    std::string string;
    char joker = 0;

    std::cout << "CHOOSE WHERE W/R DATA:" << std::endl;
    std::cout << "c - console;" << std::endl;
    std::cout << "f - file." << std::endl;
    std::cout << "READ DATA FROM" << std::endl;
    std::cin >> in;
    std::cout << "WRITE DATA TO" << std::endl;
    std::cin >> out;

    if((in != 'c' && in != 'f') || (out != 'c' && out != 'f'))
    {
        std::cout << "ERROR: WRONG INPUT";
        return false;
    }

    if(in == 'c') std::cin >> text >> string >> joker;
    else if(in == 'f')
    {
        getline(std::cin, path);
        std::ifstream fin(path);
        fin >> text >> string >> joker;
    }

    /* Бор, представленный как вектор вершин бора. */
    std::vector<Vertex> Trie(1);
    /* Вектор подстрок. */
    std::vector<std::pair<std::string, int>> substring;
    /* Инициализация вектора подстрок и подготовка остальных строк. */
    preparing(string, Trie, substring, joker);
    /* Вывод размера бора. */
    outputTrieSize(Trie);
    outputUnderline();
    /* Поиск всех вхождений. */
    findAllPositions(text, string, substring, Trie);
    return true;
}

int main()
{
    if(!input()) return 1;
}