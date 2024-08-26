#pragma once
#include <vector>
#include <array>
#include <iostream>

using namespace std;

class Math
{
public:
	// Pega os pontos de uma matriz e calcula o m�dio do coeficiente angular da fun��o.
	static double linearRegression(vector<vector<double>> data/* (x, y) */, bool inverted = 0, double constant = 1.0) {
		// Armazena o somat�rio das varia��es.
		double sum = 0;
		
		// Faz o somat�rio da fun��o.
		for (unsigned int i = 1; i < data.size(); i++) {
			try {
				sum += (data[i][1 - inverted] - data[i - 1][1 - inverted]) / (data[i][0 + inverted] - data[i - 1][0 + inverted]);

			}
			catch (const std::runtime_error& e) {
				e;
			}
		}

		return (sum / (data.size() - 1) * constant);
	}

	void log_value_table(vector<vector<double>> valueTable) {
		for (vector<double> line : valueTable) {
			for (int i = 0; i < line.size(); i++) {
				if (((unsigned int)i + 1) < line.size()) {
					cout << line[i] << "   ";
				}
				else {
					cout << line[i] << endl;
				}

			}
		}
	}
};

