#include <iostream>
#include <vector>
#include <ctime>
#include <vector>
#include <math.h>

using std::vector;

class Square
{
public:
	size_t x;
	size_t y;
	size_t length;

	Square();
	Square(const Square& copy);
	~Square();

	void print();
	void print(size_t mlp);
};

Square::Square() {
	x = 0;
	y = 0;
	length = 0;
}

Square::Square(const Square& copy) {
	x = copy.x;
	y = copy.y;
	length = copy.length;
}

Square::~Square() {

}

void Square::print() {
	std::cout << x << " " << y << " " << length << std::endl;
}

void Square::print(size_t mlp) {
	std::cout << x * mlp << " " << y * mlp << " " << length * mlp << std::endl;
}


class Table {
public:
	Table(size_t size_);
	Table(Table& copy);
	~Table();

	Table& operator=(const Table& copy);

	Table& shareTable();

	Table& rewrite(Square p);
	Table& rewriteShare();
	Table& remove(Square point);
	Table& printTable();

	void print_result();

	bool can_insert(int x, int y, int lenght);
	bool chek();

	size_t primal_size(size_t size);

private:
	int** table;
	int** result_share;
	int mlp;

	size_t size;
	size_t count;
	size_t result_count;
	vector <Square> current_share;
	vector <Square> result;
};




Table::Table(size_t size_) {
	size = primal_size(size_);
	mlp = size_ / size;
	table = new int* [size];
	result_share = new int* [size];
	for (int i = 0; i < size; i++)
	{
		table[i] = new int[size];
		result_share[i] = new int[size];
		for (int j = 0; j < size; j++)
		{
			table[i][j] = 0;
			result_share[i][j] = 0;
		}
	}
	count = 0;
	result_count = size*size;
}

Table::Table(Table& copy) {
	size = copy.size;
	table = new int* [size];
	result_share = new int* [size];
	mlp = copy.mlp;
	for (int i = 0; i < size; i++) {
		table[i] = new int[size];
		result_share[i] = new int [size];
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			table[i][j] = table[i][j];
			result_share[i][j] = copy.result_share[i][j];
		}
	}
	count = copy.count;
	result_count = copy.result_count;
}

Table::~Table() {
	for (int i = 0; i < size; i++) {
		delete table[i];
		delete result_share[i];
	}
	delete table;
	delete result_share;
}

Table& Table::operator=(const Table& copy) {
	size = copy.size;
	table = new int* [size];
	result_share = new int* [size];
	for (int i = 0; i < size; i++) {
		table[i] = new int[size];
		result_share[i] = new int[size];
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			table[i][j] = table[i][j];
			result_share[i][j] = copy.result_share[i][j];
		}
	}
	count = copy.count;
	result_count = copy.result_count;

	return *this;
}

Table& Table::rewrite(Square p) {
	for (int i = p.y; i < p.y + p.length; i++)
	{
		for (int j = p.x; j < p.x + p.length; j++)
		{
			table[i][j] = count + 1;
		}
	}
	count++;
	return *this;
}

Table& Table::rewriteShare() {
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			result_share[i][j] = table[i][j];
		}
	}
	return *this;
}

Table& Table::printTable() {
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			std::cout << table[i][j] << " ";
		}
		std::cout << std::endl;
	}
	return *this;
}


bool Table::chek() {
	size_t count = 0;
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			if (table[i][j] == 0) return false;
		}
	}
	return true;
}

Table& Table::remove(Square square) {
	for (int i = square.y; i < square.y + square.length; i++)
	{
		for (int j = square.x; j < square.x + square.length; j++)
		{
			table[i][j] = 0;
		}
	}
	count--;
	return *this;
}

bool Table::can_insert(int x, int y, int lenght) {
	if (x >= size || y >= size || x + lenght > size || y + lenght > size)
		return false;
	for (int i = y; i < y + lenght; i++)
	{
		for (int j = x; j < x + lenght; j++)
		{
			if (table[i][j] != 0)
			{
				return false;
			}
		}
	}
	return true;
}

size_t Table::primal_size(size_t size_){
	for (size_t i = 2; i <= sqrt(size_); i++)
	{
		if (size_ % i == 0)
			return i;
	}
	return size_;
}

Table& Table::shareTable()
{
	Square square;
	square.length = size / 2 + size % 2;
	current_share.push_back(square);
	rewrite(square);

	square.length = size / 2;
	square.x = size / 2 + size % 2;
	current_share.push_back(square);
	rewrite(square);

	square.x = 0;
	square.y = size / 2 + size % 2;
	current_share.push_back(square);
	rewrite(square);

	do
	{
		while (count < result_count && !chek())
		{
			for (size_t i = 0; i < size; i++)
			{
				for (size_t j = 0; j < size; j++)
				{
					if (table[i][j] == 0)
					{
						for (int len = size - 1; len > 0; len--)
						{
							if (can_insert(j, i, len))
							{
								square.x = j;
								square.y = i;
								square.length = len;
								break;
							}

						}
						rewrite(square);
						current_share.push_back(square);
					}
				}
			}
		}

		if (result_count > count || result_count == 4)
		{
			std::cout << "Update result vector, becouse number of squares less than previous value." << std::endl << "Now result table is:" << std::endl;
			printTable();
			result_count = count;
			rewriteShare();
			result = current_share;
		}
		while (!current_share.empty() && current_share[current_share.size() - 1].length == 1)
		{
			remove(current_share[current_share.size() - 1]);
			current_share.pop_back();
		}
		if (!(current_share.empty()))
		{
			square = current_share[current_share.size() - 1];
			current_share.pop_back();
			remove(square);
			square.length -= 1;
			rewrite(square);
			current_share.push_back(square);
		}


	} while (count < result_count * 3 && !(current_share.empty()));

	return *this;
}

void Table::print_result()
{
	std::cout << result_count <<  std::endl;
	for(int i = 0; i < result.size(); i++)
	{
		result[i].print(mlp);
	}
}

int main() {
	size_t size;

	std::cout << "Please enter size of square size: ";
	std::cin >> size;

	Table t(size);
	
	t.shareTable();
	std::cout << std::endl << "Result is:" << std::endl;
	t.print_result();
	return 0;
}