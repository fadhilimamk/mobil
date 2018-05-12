all: make run
make:
	g++ src/main.cpp -o bin/main -lglfw -lGLEW -lGL -lassimp -I src -g -std=c++11
run:
	./bin/main
