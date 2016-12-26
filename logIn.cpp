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

#include "validate.h"
#include "messages.h"

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

//Function used to log in users
int logInUser(){
   sqlite3 *db;
   char *zErrMsg = 0;
   int  rc;
   const char * sql;
   
   string userValue;
   string passwordValue;
   

   // Open database
   rc = sqlite3_open("test.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   }
   
    cout << "Please username" << endl;
    cin >> userValue ;
    
   sql = "select USERNAME from USER WHERE USERNAME= ?";
   
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

// set the ? parameter to the thing you are looking for:

    sqlite3_bind_text(stmt, 1, userValue.c_str(), -1, SQLITE_TRANSIENT);
    
    //If the username is not registered
     if(sqlite3_step(stmt) != SQLITE_ROW )
    {
    cout << "\n" << endl;
    cout << "Username not registered" << std::endl;
    logInUser();
    }
    
    sqlite3_finalize(stmt);
    
    
    int salt = 0;
    
    sql = "SELECT SALT from USER where USERNAME = ?";    

    sqlite3_stmt *stmt2;
    sqlite3_prepare_v2(db, sql, -1, &stmt2, NULL);

    
    sqlite3_bind_text(stmt2, 1, userValue.c_str(), -1, SQLITE_TRANSIENT);
        
        //Looks for user name and then access the stored salt value
    if(sqlite3_step(stmt2) == SQLITE_ROW )
    {
        salt =sqlite3_column_int(stmt2,0);
    }

    sqlite3_finalize(stmt2);
    
    cout << "Please enter password" << endl;
    cin >> passwordValue ;
    
    //Hashes the inputted password and hashes salt amount of times to see if the output matches the password value that is in the database
    string hashedVal = hashValue(passwordValue,salt);
    passwordValue = hashedVal;
    
         sql = "SELECT * from USER where USERNAME = ? AND PASSWORD = ?";    

    sqlite3_stmt *stmt3;
    sqlite3_prepare_v2(db, sql, -1, &stmt3, NULL);


   
   sqlite3_bind_text(stmt3, 1, userValue.c_str(), userValue.length(), SQLITE_TRANSIENT);
   sqlite3_bind_text(stmt3, 2, passwordValue.c_str(), passwordValue.length(), SQLITE_TRANSIENT);
   
   
    //checks to see if the username and password value are in the database
    if(sqlite3_step(stmt3) == SQLITE_ROW )
    {
        cout << "Successfully logged in" << endl;
        cout << userValue << endl;
        sqlite3_finalize(stmt3);
        sqlite3_close(db);
        //If successful we can now view/send messages
        handleMessages(userValue);
    }
    else
    {
        cout << "\n" << endl;
        cout << "wrong password entered" << endl;
        cout << "Do you want to try again? Press 1 for Y or any other key for N" << endl;
        
        int choice = 0;
        cin >> choice;
        
        if(choice == 1)
        {
            logInUser();
        }
        else
        {
            sqlite3_finalize(stmt3);
             sqlite3_close(db);
             cout <<"Logging out...." << endl;
            exit(0);
        }
    }
    
   return 0;
}

