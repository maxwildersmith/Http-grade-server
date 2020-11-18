

server.o: src/server.cpp
	g++ src/server.cpp -std=c++2a -o server.out

server: server.o
	./server.out

