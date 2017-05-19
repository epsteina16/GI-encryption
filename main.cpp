/* Aaron Epstein
* Copyright 2017
* Graph Isomorphism Encryption
* Symmetric Key Encryption
*/

#include <iostream>
#include <fstream>
#include <string>
#include "crypter.h"

using namespace std;

void performInput(string type, string filename);
void errorMessage();

int main(int argc, char *argv[])
{
	if (argc != 3){
		errorMessage();
	}
	performInput(argv[1], argv[2]);
	return 0;
}

//tests input and runs the encrypter/decrypter on given file
void performInput(string type, string filename)
{
	type = type.substr(1);
	if (!(type == "encrypt" || type == "decrypt")){
		errorMessage();
	}
	Crypter c(type, filename);
}

//prints usage error message to console
void errorMessage()
{
	cerr << "Usage: " << endl << "	./crypter -type filename"
			<< endl << " with type as encrypt or decrypt" << endl;
	exit(1);
}