/* Aaron Epstein
* Copyright 2017
* Crypter.cpp - Implements crypter class
*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "crypter.h"
#include <cstddef>

using namespace std;

Crypter::Crypter(string type, string filename)
{
	getFileSize(filename, type);
	readInFile(filename, type);
	if (type == "encrypt")
		encrypt(filename);
	else
		decrypt(filename);
}

Crypter::~Crypter()
{
	for (int i=0; i < size; i++){
		delete [] file[i];
	}
	delete [] file;
}

//stores file size for given file (makes it square)
void Crypter::getFileSize(string filename, string type)
{
	ifstream infile;
	infile.open(filename);

	if (!infile.is_open()){
		cerr << "Could not open file: " << filename << endl;
		exit(1);
	}

	string line;
	int numLines = 0;
	int maxLineLength = 0;
	while (getline(infile, line)){
		numLines++;
		if (line.length() > (unsigned)maxLineLength)
			maxLineLength = line.length();
	}

	if (type == "encrypt"){
		if (numLines > maxLineLength){
			size = numLines;
		} else{
			size = maxLineLength;
		}
	} else{
		size = numLines;
	}
	ensureSize();

	infile.close();
}

//reads file into file matrix
void Crypter::readInFile(string filename, string type)
{
	file = new int*[size];
	for (int i=0; i < size; i++){
		file[i] = new int[size];
	}
	
	ifstream infile;
	infile.open(filename);

	if (type == "encrypt")
		readUnencryptedFile(infile);
	else
		readEncryptedFile(infile);

	infile.close();
}

//reads unencrypted file (regular file)
void Crypter::readUnencryptedFile(ifstream &infile)
{
	string line;
	int lineNum = 0;
	int prevChar;
	int newChar;
	while (getline(infile, line)){
		prevChar = line[0];
		file[lineNum][0] = line[0];
		for (size_t i=1; i < line.length(); i++){
			newChar = prevChar - line[i];
			file[lineNum][i] = newChar;
			prevChar = line[i];
		}
		if (line.length() < (unsigned)size){
			for (int j=line.length(); j < size; j++){
				newChar = prevChar - 32;
				file[lineNum][j] = newChar;
				prevChar = 32;
			}
		}
		lineNum++;
	}
	for (int k=lineNum; k<size; k++){
		file[k][0] = 32;
		for (int m=1; m < size; m++){
			file[k][m] = 0;
		}
	}
}

//reads encrypted file (numbers with spaces - square file)
void Crypter::readEncryptedFile(ifstream &infile)
{
	string line;
	int lineNum = 0;
	int j=0;
	int elt;
	while (getline(infile, line)){
		j =0;
		char *str = strdup(line.c_str());
	    char *word = strtok(str," ");
	    while (word != NULL){
	        string2int(word, elt);
	        file[lineNum][j] = elt;
	        j++;
	        word = strtok(NULL, " ");
	    }
	    free(str);
		lineNum++;
	}
}

//encrypts file - stores in original file
void Crypter::encrypt(string filename)
{
	key = permutateMatrix();
	createEncryptedFile(filename);
	printInfo(filename);
}

//decrypts file - stores in original file
void Crypter::decrypt(string filename)
{
	cout << "Please enter key to decrypt file: ";
	cin >> key;
	createDecryptedFile(filename);
	cout << "File: " << filename << " has been decrypted using given key."
		<< endl;
}

//creates key and permutates adjancency matrix
string Crypter::permutateMatrix()
{
	string pKey = "";
	srand(time(NULL));
	int numPermutations = rand() % size + 5;

	int row1, row2;
	int limit = size;
	for (int i=0; i < numPermutations; i++){
		row1 = rand() % limit;
		row2 = rand() % limit;
		if (row2 == row1){
			row2 = rand() % limit;
		}
		swapRows(row1, row2);
		swapColumns(row1, row2);
		pKey = to_string(row1) + "!" + to_string(row2) + "-" + pKey;
	}

	return pKey;
}

//swaps two rows in matrix
void Crypter::swapRows(int row1, int row2)
{
	int *temp;
	temp = file[row1];
	file[row1] = file[row2];
	file[row2] = temp;
}

//swaps two columns in matrix
void Crypter::swapColumns(int row1, int row2)
{
	int temp;
	for (int i=0; i < size; i++){
		temp = file[i][row1];
		file[i][row1] = file[i][row2];
		file[i][row2] = temp;
	}
}

//prints encrypted matrix to file
void Crypter::createEncryptedFile(string filename)
{
	ofstream outfile;
	outfile.open(filename);

	if (outfile.is_open()){
		for (int i=0; i < size; i++){
			for (int j=0; j < size; j++){
				outfile << file[i][j] << " ";
			}
			outfile << endl;
		}
	} else{
		cerr << "Unable to write to file" << endl;
		exit(1);
	}

	outfile.close();
}

//gives user info about encryption
void Crypter::printInfo(string filename)
{
	cout << "File: " << filename << " has been encrypted." << endl;
	cout << "Use key: " << key << " to decrypt this file." << endl;
}

//ensures size is >= 15
void Crypter::ensureSize()
{
	if (size < 15){
		size = 15;
	}
}

//prints decrypted matrix to file
void Crypter::createDecryptedFile(string filename)
{
	decryptMatrix();
	transformMatrix();
	ofstream outfile;
	outfile.open(filename);

	char c;
	if (outfile.is_open()){
		for (int i=0; i < size; i++){
			for (int j=0; j < size; j++){
				c = file[i][j];
				outfile << c;
			}
			outfile << endl;
		}
	} else{
		cerr << "Unable to write to file" << endl;
		exit(1);
	}

	outfile.close();
}

//decrypts matrix using key and permutations in key
void Crypter::decryptMatrix()
{
	int row1, row2, elt, i =0;

	char *str = strdup(key.c_str());
    char *word = strtok(str,"!-");
    while (word != NULL){
        string2int(word, elt);
        if (i % 2 == 0){
        	row1 = elt;
        } else{
        	row2 = elt;
        	swapColumns(row1, row2);
			swapRows(row1, row2);
        }
        i++;
        word = strtok(NULL, "!-");
    }
    free(str);
}

//transforms string to int
bool Crypter::string2int(const string& str, int& result)
{
    result = std::atoi(str.c_str());
    return true;
}

//adds second layer of "encoding" - each char based off of prev char
void Crypter::transformMatrix()
{
	int prevInt, currInt;
	for (int i=0; i < size; i++){
		prevInt = file[i][0];
		for (int j=1; j < size; j++){
			currInt = file[i][j];
			file[i][j] = prevInt - currInt;
			prevInt = file[i][j];
		}
	}
}

//prints adjancency matrix - for debugging
void Crypter::printMatrix()
{
	for (int i=0; i < size; i++){
		for (int j=0; j < size; j++){
			cout <<  file[i][j] << " ";
		}
		cout << endl;
	}
}


