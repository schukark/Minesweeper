all: main

main: main.o GUI.o Menu.o Board.o
	g++ -I src\include -Wall $^ -o $@ -L src\lib -lmingw32 -lSDL2main -lSDL2

%.o: %.cpp
	g++ -I src\include -Wall -c -o $^ $@  -L src\lib -lmingw32 -lSDL2main -lSDL2

clean:
	del *.exe *.o all