# Iterators

## File Information
MyContainer.hpp - Has a custom container and iterators (See iterators below).
Animal.hpp - A custom class to test the iterators on a custom type.
Main.cpp - A main file that runs teh container.
test.cpp- A test file (Uses doctest.h).
makefile - has these commands:
main: Compiles the Main file and runs it.
test: Compiles the test file and runs it.
clean: removes the executables.
valgrind: runs valgrind on Main.

## Iterators

Order - The natural order: From the first element to the last
ReverseOrder - The opposite of the natural order: From the last element to the first
AcsendingOrder - The ascending order: From the smallest element to the biggest
DecsendingOrder - The descending order: From the biggest element to the smallest
SideCrossOrder - The sidecross order: Smallest, then biggest. Rince and repeat until the end of the vector
MiddleOutOrder - The MiddleOut order: The element in the middle, then to the left, then to the right, rince and repeat
