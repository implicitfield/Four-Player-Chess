#pragma once

#include "SDL.h"
#include "library.h"

namespace GUI {

constexpr int window_width = 1024;
constexpr int window_height = 768;


struct Color {
    int red;
    int green;
    int blue;
};

class Painter {
public:
    Painter(FPC::GameState& board, SDL_Renderer *renderer);
    void draw_board();
    void update(FPC::GameState& board);
private:
    Color get_piece_color(int x, int y);
    FPC::GameState& m_board;
    SDL_Renderer *m_renderer;
};

}
