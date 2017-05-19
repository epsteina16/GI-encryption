/* Aaron Epstein
* Copyright 2017
* Crypter.h - Defines a crypter class to handle the encryptions/decryptions
	of files
*/

#include <string>
#include <fstream>
using namespace std;

#ifndef CRYPTER_H_
#define CRYPTER_H_

class Crypter
{
public:
	Crypter(string type, string filename);
	~Crypter();

private:
	string key;
	int **file;
	int size;

	void getFileSize(string filename, string type);
	void readInFile(string filename, string type);
	void readUnencryptedFile(ifstream &infile);
	void readEncryptedFile(ifstream &infile);
	void encrypt(string filename);
	void decrypt(string filename);
	string permutateMatrix();
	void swapRows(int row1, int row2);
	void swapColumns(int row1, int row2);
	void createEncryptedFile(string filename);
	void createDecryptedFile(string filename);
	void decryptMatrix();
	void printInfo(string filename);
	void ensureSize();
	bool string2int(const string& str, int& result);
	void transformMatrix();
	void printMatrix();
};
#endif