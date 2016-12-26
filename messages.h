#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>

#include <ctime> 
#include <cstdlib>

#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

int viewMessages(string userName);
int sendMessage(string userName);
int handleMessages(string userName);