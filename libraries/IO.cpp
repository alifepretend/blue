#include "IO.h"
#include <chrono>

string temp = "";

void IO::writeFile(string text, string path, bool append) {
    using namespace std::chrono;

    // Cria e abre um arquivo de texto
    // ios::app -> append
    ofstream file;
    if (append) {
        file.open(path, ios::app | std::ios_base::out);
    }
    else {
        file.open(path);
    }
    
    // Escreve no arquivo
    file << text;

    // Fecha o arquivo
    file.close();

    cout << text << "	Written to file" << endl;
}

void IO::writeFileWithCurrentTime(string text, string path, bool append) {
    using namespace std::chrono;

    // Cria e abre um arquivo de texto
    // ios::app -> append
    text.pop_back();
    ofstream file;
    if (append) {
        file.open(path, ios::app | std::ios_base::out);
    }
    else {
        file.open(path);
    }


    // Escreve no arquivo
    auto time = system_clock::now();
    file << text << ',' << duration_cast<milliseconds>(time.time_since_epoch()).count();

    // Fecha o arquivo
    file.close();

    cout << text << "	Written to file" << endl;
}

vector<string> IO::readFile(string path) {
    ifstream file(path);

    string line;
    vector<string> lineList;

    // Adiciona as linhas na lista.
    while (getline(file, line)) {
        lineList.push_back(line);
    }

    return lineList;
}

vector<vector<double>> IO::parsecsv(vector<string> linelist) {
    int doublequotescount = 0;
    int quotescount = 0;
    int parenthesescount = 0;

    // A posição inicia no (0, 0) [x, y]
    Position currentposition;

    // y<x<valor>>
    vector<vector<double>> valuematrix;

    for (currentposition.y; currentposition.y < linelist.size(); currentposition.y++) {
        string line = linelist[currentposition.y];
        vector<double> linetemp;
        string valuetemp;
	bool append = true;      
        
	for (unsigned int i = 0; i < line.length(); i++) {
            char c = line[i];
	         
            switch (c) {
                case ',': {
                    if ((doublequotescount | quotescount | parenthesescount) > 0) {
                        valuetemp += c;
                    }
                    else {
                        // adiciona valor à lista temporária da linha.
                        linetemp.push_back(stringToDouble(valuetemp));
			
                        valuetemp = ""; // reinicia o valor.					
                    }

                    break;
                }
                case ';': {
                    linetemp.push_back(stringToDouble(valuetemp));
		    append = false;
                    break;
                }
                case '\n': {
		    if(append == true) {
               	     	linetemp.push_back(stringToDouble(valuetemp));
		     	append = false;
		     }
		     break;
                }
                case '\r': {
		    if(append == true) {
                    	linetemp.push_back(stringToDouble(valuetemp));
		    	append = false;
		    }
		    
		    break;
                }

                // avisa que um valor string está começando ou terminando.
                case '"': {
                    if (doublequotescount > 0) {
                        doublequotescount--;
                    }
                    else {
                        doublequotescount++;
                    }
                    break;
                }
                
                // Qualquer outro caractere que não se encaixe nas condições acima.
                default: {
                    valuetemp += c;
                }

            }
        }
	valuematrix.push_back(linetemp);
    }

    return valuematrix;

}

double IO::stringToDouble(string s) {
    istringstream iss(s);
    double value;
    // Tenta extrair um double do stream
    if (iss >> value) {
        return value;
    }
    else {
        return 0;
    }
}

IO::Serial::Serial(const char* serial_port, long int baud_rate) {
    // tenta iniciar conex�o com porta serial.
    erroropening = serial.openDevice(serial_port, baud_rate);

    // se conseguir abrir o dispositivo:
    if (erroropening == 1) {
        printf("successful connection to %s\n", serial_port);
    }
    else {
        cout << "erro ao tentar estabelecer conex�o com dispositivo." << endl;
    }
}

int IO::Serial::receiveData(string* line) {
    if (serial.available() != 0) {
        char c;

        serial.readChar(&c, 4);
            
        switch (c) {
            case ';': {               
                temp += c;
                *line = temp;
                temp = "";

                return 1;
                break;
            }
            case '\n': {
                break;
            }
            case '\r': {
                break;
            }
            case '\0': {
                break;
            }
            default: {
                temp += c;
            }
        }
    }

    return 0;
}

