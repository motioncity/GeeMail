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

#include "validate.h"



static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

//Used to help the user view messages
int viewMessages(string userName)
{
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
    
    int count = 0;

    sql = "SELECT MESSAGEID,SENDER from MESSAGE where RECIPITENT = ?";    

    sqlite3_stmt *st;
    int rc2= sqlite3_prepare_v2(db, sql, -1, &st, NULL);

    sqlite3_bind_text(st, 1, userName.c_str(), -1, SQLITE_TRANSIENT);

   if (rc2 != SQLITE_OK) {
        printf("error: %s!\n", sqlite3_errmsg(db));
        return 0;
    }


    //This for loop takes all of the messages in the user's inbox and displays them
    for (;;) {
        rc2 = sqlite3_step(st);
        if (rc2 == SQLITE_DONE)
            break;
        if (rc2 != SQLITE_ROW) {
            printf("error: %s!\n", sqlite3_errmsg(db));
            break;
        }

        
        int currentID = sqlite3_column_int(st, 0);
        const char *sender = (const char *)sqlite3_column_text(st, 1);
        
        cout <<"MessageID: ";
        cout << currentID;
        cout << "  ";
        cout << " From: ";
        cout << sender ;
        cout << endl;
        cout<< "\n";
        count++;
    }
    

    cout<< "You have ";
    cout<<count ;
    cout << " messages in your inbox" <<endl;
    sqlite3_finalize(st);
    
    if(count == 0)
    {
        sqlite3_close(db);
        return 0;
    }

    int messageID = 0;
    
    cout << "Please enter the MessageID of the email you want to read" << endl;
    
    cin >> messageID;
    
    
    //Gets the noce and message body of the messaged with the inputted messageid
    sql = "SELECT MESSAGEBODY,NONCE from MESSAGE where MESSAGEID = ?";

    sqlite3_stmt *st2;
    sqlite3_prepare_v2(db, sql, -1, &st2, NULL);

    sqlite3_bind_int(st2, 1, messageID);

    if(sqlite3_step(st2) == SQLITE_ROW )
    {
        const char *cipher = (const char *)sqlite3_column_text(st2, 0);
        const char *nonce = (const char *)sqlite3_column_text(st2, 1);

        string passValue="";
        
        //Asks for passphrase which is then hashed and compared to the passphrase that is stored in database
        cout << "Please enter the passphrase from the recipitent" << endl;
        cin >> passValue ;
    
        string hashedPass = hashValue(passValue,1);
        string message = encrypt(cipher,hashedPass,nonce);
        cout << "Email:" << endl;
        cout << message << endl;
    }
    
    else 
    {
        cout << "MessageID not recognized" << endl;
        cout << "Do you want to try again? Press 1 for Y or any other key for N" << endl;
        
        int choice = 0;
        cin >> choice;
        
        if(choice == 1)
        {
            viewMessages(userName);
        }
        else
        {
            sqlite3_finalize(st2);
             sqlite3_close(db);
             cout <<"Logging out...." << endl;
            exit(0);
        }
    }
    
    
   sqlite3_finalize(st2);
   sqlite3_close(db);
   return 0;

}


//Responsible for sending messages
int sendMessage(string userName){
   sqlite3 *db;
   char *zErrMsg = 0;
   int  rc;
   const char * sql;
   
   string recValue;
   string passPhrase;
   string message;
   
   // Open database
   rc = sqlite3_open("test.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   }

 
   // Create SQL statement 
   /*
    sql = "CREATE TABLE MESSAGE("  \
        
         "MESSAGEID  INT PRIMARY KEY NOT NULL,"\
         "RECIPITENT TEXT  NOT NULL," \
         "SENDER    TEXT      NOT NULL," \
         "MESSAGEBODY TEXT    NOT NULL," \
         "NONCE     STRING     NOT NULL);";
  
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }
   else{
      fprintf(stdout, "Table created successfully\n");
   }
   */
   
   
    cout << "Please enter the username of the recipitent" << endl;
    cin.ignore();
    getline(cin,recValue);
    
   sql = "select USERNAME from USER WHERE USERNAME= ?";
   
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

// set the ? parameter to the thing you are looking for:

    sqlite3_bind_text(stmt, 1, recValue.c_str(), -1, SQLITE_TRANSIENT);
    
     
     //checks to see if inputted username is in database
     
     if(sqlite3_step(stmt) != SQLITE_ROW )
    {
        cout << "\n" << endl;
        cout << "This username is not registered in the database" << endl;
        cout << "Do you want to try again? Press 1 for Y or any other key for N" << endl;
            
        
        int choice = 0;
        cin >> choice;
        
        if(choice == 1)
        {
            sendMessage(userName);
        }
        else
        {
            cout <<"Logging out...." << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            exit(0);
        }
    }
    
    sqlite3_finalize(stmt);
    
    
    //Takes in the passphrase and hashes it
    cout << "Please enter an one word passphrase that the recipitent will recognize" << endl;

    cin >> passPhrase ;
    
    string hashedPass = hashValue(passPhrase,1);

    cout << "Please enter the message you want to send to the recipitent" << endl;

    cin.ignore();
    getline(cin,message);
    
    string nonce = generateNonce();
    
    string cipher = encrypt(message,hashedPass,nonce);
    int messageID = rand_num();
    
    //Makes a messageId which is a random number and other user values and creates a message record in the database
   sqlite3_stmt *st;
        sql = "INSERT INTO MESSAGE (MESSAGEID,RECIPITENT,SENDER,MESSAGEBODY,NONCE) VALUES (?,?,?,?,?)";

       sqlite3_prepare_v2(db, sql, -1, &st, NULL);
        sqlite3_bind_int(st, 1, messageID);
        sqlite3_bind_text(st, 2, recValue.c_str(), recValue.length(), SQLITE_TRANSIENT);
        sqlite3_bind_text(st, 3, userName.c_str(), userName.length(), SQLITE_TRANSIENT);
        sqlite3_bind_text(st, 4, cipher.c_str(), cipher.length(), SQLITE_TRANSIENT);
        sqlite3_bind_text(st, 5, nonce.c_str(), nonce.length(), SQLITE_TRANSIENT);
        sqlite3_step( st );
        
        sqlite3_finalize(st);
  
    
   cout << "Message has been successfully sent" << endl;
   
   sqlite3_close(db);
   
   return 0;
}


//Responsible for deciding whether the user will read or view messages
int handleMessages(string userName)
{
    cout << "\n" << endl;
    cout << "Welcome to geeMail " + userName +" please enter 1 to view messages or 2 to send messages or any other key to log out" << endl;
    
    int choice;

    cin >> choice;
    
    if(choice ==1)
    {
        viewMessages(userName);
        handleMessages(userName);
    }
    else if (choice ==2)
    {
        sendMessage(userName);
        handleMessages(userName);
    }
    else
    {
        cout << "Logging out...." << endl;
        exit(0);
    }
    
    return 0;
}
