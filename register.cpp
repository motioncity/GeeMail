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

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

//Function used to register users
int registerUser(){
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

/* 
   // Create SQL statement 
   const char * sql = "CREATE TABLE USER("  \
         "USERNAME TEXT PRIMARY KEY     NOT NULL," \
         "PASSWORD TEXT    NOT NULL," \
         "SALT     INT     NOT NULL);";
   // Execute SQL statement 
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }
   else{
      fprintf(stdout, "Table created successfully\n");
   }
   */
   
    cout << "Please enter an username" << endl;
    cin >> userValue ;
    
   sql = "select USERNAME from USER WHERE USERNAME= ?";
   
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);



    sqlite3_bind_text(stmt, 1, userValue.c_str(), -1, SQLITE_TRANSIENT);
    

    //Checks to see if the inputted username is already registered
     if(sqlite3_step(stmt) == SQLITE_ROW )
    {
        cout << "\n" << endl;
        cout << "Username already registered" << endl;
        cout << "Do you want to try again? Press 1 for Y or any other key for N" << endl;
        
        int choice = 0;
        cin >> choice;
        
        if(choice == 1)
        {
            registerUser();
        }
        else
        {
            cout <<"Logging out...." << endl;
            sqlite3_close(db);
            exit(0);
        }
    }
    
    sqlite3_finalize(stmt);
    
    
    //Takes the password value and hashes it a random amount of times
    
     cout << "Please enter a password" << endl;
    cin >> passwordValue ;
    
    int salt = 0;
    salt = rand_num();
    
    string hashedVal = hashValue(passwordValue,salt);
    passwordValue = hashedVal;
    
     
   sqlite3_stmt *st;
        sql = "INSERT INTO USER (USERNAME,PASSWORD,SALT) VALUES (?,?,?)";
        
        
        //Takes in the input and creates a user with the values inputted
       sqlite3_prepare_v2(db, sql, -1, &st, NULL);
        sqlite3_bind_text(st, 1, userValue.c_str(), userValue.length(), SQLITE_TRANSIENT);
        sqlite3_bind_text(st, 2, passwordValue.c_str(), passwordValue.length(), SQLITE_TRANSIENT);
        sqlite3_bind_int(st, 3, salt);
        sqlite3_step( st );
        
        sqlite3_finalize(st);
    
    
   cout << "User has sucessfully been registered" << endl;
   sqlite3_close(db);
   return 0;
}
