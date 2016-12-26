#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <algorithm>

#include <ctime> 
#include <cstdlib>

#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/salsa.h>
#include <cryptopp/osrng.h>


using namespace std;
using namespace CryptoPP;
using CryptoPP::AutoSeededRandomPool;

string generateNonce();
string encrypt(string inputMessage, string passPhrase,string nonceString);
string decrypt(string inputCipher, string passPhrase, string nonceString);
int rand_num();
string hashValue(string passWord, int randomNumber);




