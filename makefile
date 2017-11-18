all:
	g++ -std=c++11 -c utilities.cpp -o utilities.o
	g++ -std=c++11 -o client client.cpp utilities.o
	g++ -std=c++11 -o server server.cpp utilities.o
