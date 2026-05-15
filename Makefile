CXX = g++

# -Iinclude tells the compiler to search the include/ folder for header files

CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC = src/main.cpp src/game.cpp

TARGET = snake

all: $(TARGET)

$(TARGET): $(SRC)

	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)

	./$(TARGET)

clean:

	rm -f $(TARGET)
