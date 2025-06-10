CXX = g++
CFLAGS = -Wall -g

MAIN_SRC = Main.cpp
TEST_SRC = test.cpp
HEADER = MyContainer.hpp

MAIN_TARGET = Main
TEST_TARGET = test

all: $(MAIN_TARGET) $(TEST_TARGET)

$(MAIN_TARGET): $(MAIN_SRC) $(HEADER)
	$(CXX) $(CFLAGS) -o $(MAIN_TARGET) $(MAIN_SRC)

$(TEST_TARGET): $(TEST_SRC) $(HEADER)
	$(CXX) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SRC)

clean:
	rm -f $(MAIN_TARGET) $(TEST_TARGET)

valgrind:
	valgrind --leak-check=full ./$(MAIN_TARGET)