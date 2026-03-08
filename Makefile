CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O3 -DNDEBUG
TARGET = output 
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(OBJS) $(TARGET)
