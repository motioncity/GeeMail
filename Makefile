validate.o:validate.cpp
	 g++ -c -std=c++11 validate.cpp
	 
messages.o:messages.cpp
	g++ -c -std=c++11 messages.cpp
	
register.o:register.cpp
	g++ -c -std=c++11 register.cpp
	
logIn.o:logIn.cpp
	g++ -c -std=c++11 logIn.cpp
	
main.o:main.cpp
	g++ -c -std=c++11 main.cpp

main:validate.o messages.o register.o logIn.o main.o
	g++ -o main validate.o messages.o register.o logIn.o main.o -l sqlite3 -lcryptopp
	
