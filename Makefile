all: 
	g++ main.cpp -I src\include -L src\lib -Wall -lmingw32 -lSDL2main -lSDL2 -o main