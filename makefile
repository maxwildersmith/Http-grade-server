

server.o: src/server.cpp
	g++ src/server.cpp -std=c++2a -pthread -o server.out

server: server.o
	./server.out

teacher.o: src/teacher.cpp
	g++ src/teacher.cpp -std=c++2a -pthread -o teacher.out

teacher: teacher.o
	./teacher.out

student.o: src/student.cpp
	g++ src/student.cpp -std=c++2a -pthread -o student.out

student: student.o
	./student.out