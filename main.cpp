
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <string>
#include<iostream>

#include "logIn.h"
#include "register.h"

using namespace std;

int geeMail()
{
    
    cout << "Welcome to geeMail please enter 1 to register, 2 to login  or any other key to exit" << endl;
    
    int choice;

    cin >> choice;
    
    if(choice ==1)
    {
        registerUser();
        geeMail();
    }
    else if (choice ==2)
    {
        logInUser();
        geeMail();
    }
    else
    {
        cout << "Exiting geeMail...." << endl;
        exit(0);
    }
    
    return 0;
}

int main()
{
  geeMail();
  return 0;
}

