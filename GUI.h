#pragma once

#include "SDL.h"
#include "library.h"
#include <optional>

namespace GUI {

constexpr int window_width = 1024;
constexpr int window_height = 768;

struct Color {
    int red;
    int green;
    int blue;
};

std::optional<FPC::Point> get_square_from_pixel(FPC::Point point);

class Painter {
public:
    Painter(FPC::GameState& board, SDL_Renderer *renderer);
    void draw_board();
    void update(FPC::GameState& board);
    void draw_valid_positions(FPC::Point position, FPC::Color player) const;
private:
    Color get_piece_color(int x, int y);
    FPC::GameState& m_board;
    SDL_Renderer *m_renderer;
};

}
