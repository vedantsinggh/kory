CXX := g++
CXXFLAGS := -Wall -std=c++2a -Wextra -Iinclude -O2

TARGET := app
SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/%.o: src/%.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o

run: all
	./$(TARGET)

.PHONY: all clean run
