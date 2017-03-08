CC = g++
VPATH = src
CXXFLAG = --std=c++11 -I include -Ofast
HEADERS = $(wildcard client/*.h protocol/*.h)
OBJ = lpm.o
EXEC = lpm
LIBS = -lpthread
all : ll
%.o : %.cpp
	$(CC) -c $< -o $@ $(CXXFLAG)
ll : $(OBJ) $(HEADERS)
	$(CC) -o $(EXEC) $(OBJ) $(LIBS)
.PHONY : clean
clean :
	-rm $(EXEC) $(OBJ)

run : ll
	./$(EXEC)
