//============================================================================
// Name        : Lab0
// Author      : Branden Lee
// Version     :
// Copyright   : Free
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <string>
#include <fstream>
#include <iostream>     // std::cout
#include <algorithm>    // std::find
#include <vector>       // std::vector
using namespace std;

class FileHandler {
private:
	fstream fileIn;
	fstream fileOut;
public:
	FileHandler();
	~FileHandler();
	bool readLines(string fileName, vector<double>&);
	void writeLines(string fileName, string writeStr);
};

class hexCharTable {
private:
	vector<char> table;
public:
	hexCharTable();
	~hexCharTable();
	char hexToChar(int);
	int charToHex(char);
};

class degreeHexTable {
private:
	vector<int> table;
	int hexInDegree;
public:
	degreeHexTable();
	~degreeHexTable();
	int degreeToHex(int);
	int hexToDegree(int);
};

class CharClass {
private:
	char char1;
	degreeHexTable angleTable;
	hexCharTable hexTable;
public:
	CharClass(char);
	~CharClass();
	char toChar();
	int degreeToHex;
	void toHex(double&, double&);
};

class DegreePair {
private:
	int degree1;
	int degree2;
	const int maxDegree;
	const int minDegree;
	degreeHexTable angleTable;
	hexCharTable hexTable;
public:
	DegreePair(double, double);
	~DegreePair();
	char toChar();
};

hexCharTable::hexCharTable() {
	unsigned int i, n;
	// build a lookup map
	// hex values are actually represented in base10
	for (i = 0, n = 128; i < n; i++) {
		table[i] = char(i);
	}
}

hexCharTable::~hexCharTable() {
	// pointers were not used
}

char hexCharTable::hexToChar(int hex_) {
	try {
		return table[hex_];
	} catch (...) {
		return char(0);
	}
}

int hexCharTable::charToHex(char char_) {
	try {
		return *(vector<char>::iterator) (find(table.begin(), table.end(),
				char_));
	} catch (...) {
		return 0;
	}
}

degreeHexTable::degreeHexTable() {
	hexInDegree = 16;
	unsigned int i, n;
	// build a lookup map
	for (i = 0, n = 360; i < n; i++) {
		table[i] = (int) ((double) i / (360.0 / (double) hexInDegree));
	}
	table[360] = hexInDegree;
}

degreeHexTable::~degreeHexTable() {
	// pointers were not used
}

int degreeHexTable::degreeToHex(int degree_) {
	try {
		return table[degree_];
	} catch (...) {
		return 0;
	}
}

int degreeHexTable::hexToDegree(int hex_) {
	try {
		return *(vector<int>::iterator) (find(table.begin(), table.end(), hex_));
	} catch (...) {
		return 0;
	}
}

FileHandler::FileHandler() {

}
FileHandler::~FileHandler() {

}

bool FileHandler::readLines(string filename, vector<double>& angles) {
	ifstream in;    // Create an input file stream.
	double angle;
	in.open(filename);  // Use it to read from a file named data.txt.
	cout << "whyyy";
	if (in.is_open()) {
		cout << "why";
		while (in >> angle) {
			cout << angle << endl;
			angles.push_back(angle);
		}
		return true;
	} else {
		return false;
	}
}

class Parser {
private:
	string degreeLine;
	string charLine;
	vector<DegreePair> degreePairList;
	vector<CharClass> charClassList;
public:
	Parser(string);
	~Parser();
	vector<DegreePair> parseDegree(string);
	vector<CharClass> parseChar(string);
	string degreeLineToCharLine(string);
	string charLineToDegreeLine(string);
};

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
	vector<double> angles;
	FileHandler fh;
	string fileName;
	cout << "Please enter the file name and extension:" << endl;
	cin >> fileName;
	if(!fh.readLines(fileName, angles)){
		cout << "Could not read the file." << endl;
	} else {
		cout << "File read." << endl;
	}
	unsigned int i, n;
	n = angles.size();
	for (i = 0; i < n; i++) {
		cout << angles[i] << endl;
	}

	double x;
	char ascii;
	cin >> x;
	ascii = martian::degreeToHex(x);
	if (ascii == 0)
		cout << "out of range";
	else
		cout << x << " hex:" << ascii;
	cin >> x;
	return 0;
}
