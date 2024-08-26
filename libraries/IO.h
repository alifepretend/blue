#pragma once
#include "serialib.h"

#ifndef IO_H
#define IO_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <codecvt>
#include <locale>
#include <functional>

using namespace std;



class IO
{
public:
	struct Position{
		unsigned int x = 0;
		unsigned int y = 0;
	};

	void writeFile(string text, string path, bool append = 1);

	void writeFileWithCurrentTime(string text, string path, bool append = 1);

	// Retorna a lista de linhas em um arquivo.
	static vector<string> readFile(string path);

	struct Type {
		static const int STRING = 0;
		static const int INT = 1;
		static const int DOUBLE = 2;
	};

	vector<vector<double>> parsecsv(vector<string> linelist);

	double stringToDouble(string s);

	vector<vector<double>> approximateGivenTheTime(vector<vector<double>>* reference_list, vector<vector<double>> signal_list);

	vector<vector<double>> removeColumn(vector<vector<double>> matrix, int column_index);

  // Passar -1 para o column_index aplica a função a todos os elementos da matriz.
  vector<vector<double>> applyFunction(vector<vector<double>> matrix, int column_index, function<double(double)> fun);

  vector<vector<double>> applyFunction(vector<vector<double>> matrix, int column_index, function<double(double, double, double)> fun, double arg1, double arg2);

	class Serial {
	private:
		serialib serial;
	public:
		char erroropening = 0;

		// inicia a porta serial.
		Serial(const char* serial_port, long int baud_rate);

		// retorna 1 no caso de sucesso, 0 caso falhe.
		int  receiveData(string* line);
	};
};

#endif // !IO_H
