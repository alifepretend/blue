INC=-I/../libraries

main: ../libraries/Calibration.h ../libraries/IO.cpp ../libraries/IO.h ../libraries/Math.h ../libraries/Math.cpp ../libraries/serialib.h ../libraries/serialib.cpp
	g++ -o test $(INC) ../libraries/IO.cpp ../libraries/Math.cpp ../libraries/serialib.cpp main.cpp

clean:
	rm main
