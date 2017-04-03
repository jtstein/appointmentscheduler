runServer: server.o functions.o user.o
	g++ -std=c++11 server.o functions.o user.o -o runServer

server.o: server.cpp
	g++ -c -std=c++11 server.cpp

functions.o: functions.cpp
	g++ -c -std=c++11 functions.cpp

user.o: user.cpp
	g++ -c -std=c++11 user.cpp

clean:
	rm *.o runServer
