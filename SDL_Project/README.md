# To run code you created:

Example of compiling a file called hello.c into an executable called hello_sdl

## Check the flags
pkg-config --cflags --libs sdl3

## Compile and link
clang hello.c -o hello_sdl $(pkg-config --cflags --libs sdl3)

## Run the executable
./hello_sdl