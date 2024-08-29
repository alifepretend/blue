#include <iostream>
#include "../libraries/IO.h"
#include "../libraries/Math.h"
#include "../libraries/Calibration.h"
#include <ostream>
#include <vector>
#include <string>

using namespace std;

IO io;

int main() {
	vector<vector<double>> value_list = io.parsecsv(IO::readFile("./temp_measurement_ambient.txt")); 

	vector<vector<double>> trimmed_value_list = io.removeColumn(value_list, 2);

	vector<vector<double>> transformed_value_list = io.applyFunction(trimmed_value_list, -1, &Calibration::esp32AdcToVoltage);

  vector<vector<double>> resistance_value_list = io.applyFunction(transformed_value_list, 0, &Calibration::voltageInDivisorToResistance, 93.9, 3.3);
  
  resistance_value_list = io.applyFunction(resistance_value_list, 1, &Calibration::voltageInDivisorToResistance, 491, 3.3);

  resistance_value_list = io.applyFunction(resistance_value_list, 0, &Calibration::pt100ResistanceToTemperature);

  resistance_value_list = io.applyFunction(resistance_value_list, 1, &Calibration::ntcResistanceToTemperature); 

for (vector<double> line : resistance_value_list) {
    string temp_line = "";

    for (int i = 0; i < line.size(); i++) {
      temp_line += to_string(line[i]);
      if (i != line.size() - 1) {
        temp_line += ",";
      } else {
        temp_line += "\n";
      }
    }

    io.writeFile(temp_line, "./temperature_ambient.txt");
    cout << endl;
  }
  
  return 1;
}

