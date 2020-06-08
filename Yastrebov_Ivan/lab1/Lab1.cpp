#include "pch.h"
#include <iostream>
#include <vector>

//size, n - таблицы и подтаблицы
int size = 1;
int n;
int cnt = 0;

//Стрктура, описывающая хар-ки квадратов для в вставки
struct element {
	int x;
	int y;
	int len;
};

//Функция ввода данных
bool input(std::vector<int>& squares) {
	std::cout << "Insert square's side length\n";
	std::cin >> n;

	int quantity;
	std::cout << "Insert number of necessary squares\n";
	std::cin >> quantity;
	if (quantity < 0 || quantity > n*n) {
		std::cout << "Impossible number of squares\n";
		return false;
	}

	int side_l;

	if (quantity != 0) {
		std::cout << "Insert " << quantity << " side's length\n";
		for (int i = 0; i < quantity; ++i) {
			std::cin >> side_l;
			if (side_l > 0 && side_l <= n - 1) {
				squares.push_back(side_l);
			}
		}
	}
	return true;
}


//Функция проверки частного разбиения
bool check_variant(std::vector<int> squares, std::vector<element> tmp) {

	if (squares.empty())
		return true;

	for (int i = 0; i < tmp.size(); ++i) {
		for (int j = 0; j < squares.size(); ++j) {
			if (tmp[i].len == squares[j]) {
				tmp.erase(tmp.begin() + i);
				i -= 1;
				squares.erase(squares.begin() + j);
				break;
			}
		}
	}
	return squares.empty();
}

// Функция вывода промежуточного решения - вызывается каждый раз, когда исходный квадрат оказывается полностью заполнен
// Разные квадраты обозначаются разными числами в таблице
void print_particular_answer(std::vector<element>& tmp) {
	std::vector<std::vector<int>> view_field(n, std::vector<int>(n, 0));
	for (int i = 0; i < tmp.size(); ++i) {

		int tmp_x = tmp[i].x, tmp_y = tmp[i].y, tmp_s = tmp[i].len;

		for (int y = tmp_y; y < tmp_y + tmp_s; ++y)
			for (int x = tmp_x; x < tmp_x + tmp_s; ++x)
				view_field[y][x] = i;
	}

	for (auto& i : view_field) {
		for (auto& j : i) {
			std::cout << j << ' ';
		}
		std::cout << std::endl;
	}
	std::cout << "________________\n";
}

//Рекурсивная функция бэктрекинга
void backtracking(int * field, std::vector<element>& tmp, std::vector<element>& min, std::vector<int>& squares) {
	bool zero_found = false;
	element current;

	//Проверка на наличие в квадрате пустых мест
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j)
			if (field[i * size + j] == 0) {
				zero_found = true;
				current = { i, j, 1 };
				break;
			}
		if (zero_found)
			break;
	}

	if (!zero_found) {
		std::cout << "Squaring #" << cnt + 1 << std::endl;
		print_particular_answer(tmp);
		if (cnt > 0) {
			std::cout << "Going backwards!!!\n";
		}
		cnt++;

		if (check_variant(squares, tmp))
			min = tmp;
		return;
	}

	//Заполнение поля квадратами со стороной от 1 до n-1
	//Также выполняются проверки на возвожность вставки квадрата
	for (int tmp_len = 1; tmp_len < size; ++tmp_len) {

		if (squares.empty() && tmp.size() + 1 >= min.size()) {
			break;
		}

		if (tmp_len > size - current.x || tmp_len > size - current.y)
			return;

		for (int i = current.x; i < current.x + tmp_len; ++i)
			for (int j = current.y; j < current.y + tmp_len; ++j)
				if (field[i * size + j] == 1)
					return;

		for (int i = current.x; i < current.x + tmp_len; ++i)
			for (int j = current.y; j < current.y + tmp_len; ++j)
				field[i * size + j] = 1;

		current.len = tmp_len;

		tmp.push_back(current);

		backtracking(field, tmp, min, squares);
		for (int i = current.x; i < current.x + tmp_len; ++i)
			for (int j = current.y; j < current.y + tmp_len; ++j)
				field[i * size + j] = 0;
		tmp.pop_back();
	}
}

int main() {

	std::vector<int> squares;

	//Ввод необходимых данных для работы программы
	if (!input(squares)) {
		return 0;
	}
	std::vector<element> tmp;

	//Случай, когда требуется найти миниимальное разложение без обязательных квадратов
	if (squares.empty()) {

		//Находим наименьший делитель
		for (size = 2; size < n; ++size)
			if (n % size == 0)
				break;

		std::vector<element> min(size + 4);

		int multiplier = n / size;

		//Три квадрата для минимального разбиения
		tmp.push_back({ 0, size / 2 + size % 2, size / 2 });
		tmp.push_back({ size / 2 + size % 2, 0, size / 2 });
		tmp.push_back({ size / 2, size / 2, size / 2 + size % 2 });

		//Если сторона исходного квадрата нечётная, то с помощью алгоритма бэктрекинга
		// находим минимальное разбиение для оставшейся области
		if (size % 2 == 1) {
			int* field = new int[(size / 2 + 1) * (size / 2 + 1)]();
			field[(size / 2 + 1) * (size / 2 + 1) - 1] = 1;
			size = size / 2 + 1;

			backtracking(field, tmp, min, squares);
		}
		//Для стороны квадрата с чётной длиной минимальное разбиение всегда состоит из 4-ёх квадратов
		else {
			tmp.push_back({ 0, 0, 1 });
			min = tmp;
		}

		//Вывод найденного разбиения
		//Если сторона исходного квадрата была не простым числом, то координаты ответа умножаются на множитель
		std::cout << "Number of squares in the best squaring variant " << min.size() << std::endl;

		/*for (auto i:min) {
			std::cout << i.x * multiplier << " " << i.y * multiplier << " " << i.len * multiplier << std::endl;
		}*/
		std::cout << "The best squaring:\n";
		print_particular_answer(min);
	}
	//Для случая, когда заданы необходимые для разбиения квадраты
	// просматриваются все варианты разложения без оптимизаций
	else {
		size = n;
		std::vector<element> min(size*size);
		int *field = new int[size * size]();

		backtracking(field, tmp, min, squares);

		//Если ни одно разбиение не содержит необходимых квадратов, то выводится соответсвующее сообщение
		if (!check_variant(squares, min)) {
			std::cout << "Impossible variant!" << std::endl;
			return 0;
		}

		//Вывод найденного минимального разбиения
		std::cout << "Number of squares in the best squaring variant " << min.size() << std::endl;
		/*for (auto i:min) {
			std::cout << i.x << " " << i.y << " " << i.len << std::endl;
		}*/
		std::cout << "The best squaring:\n";
		print_particular_answer(min);
	}
	return 0;
}