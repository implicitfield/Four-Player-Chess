# Four Player Chess

This repository contains a C++ library and a somewhat rudimentary GUI for playing four player chess.

# Details

Rules implemented by the library:
- All of the standard rules of chess apply unless they are contradicted by the following rules.
- The game is a free-for-all.
- Promotion occurs once a pawn reaches the opposite player's first square.
- The winner is the last player remaining. (No points are calculated.)
- A player is eliminated once their king is either checkmated or stalemated.

# Building

To build a standalone version of the library, simply compile ```library.cpp``` with a C++17-compliant compiler.
To build the GUI, you will need the following libraries:
- ```SDL2```
- ```SDL_image 2.x```

Once those are installed, run ```./build.sh```.
If you want to test an even more rudimentary GUI, or don't want SDL_image, check out ```0ed863f```, or an even earlier commit.

# License
Everything in ```assets/shapes```:
CC-SA-4.0 (see ```assets/shapes/LICENSE.html```, and check out the [original](https://github.com/flugsio/chess_shapes) piece set.) Note that the SVGs included here are based on the minified versions from [lila](https://github.com/lichess-org/lila/tree/master/public/piece/shapes).

Everything else:
MIT (see ```LICENSE.txt```.)

# Additional credits
SerenityOS, for the color scheme used by the pieces and for the basis of ```.clang_format```.

# Miscellaneous

## Changing the piece set

1. Copy a new set to ```assets/shapes``` with filenames conforming to the current naming scheme.
2. Run ```node ../../colorgen.js <hex-color> <character>``` with suitable values until all of the required colors have been generated. (You might need to tweak the script for it to correctly replace the colors of the new pieces.)

## Contributing

Be sure to run ```clang-format```, and make sure that your code fits the theme of the project.
