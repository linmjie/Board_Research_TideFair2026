CPP = g++
FLAGS =

output: board.o main.o
	$(CPP) $(FLAGS) -o output board.o main.o

main.o: main.cpp
	$(CPP) $(FLAGS) -c main.cpp -o main.o

board.o: Board.cpp
	$(CPP) $(FLAGS) -c Board.cpp -o board.o

clean:
	rm output board.o
