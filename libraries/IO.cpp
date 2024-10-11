#include "IO.h"
class SerialConfig {
public:
	SerialConfig(char* serial_port, long int baud_rate) {
		serialib serial;

		// Tenta iniciar conexão com porta serial.
		char errorOpening = serial.openDevice(serial_port, baud_rate);

		// Se conseguir abrir o dispositivo:
		if (errorOpening == 1) {
			printf("Successful connection to %s\n", serial_port);
		}
		else {
			cout << "Erro ao tentar estabelecer conexão com dispositivo." << endl;
		}
	}
};