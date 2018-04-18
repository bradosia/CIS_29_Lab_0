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
#include <vector>       // std::vector
#include <bitset>
using namespace std;

/**
 @class FileHandler
 simply reads or writes the decoded or encoded message to a file\n
 */
class FileHandler {
public:
	FileHandler() {
	}
	~FileHandler() {
	}
	/** takes a file stream by reference and opens a file.\n
	 * the reason we do not return the string of the entire ASCII file
	 * is because we want to stream and not waste memory
	 @pre None
	 @post None
	 @param string File name to open
	 @return True on file open successful and false in not
	 */
	bool readLines(string fileName, ifstream& fileStream);
	bool writeLines(string fileName, ofstream& fileStream);
	bool close(ifstream& fileStreamIn, ofstream& fileStreamOut);
};

/**
 @class hexCharTable
 It converts hexadecimals to chars \n
 */
class hexCharTable {
private:
	vector<char> table;
public:
	hexCharTable();
	~hexCharTable() {
	}
	bool hexToChar(unsigned int, unsigned int, char& charOut);
};

/**
 @class degreeHexTable
 It converts degree angles to hexadecimal \n
 */
class degreeHexTable {
private:
	vector<unsigned int> table;
	const int hexInDegree = 16;
public:
	degreeHexTable();
	~degreeHexTable() {
	}
	bool degreeToHex(unsigned int, unsigned int&);
};

/**
 @class DegreePair
 Utilities for pairs of degree angles \n
 */
class DegreePair {
private:
	unsigned int degree1;
	unsigned int degree2;
	bool badFlag;
	const double maxDegree = 360; //inclusive
	const double minDegree = 0; //inclusive
	degreeHexTable angleTable;
	hexCharTable hexTable;
public:
	DegreePair(double angle1, double angle2);
	~DegreePair() {
	}
	bool toChar(char& retChar);
};

/**
 @class Parser
 Utilities for stream decoding \n
 */
class Parser {
private:
	unsigned int fileSize, filePos, angleBufferNum, charWritten, ignoreNum;
	char charBuffer[2], charBufferOut[1];
	double angle1, angleTemp;
	string streamBuffer;
public:
	Parser() {
	}
	~Parser() {
	}
	bool degreeLineToCharLine(ifstream& fileStreamIn, ofstream& fileStreamOut);
	bool bufferHandle();
	bool angleHandle();
};

/*
 * hexCharTable Implementation
 */
hexCharTable::hexCharTable() {
	unsigned int i, n;
	// build a lookup map
	// hex values are actually represented in base10
	for (i = 0, n = 128; i < n; i++) {
		table.push_back(char(i));
	}
}

bool hexCharTable::hexToChar(unsigned int hex1, unsigned int hex2,
		char& charOut) {
	unsigned long charCode;
	std::bitset<4> bs1(hex1);
	std::bitset<4> bs2(hex2);
	/*cout << hex1 << "=" << bs1.to_string() << " " << hex2 << "="
	 << bs2.to_string() << endl;*/
	try {
		charCode = bitset<8>(bs1.to_string() + bs2.to_string()).to_ulong();
		charOut = table[charCode];
		return true;
	} catch (...) {
		return false;
	}
}
/*
 * degreeHexTable Implementation
 */
degreeHexTable::degreeHexTable() {
	unsigned int i, n;
	// build a lookup map
	for (i = 0, n = 360; i < n; i++) {
		table.push_back(
				(unsigned int) ((double) i / (360.0 / (double) hexInDegree)));
	}
	table[360] = hexInDegree;
}

bool degreeHexTable::degreeToHex(unsigned int degree_, unsigned int& hexOut) {
	try {
		hexOut = table[degree_];
		return true;
	} catch (...) {
		// nothing
	}
	return false;
}

/*
 * FileHandler Implementation
 */
bool FileHandler::close(ifstream& fileStreamIn, ofstream& fileStreamOut) {
	fileStreamIn.close();
	fileStreamOut.close();
	return true;
}

bool FileHandler::readLines(string fileName, ifstream& fileStream) {
	fileStream.open(fileName);  // Use it to read from a file named data.txt.
	if (fileStream.is_open()) {
		return true;
	} else {
		return false;
	}
}

bool FileHandler::writeLines(string fileName, ofstream& fileStream) {
	fileStream.open(fileName);  // Use it to read from a file named data.txt.
	if (fileStream.is_open()) {
		return true;
	} else {
		return false;
	}
}

/*
 * DegreePair Implementation
 */
DegreePair::DegreePair(double angle1, double angle2) {
	//cout << angle1 << " " << angle2 << endl;
	badFlag = false;
	if (angle1 > maxDegree || angle1 < minDegree || angle2 > maxDegree
			|| angle2 < minDegree) {
		// out of range, let's mark the angles as bad
		degree1 = degree2 = 0;
		badFlag = true;
	}
	degree1 = (unsigned int) angle1;
	degree2 = (unsigned int) angle2;
}

