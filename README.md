# Burt-Blocks

A small tetris clone using C++ and SDL2.

## Building

### Clang

Requirements:

* SDL2
* SDL2_image
* SDL2_ttf

Build:

```sh
make
```

Development build:

```sh
make burt-blocks-dev
```

### Emscripten

Build:

```sh
make build/burt-blocks.js
```

Development build:

```sh
make build/burt-blocks.html
```

## Contributing

Format using `clang-format`:

```sh
 clang-format -i --style=mozilla src/*.h src/*.cpp
```