Stack: main.o stack.o
	g++ main.o stack.o -o Stack # -lasan -fsanitize=address -ggdb3

main.o: main.cpp
	g++ -c main.cpp -o main.o # -lasan -fsanitize=address -ggdb3

stack.o: stack.cpp
	g++ -c stack.cpp -o stack.o # -lasan -fsanitize=address -ggdb3