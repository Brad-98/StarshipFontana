# Starship Fontana #

This is an example C++ application using the SDL library.
It tries to be as nicely C++11 as possible but do keep in
mind that SDL is written in C and, at some stage, you have
to interface with it.

## Story ##
The skeletons have stolen your coins, fight the skeletons back to recover your coins.

## Installation ##
You will have to have the SDL development libraries installed on
your system.  The easiest way to compile is to use a command-line

```bash
$ g++ -c src/*.cpp
$ g++ -o StarshipFontana *.o -lSDL2 -lSDL2_image
```

which will produce an executable file called "StarshipFontana" in the
top-level directory.  To execute this file do the following

`$ ./StarshipFontana`
 
from the top-level directory.  The game will expect to find the
`assets` directory under its current working directory.

## Credits ##
The sprites in this game come directly from (https://opengameart.org/)

All of the authors of the sprites have published them without any copyright claims on them.

