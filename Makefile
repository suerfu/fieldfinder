CC = g++ #-pg # -g


CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(patsubst %.cpp, %.o, $(CPP_FILES))


CFLAGS = -Wall -std=c++0x -I./include $(root-config --cflags)
ROOT_LD_FLAGS = $(root-config --libs)


all : gauss2d

gauss2d : test/Gauss2D.cpp $(OBJ_FILES)
	@echo "compiling $@ using $^"
	@$(CC) $(CFLAGS) $^ -o $@ `root-config --libs`


%.o : %.cpp
	@echo "compiling $@"
	@$(CC) $(CFLAGS) -c $^ -o $@


clean:
	@echo "cleaning..."
	@-rm gauss2d > /dev/null 2>&1
	@-rm ${OBJ_FILES} > /dev/null 2>&1

