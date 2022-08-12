build:
	g++ -std=c++17 src/*.cpp -O3 -lSDL2 -lSDL2_image -lSDL2_ttf -o burt-blocks

dev:
	g++ -std=c++17 src/*.cpp -g -Wall -lSDL2 -lSDL2_image -lSDL2_ttf -o burt-blocks-dev

clean:
	rm -rf *.dSYM
	rm -rf burt-blocks
	rm -rf burt-blocks-dev
