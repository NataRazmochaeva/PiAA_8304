#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::string inPath = "./input.txt";
std::string outPath = "./output.txt"; // Пути к файлам ввода и вывода

void prefixFunction(const std::string& srcString, std::vector<int>& prefixFunctionValues); // Префикс-функция
void KMP(std::istream& input, std::ostream& output); // Алгоритм КМП

void prefixFunction(const std::string& srcString, std::vector<int>& prefixFunctionValues) {
	prefixFunctionValues[0] = 0; // Инициализируем начальное значение префикс-функции

	for (size_t i = 1; i < srcString.size(); ++i) {
		int expectedValue = prefixFunctionValues[i - 1]; // Запоминаем предыдущее значение,
														 // оно фигурирует в теоретическких расчетах для КМП
		while (expectedValue > 0 && srcString[expectedValue] != srcString[i]) // Смотрим в предыдущие значения, пока не встретим 0 или пока не получим совпадение
			expectedValue = prefixFunctionValues[expectedValue - 1];// Шаг назад

		if (srcString[expectedValue] == srcString[i]) // Если цикл завершился, потому что есть совпадение, то
			expectedValue += 1;						  // увеличиваем на 1 и пишем в ответ. В случае, когда дошли до 0, запишется 0

		prefixFunctionValues[i] = expectedValue;
	}
}

void KMP(std::istream& input, std::ostream& output) {
	std::string substring;

	std::cout << "Enter pattern(substring)" << std::endl;
	input >> substring; // Считываем шаблон (подстроку)

	std::vector<int> prefixFunctionValues(substring.size()); // Вектор для хранения значений префикс-функции
	prefixFunction(substring, prefixFunctionValues); // Вычисляем значение префикс-функции

	output << "Prefix function values: ";
	for (auto i : prefixFunctionValues)
		output << i << " ";
	output << std::endl;

	int expectedValue = 0;

	int resultCount = -1; // Если совпадений не обнаружится, то ответ так и останется -1

	char current(0); // Для посимвольного считывания строки

	std::cout << "Enter the text string" << std::endl;

	input.get(current); // Считываем символ переноса строки, оставшийся после ввода шаблона
	input.get(current); // Считываем первый символ строки-текста

	std::vector<int> result; // Массив с ответом - индексами вхождений

	
	for(int i =0; (current != '\n' && !input.eof()); ++i) { // Пока каретка не дошла до конца входных данных
		output << "Current index = " << i << " Expected value = " << expectedValue << std::endl;

		while (expectedValue > 0 && current != substring[expectedValue]) { // Пока не совпадут последние символы
			expectedValue = prefixFunctionValues[expectedValue - 1]; // Пробуем слудющее значение
			
			output << " Expected value = " << expectedValue << std::endl;
		}
		
		if (current == substring[expectedValue]) { // Если есть совпадение
			expectedValue += 1; // Засчитываем вхождение

			output << " Expected value = " << expectedValue << std::endl;
		}

		if (expectedValue == substring.size()) { // Если совпали длины совпадений и шаблона
			resultCount = i - substring.size() + 1; // Формируем индекс
			result.push_back(resultCount);
			output << "_____________________________________________" << std::endl;
			output << "Loop iteration #" << i << " Expected value was (equal to substr length) = " << expectedValue
				<< " Found index = " << resultCount << std::endl;
			output << "______________________________________________" << std::endl;
		}
		input.get(current);
	}

	output << std::endl << "Result: ";

	if (!result.empty()) {
		for (size_t i = 0; i < result.size() - 1; ++i) {
			output << result[i] << ",";
		}

		output << result[result.size() - 1] << std::endl;
	}

	output << "Total entries : " << (int)(((result.size()) != 0) ? (result.size()) : (-1))  << std::endl;
}


int main() {
	std::cout << "Choose input mode" << std::endl << std::endl
		<< "1 - console" << std::endl
		<< "2 - file" << std::endl; // Выбор режима ввода

	int mode = 0;

	std::cin >> mode;
	if (mode == 1) { // Консоль
		std::cout << std::endl << "Choose output mode" << std::endl << std::endl
			<< "1 - console" << std::endl
			<< "2 - file" << std::endl; // Выбор режима вывода
		std::cin >> mode;

		if (mode == 1) { // Консоль
			KMP(std::cin, std::cout);
		}

		else if (mode == 2) { // Файл
			std::ofstream fileout;

			fileout.open(outPath);

			if (!fileout.is_open()) {
				std::cout << "Can't open file!\n";
			}

			KMP(std::cin, fileout);
		}
	}

	else if (mode == 2) { // Файл
		std::ifstream filein;

		filein.open(inPath);

		std::cout << std::endl << "Choose output mode" << std::endl << std::endl
			<< "1 - console" << std::endl // Выбор режим вывода
			<< "2 - file" << std::endl;

		std::cin >> mode;

		if (mode == 1) { // Консоль
			KMP(filein, std::cout);
		}

		else if (mode == 2) { // Файл
			std::ofstream fileout;

			fileout.open(outPath);

			KMP(filein, fileout);
		}


	}


	return 0;
}