// Pega os valores mais próximos do y da segunda coluna em relação ao y da primeira coluna (proximidade
// de acordo com o x).
// Retorna o y da referencia em função do y do sinal.
vector<vector<double>> IO::approximateGivenTheTime(vector<vector<double>>* reference_list, vector<vector<double>> signal_list) {
    vector<vector<double>> approximation = {};
    int current_signal_index_verified = 0;
    int last_signal_index_verified = 0;
    bool next = false;

    for (int i = 0; i < (*reference_list).size(); i++) {
        next = false;


        while (!next && (current_signal_index_verified < signal_list.size()-1)) {
            if (signal_list[current_signal_index_verified][1] <= (*reference_list)[i][1]) {
                last_signal_index_verified = current_signal_index_verified;
                current_signal_index_verified += 1;
            }
            else {
                approximation.push_back({ signal_list[last_signal_index_verified][0], (*reference_list)[i][0] });
                cout << "Added to aproximation: { " << signal_list[last_signal_index_verified][0] << ", " << (*reference_list)[i][0] << " }" << endl;
                next = true;
            }
        }
    }
    return approximation;
}

vector<vector<double>> IO::removeColumn(vector<vector<double>> matrix, int column_index) {
	vector<vector<double>> new_matrix;
	for (int i = 0; i < matrix.size(); i++) {
		vector<double> line;
		for (int j = 0; j < matrix[i].size(); j++) {
			if(j != column_index) {
				line.push_back(matrix[i][j]);	
			}
		}
		new_matrix.push_back(line);
	}
	return new_matrix;
}

vector<vector<double>> IO::applyFunction(vector<vector<double>> matrix, int column_index, function<double(double)> fun) {
  // Aplica a função em todos os elementos.
  if(column_index == -1) {
   	vector<vector<double>> new_matrix;
	  for (int i = 0; i < matrix.size(); i++) {
		  vector<double> line;
		  for (int j = 0; j < matrix[i].size(); j++) {
				  line.push_back(fun(matrix[i][j]));	
		  }
		  new_matrix.push_back(line);
	  }
	  return new_matrix;     
    } else {
    vector<vector<double>> new_matrix;
	  for (int i = 0; i < matrix.size(); i++) {
		  vector<double> line;
		  for (int j = 0; j < matrix[i].size(); j++) {
			  if(j == column_index) {
			  	line.push_back(fun(matrix[i][j]));	
		  	} else {
          line.push_back(matrix[i][j]);
        }
		  }
		  new_matrix.push_back(line);
	  }
	  return new_matrix;
    }
  }

vector<vector<double>> IO::applyFunction(vector<vector<double>> matrix, int column_index, function<double(double, double, double)> fun, double arg1, double arg2) {
  // Aplica a função em todos os elementos.
  if(column_index == -1) {
   	vector<vector<double>> new_matrix;
	  for (int i = 0; i < matrix.size(); i++) {
		  vector<double> line;
		  for (int j = 0; j < matrix[i].size(); j++) {
				  line.push_back(fun(matrix[i][j], arg1, arg2));	
		  }
		  new_matrix.push_back(line);
	  }
	  return new_matrix;     
    } else {
    vector<vector<double>> new_matrix;
	  for (int i = 0; i < matrix.size(); i++) {
		  vector<double> line;
		  for (int j = 0; j < matrix[i].size(); j++) {
			  if(j == column_index) {
			  	line.push_back(fun(matrix[i][j], arg1, arg2));	
		  	} else {
          line.push_back(matrix[i][j]);
        }
		  }
		  new_matrix.push_back(line);
	  }
	  return new_matrix;
    }
  }

vector<vector<double>> IO::applyFunction(vector<vector<double>> matrix, int column_index, function<double(double, double, double, double)> fun, double arg1, double arg2, double arg3) {
  // Aplica a função em todos os elementos.
  if(column_index == -1) {
   	vector<vector<double>> new_matrix;
	  for (int i = 0; i < matrix.size(); i++) {
		  vector<double> line;
		  for (int j = 0; j < matrix[i].size(); j++) {
				  line.push_back(fun(matrix[i][j], arg1, arg2, arg3));	
		  }
		  new_matrix.push_back(line);
	  }
	  return new_matrix;     
    } else {
    vector<vector<double>> new_matrix;
	  for (int i = 0; i < matrix.size(); i++) {
		  vector<double> line;
		  for (int j = 0; j < matrix[i].size(); j++) {
			  if(j == column_index) {
			  	line.push_back(fun(matrix[i][j], arg1, arg2, arg3));	
		  	} else {
          line.push_back(matrix[i][j]);
        }
		  }
		  new_matrix.push_back(line);
	  }
	  return new_matrix;
    }
  }
