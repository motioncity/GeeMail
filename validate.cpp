#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string>
#include <iostream>

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


//Creates a random 8 byte array which is used for the salsa20 nonce
string generateNonce()
{
    AutoSeededRandomPool prng;
    
    byte nonce[8];
    
    prng.GenerateBlock(nonce, 8);

    string nonceString;
    
    nonceString.assign((char *) nonce);
    
    return nonceString;
}


//Uses salsa20 to encrypt the inputted message
string encrypt(string inputMessage, string hash,string nonceString)
{

    string message = inputMessage;
    
    string ciphertext="";
	byte *plaintextBytes = (byte *) message.c_str();
	byte *ciphertextBytes = new byte[message.length()];
	
	byte *nonce = (byte *)nonceString.c_str();
  
    byte key[32];
    {
        HexDecoder decoder;
        decoder.Put((byte*)hash.data(), hash.size());
        decoder.MessageEnd();
        decoder.Get(key, sizeof(key));
    }
    
    Salsa20::Encryption salsa;	
	salsa.SetKeyWithIV(key, 32, nonce);
	salsa.ProcessData(ciphertextBytes, plaintextBytes, message.length());
	ciphertext.assign((char *) ciphertextBytes);

	/*Output plaintext/ciphertext 
	cout << "Plaintext: " << message << endl;
	cout << "Ciphertext: " << ciphertext << endl;
	*/
	
	
	delete ciphertextBytes;
	

    return ciphertext;
}

//decrypts the message
string decrypt(string inputCipher, string hash, string nonceString)
{
   string cipher = inputCipher;
    
    string plaintext="";
	byte *ciphertextBytes = (byte *) cipher.c_str();
	byte *plaintextBytes = new byte[cipher.length()];
	
	byte *nonce = (byte *)nonceString.c_str();
	
    
    byte key[32];
    {
        HexDecoder decoder;
        decoder.Put((byte*)hash.data(), hash.size());
        decoder.MessageEnd();
        decoder.Get(key, sizeof(key));
    }
    
    Salsa20::Decryption salsa;	
	salsa.SetKeyWithIV(key, 32, nonce);
	salsa.ProcessData(plaintextBytes,ciphertextBytes, cipher.length());
	plaintext.assign((char *) plaintextBytes);

	//Output plaintext
    //cout << "Plaintext  Again:"+plaintext << endl;
    
    delete plaintextBytes;
    
    return plaintext;
}

//creates a random number between 1 and 100000
int rand_num()
{
 unsigned int randomN = 0;
 srand(unsigned(time(0)));
 randomN = rand()%100000;
 
 return randomN;
}

//Takes in a string and hashes the number of times equal to the randomNumber parameter
string hashValue(string passWord, int randomNumber)
{
  SHA256 sha;
  string source = passWord;  
  string hash = "";
  
   StringSource(source, true, new HashFilter(sha, new HexEncoder(new StringSink(hash))));

  for(int i =1; i < randomNumber;i++)
  {
      string tempHash = "";
      StringSource(hash, true, new HashFilter(sha, new HexEncoder(new StringSink(tempHash))));
      hash = tempHash;
  }
  return hash;
}

