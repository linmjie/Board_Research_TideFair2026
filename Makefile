CPP = g++
FLAGS = -std=c++20
ALL = board.o movegen.o boardutil.o 

output: $(ALL) 
	$(CPP) $(FLAGS) -o output $(ALL) main.o

board.o: Board.cpp
	$(CPP) $(FLAGS) -c Board.cpp -o board.o

movegen.o: MoveGen.cpp
	$(CPP) $(FLAGS) -c MoveGen.cpp -o movegen.o

boardutil.o: BoardUtil.cpp
	$(CPP) $(FLAGS) -c BoardUtil.cpp -o boardutil.o

main.o: main.cpp
	$(CPP) $(FLAGS) -c main.cpp -o main.o

clean:
	rm output $(ALL) 
