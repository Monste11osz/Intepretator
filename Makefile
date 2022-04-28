all:
	g++ usr/example.cpp src/interpreter.cpp -I header/ -o bin/example -fmax-errors=3
x
