burt-blocks:
	g++ -std=c++17 src/*.cpp -O3 -lSDL2 -lSDL2_image -lSDL2_ttf -o burt-blocks

burt-blocks-dev:
	g++ -std=c++17 src/*.cpp -g -Wall -lSDL2 -lSDL2_image -lSDL2_ttf -o burt-blocks-dev

build/burt-blocks.js:
	em++ -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -sASYNCIFY --preload-file assets src/*.cpp -O3 -o build/burt-blocks.js

build/burt-blocks.html:
	em++ -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -sASYNCIFY --preload-file assets src/*.cpp -O3 -o build/burt-blocks.html

clean:
	rm -rf *.dSYM
	rm -rf burt-blocks
	rm -rf burt-blocks-dev
	rm -rf build/burt-blocks.*
