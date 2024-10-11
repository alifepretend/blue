#include <iostream>
#include <vector>
#include <array>
#include "Math.h"
#include "IO.h"

#define PATH "F:/Programming_Workspace/FunctionsCPP/documents/reading1.txt"

using namespace std;

static double integrateLong(vector<vector<long>> valueTable, int y, int x) {
	double sum = 0;

	long lastY = valueTable[0][y];
	long lastX = valueTable[0][x];

	for (int i = 1; i < valueTable.size(); i++) {
		vector<long> line = valueTable[i];
		long dx = line[x] - lastX;

		if (dx > 0) {
			sum += (double)((line[y] + lastY) * dx) / 2;
			
		}
		cout << line[0] << ", " << line[1] << ", " << line[2] << "  Sum = " << sum << endl;
		lastX = line[x];
		lastY = line[y];
	}

	return sum;
}

int main() {
	vector<string> lineList = IO::readFile(PATH);

	vector<vector<long>> valueTable = IO::parseCsv(lineList);
	
	integrateLong(valueTable, 1, 2);
	//cout << valueTable.size() << endl;

	return 0;
}




