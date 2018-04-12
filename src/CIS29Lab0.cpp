//============================================================================
// Name        : Lab0
// Author      : Branden Lee
// Version     :
// Copyright   : Free
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

class martian {
public:
	static char degreeToHex(double degree) {
		int hexInt;
		if (degree >= 0.0 && degree <= 360.0) { // 0.0 to 360.0 range
			hexInt = (int) (degree / (360.0 / 16.0)); // 16 hex values
			if (hexInt < 10)
				return char(hexInt + 48); // 0 to 10
			else if (hexInt < 16)
				return char(hexInt - 10 + 65); // 10 to 15
			else
				return char(70); // 360.0 => F
		}
		return 0; // null char on error or out of range
	}
};

int main() {
	double x;
	char ascii;
	cin >> x;
	ascii = martian::degreeToHex(x);
	if (ascii == 0)
		cout << "out of range";
	else
		cout << x << " hex:" << ascii;
	return 0;
}
