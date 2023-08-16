#OBJS specifies which files to compile as part of the project
OBJS = main.o

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -std=c++17

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -L src\lib

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

LIB_FLAGS = -I src\include

#This is the target that compiles our executable

all: build

build: $(OBJS)
	$(CC) $(OBJS) $(LIB_FLAGS) $(LINKER_FLAGS) $(COMPILER_FLAGS) -o $(OBJ_NAME)

%.o: %.cpp
	$(CC) $(LIB_FLAGS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -c $^ -o $@

exec: build
	./main

clean:
	del classes\*.o *.exe main.o