#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<map>
#include<vector>
using namespace std;

#include<fstream>

ofstream fout("out.txt"); // создаём объект класса ofstream для записи и связываем его с файлом cppstudio.txt
ifstream fin("inp.txt");


vector<int> prefix_function(string s)
{
	int n = s.length();
	vector<int> pi(n); // в i-м элементе (его индекс i-1) количество совпавших символов в начале и конце для подстроки длины i. 
			 // p[0]=0 всегда, p[1]=1, если начинается с двух одинаковых 
	for (int i = 1; i < n; ++i)
	{
		

		std::cout << "\nИщем какой префикс можно расширить\n";
		fout<< "\nИщем какой префикс можно расширить\n";

		int j = pi[i - 1]; 
		std::cout << "\n" << j << " длина предыдущего префикса-суффикса, возможно нулевая\n";
		fout<< "\n" << j << " длина предыдущего префикса-суффикса, возможно нулевая\n";

		while ((j > 0) && (s[i] != s[j])) // этот нельзя расширить,
			j = pi[j - 1];   // берем длину меньшего префикса-суффикса

		if (s[i] == s[j])
			++j;  // расширяем найденный (возможно пустой) префикс-суффикс
		pi[i] = j;
	}
	return pi;
}

void kmp(string T, string P, vector<int>& answer) {

	int n = T.length() - 1;
	int m = P.length() - 1;
	string a = "";
	for (int i = 1; i < m + 1; i++)
		a += P[i];
	//std::cout << a;
	vector<int> Pi1 = prefix_function(a);
	vector<int> Pi;
	Pi.push_back(0);
	for (int i = 0; i < Pi1.size(); i++)
		Pi.push_back(Pi1[i]);

	int q = 0;
	std::cout << "\nПрефикс функция подсчитана \n";
	fout<< "\nПрефикс функция подсчитана \n";
	for (int i = 1; i <= n; i++) {

		while (q > 0 && (P[q + 1] != T[i]))
			q = Pi[q];



		if (P[q + 1] == T[i]) {
			q += 1;
			//std::cout << "ax";
		}

		if (q == m) {
			//std::cout << i - m << " ";
			std::cout << "\nПаттерн совпадает с одним из слов в тексте на позиции "<<i-m<<"\n";
			fout<< "\nПаттерн совпадает с одним из слов в тексте на позиции " << i - m << "\n";
			answer.push_back(i - m);
			q = Pi[q];

		}


	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	std::cout << "\nВвод с консоли или из файла(1/2)?\n";
	int ind;
	cin >> ind;
	string P1;
	string P;
	string T2;
	string T;

	vector<int> answer;
	if (ind == 1) {
	
		cin >> P1;
		P = " ";
		P += P1;
		T2 = " ";
		cin >> T2;
		T = " ";
		T += T2;
		
	}
	else if (ind == 2) {
	
		fin >> P1;
		P = " ";
		P += P1;
		T2 = " ";
		fin >> T2;
		T = " ";
		T += T2;
		

	}

	kmp(T, P, answer);

	std::cout << "\nОтвет\n";
	fout<< "\nОтвет\n";
	if (answer.size() != 0) {
		for (int i = 0; i < answer.size(); i++)
			if (i != answer.size() - 1) {
				std::cout << answer[i] << ",";
				fout<< answer[i] << ",";
			}
			else {
				std::cout << answer[i];
				fout << answer[i];
			}
	}
	else {
		cout << "-1";
		fout << "-1";
	}


}
