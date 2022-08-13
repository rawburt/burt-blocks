build:
	g++ -std=c++17 src/*.cpp -O3 -lSDL2 -lSDL2_image -lSDL2_ttf -o burt-blocks

dev:
	g++ -std=c++17 src/*.cpp -g -Wall -lSDL2 -lSDL2_image -lSDL2_ttf -o burt-blocks-dev

em-dev:
	em++ -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -sASYNCIFY --preload-file assets src/*.cpp -O3 -o em/burt-blocks.html

clean:
	rm -rf *.dSYM
	rm -rf burt-blocks
	rm -rf burt-blocks-dev
	rm -rf em/
