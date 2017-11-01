**GeeMail**

An asynchronous email platform that allows for the safe and secure sending of messages between user accounts. The values are stored in a sql lite database and 
the cryptopp library is used for all of the cryptographic algorithms. 

The usernames are stored in plaintext however passwords are taken and then hashed a random number of times using the sha256 hash functions(that cryptopp provides)
and then stored in the database. 

Messages are encrypted using the salsa20 stream cipher and then stored in the databse. 

**Installation**
In order for this to work properly the cryptopp and sql lite libraries need to be installed on your machine. 

#
Commands to install cryptopp(in terminal): 

```sudo apt-get update
sudo apt-get install libcrypto++-dev libcrypto++-doc libcrypto++-utils
```
#

Commands to install sqllite

```sudo apt-get install sqlite3```

Once these are installed, in order to compile you must run: 

```make main```

In order to start the geeMail program you must run:

```./main ```

With this platform you are able to create a user account, log into an user account, send messages to other users and view all messages in inbox. 

When sending a message you will be prompted for a passphrase which must be entered correctly by the message recipitent when they go to view their messages,
to ensure that the messages decrypt correctly. If the wrong passphrase is entered then the message will decrypt incorrectly giving you a 
nonsense message. 
