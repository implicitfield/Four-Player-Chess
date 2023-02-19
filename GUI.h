#pragma once

#include "SDL.h"
#include "library.h"
#include <array>
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
int get_cell_size();

class Painter {
public:
    Painter(FPC::GameState& board, SDL_Renderer* renderer);
    void draw_board();
    void update(FPC::GameState& board);
    bool draw_valid_positions(FPC::Point position, FPC::Color player) const;
    std::array<SDL_Rect, 4> draw_promotion_dialog(FPC::Point position, FPC::Color player) const;

private:
    Color get_piece_color(int x, int y);
    FPC::GameState& m_board;
    SDL_Renderer* m_renderer;
};

}
