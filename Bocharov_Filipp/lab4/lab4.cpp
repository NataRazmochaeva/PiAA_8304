#include <iostream>
#include <string>
#include <vector>
#include <fstream>


void prefix(const std::string& S, std::vector<int>& n) {
	n[0] = 0;
	for (unsigned long int i = 1; i < S.size(); ++i) {
		int j = n[i - 1];
		while (j > 0 && S[j] != S[i])
			j = n[j - 1];
		if (S[j] == S[i])
			j += 1;
		n[i] = j;
	}
}

void KMP(std::istream& input, std::ostream& output) {
	std::string findstring;
	input >> findstring;
	std::vector<int> len(findstring.size());
	prefix(findstring, len);
	output << "Prefix: ";
	for (int j : len)
		output << j << " ";
	output << std::endl;
	int j = 0; //длина совпадений
	int result = -1;//выводим -1 по условию если не совпадает
	char inputstring;
	input.get(inputstring);
	input.get(inputstring);
	std::vector<int> answer;
	int i = 0;
	while (inputstring != '\n') {
		output << "Changes when i = " << i << " Start value k = " << j << std::endl;
		while (j > 0 && inputstring != findstring[j]) {//пока не совпадут символы
			j = len[j - 1];
			output << " k = " << j << std::endl;
		}
		if (inputstring == findstring[j]) {//если совпали
			j += 1;//увеличиваем значение
			output << " k = " << j << std::endl;
		}
		if (j == findstring.size()) {
			result = i - findstring.size() + 1;//значит ответ получен
			answer.push_back(result);
			output << "---" << std::endl;
			output << "Result found. Index = " << result << std::endl;
			output << "---" << std::endl;
		}
		i += 1;
		input.get(inputstring);
	}

	output << std::endl << "Result: ";
	if (!answer.empty())
		for (unsigned long int i = 0; i < answer.size() - 1; ++i) {
			output << answer[i] << ",";
		}
	output << result;
}


int main() {
		
	KMP(std::cin, std::cout);
				
	return 0;
}