bool DegreePair::toChar(char& retChar) {
	unsigned int hex1, hex2;
	if (!badFlag) {
		if (angleTable.degreeToHex(degree1, hex1)
				&& angleTable.degreeToHex(degree2, hex2)) {
			if (hexTable.hexToChar(hex1, hex2, retChar)) {
				return true;
			}
		}
	}
	return false;
}

/*
 * Parser Implementation
 */
bool Parser::degreeLineToCharLine(ifstream& fileStreamIn,
		ofstream& fileStreamOut) {
	streamBuffer = "";
	angle1 = 0;
	charBuffer[1] = 0; // null termination for the c-string
	fileSize = angleBufferNum = charWritten = ignoreNum = 0;
	fileStreamIn.seekg(0, ios::end); // set the pointer to the end
	fileSize = fileStreamIn.tellg(); // get the length of the file
	fileStreamIn.seekg(0, ios::beg); // set the pointer to the beginning
	for (filePos = 0; filePos < fileSize; filePos++) {
		fileStreamIn.seekg(filePos, ios::beg);
		fileStreamIn.read(charBuffer, 1);
		// successfully parsed a single double
		if (bufferHandle()) {
			//cout << angle1 << " " << angleTemp << endl;
			// successfully parsed a single double
			if (angleHandle()) {
				fileStreamOut.write(charBufferOut, 1);
				charWritten++;
			}
		}
		// to many bad angles, stop parsing
		if (ignoreNum >= 3) {
			return false;
		}
	}
	if (charWritten > 0) {
		return true;
	}
	return false;
}

bool Parser::bufferHandle() {
	bool flag = false;
	if (isdigit(charBuffer[0]) || charBuffer[0] == '.') {
		// numbers and decimals are allowed for double
		streamBuffer.append(charBuffer);
	} else {
		// non-double found. Let's assume it is a delimiter and parse our buffer as a double now
		if (streamBuffer.length() > 0) {
			//cout << streamBuffer << endl;
			if (streamBuffer.length() < 5) {
				try {
					angleTemp = stod(streamBuffer);
					flag = true;
				} catch (...) {
					// conversion not good.
					ignoreNum++;
				}
			} else {
				ignoreNum++;
			}
			streamBuffer = "";
		}
	}
	return flag;
}

bool Parser::angleHandle() {
	if (angleBufferNum == 0) {
		angle1 = angleTemp;
		angleBufferNum++;
	} else if (angleBufferNum == 1) {
		// We now have a pair of two angles so now we can convert them to a char
		angleBufferNum = 0;
		DegreePair degreePair(angle1, angleTemp);
		if (degreePair.toChar(charBufferOut[0])) {
			return true;
		} else {
			// conversion not good. let's ignore this char
			ignoreNum++;
		}
	}
	return false;
}

/*
 * main & interface
 * Rules For Decoding:
 * - delimiters don't matter. Consecutive angles are always paired up.
 * - angles expressed as decimals are okay
 * - any angle that can't be converted to a double will stop the entire file from decoding
 * - any angle not in 0 <= x <= 360.0 will be ignored
 * - any angle that does not convert to an ascii character 0-127 will be ignored
 * - after ignoring 3 bad angles or characters the parsing will stop
 */
int main() {
	FileHandler fh;
	Parser parser;
	string fileNameIn, fileNameOut;
	ifstream fileStreamIn;
	ofstream fileStreamOut;
	/*cout << "Please enter the input file name and extension:" << endl;
	 getline(cin, fileNameIn);
	 cout << "Please enter the output file name and extension:" << endl;
	 getline(cin, fileNameOut);*/
	fileNameIn = "AsciiDegrees.csv";
	fileNameOut = "output.txt";
	if (!fh.readLines(fileNameIn, fileStreamIn)
			|| !fh.writeLines(fileNameOut, fileStreamOut)) {
		cout << "Could not read either the input or output file." << endl;
	} else {
		cout << "Reading file..." << endl;
		/* we pass file streams instead of a string to this method
		 * because we want to stream the data and decode it as we read.
		 * This way very large files won't lag or crash the program.
		 */
		if (parser.degreeLineToCharLine(fileStreamIn, fileStreamOut)) {
			cout << "File successfully decoded!" << endl;
		} else {
			cout << "File could not be decoded." << endl;
		}
		fh.close(fileStreamIn, fileStreamOut);
	}
	cout << "Enter any key to exit..." << endl;
	string temp;
	getline(cin, temp);
	return 0;
}
/*
 * Design Reflection:
 * Writing the message parser as reading and writing two streams simultaneously
 * was actually not a good idea since it creates more random reads and writes for
 * the hard drive.
 */
