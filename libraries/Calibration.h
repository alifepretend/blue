class Calibration {
public:
  static double esp32AdcToVoltage(double signal) {
		double voltage = 0;
    if (signal <= 106) {
      voltage = -2.20612720612721 / (10*10*10*10*10) * signal * signal + 0.00401723276723277 * signal + 0.0420537795537795;
    } else if (signal <= 3161) {
      voltage = 0.000801963993453355 * signal + 0.134991816693945;
    } else if (signal <= 3950) {
      voltage = -1.9786426776347 / (10*10*10*10*10*10*10) * signal * signal + 0.00196468074215982 * signal - 1.56331169375177;
    } else {
      voltage = 9.22233300100278 / (10*10*10*10*10*10) * signal * signal - 0.0729341466600668 * signal + 147.309613659118;
    }

    return voltage;
	}
  
  // Funciona para calcular a resistência quando esta está com um dos lados conectados ao aterramento.
  // A resistance é o valor da resistência medido da resistência que não varia em série.
  static double voltageInDivisorToResistance(double voltage, double resistance, double v_max) {
    return ((resistance*voltage)/(v_max-voltage));
  }
};
