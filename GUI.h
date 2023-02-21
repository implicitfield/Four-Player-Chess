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

class PositionCache {
public:
    PositionCache(FPC::Point m_position, FPC::Color m_player, const std::vector<FPC::Point>& cached_moves);
    const std::vector<FPC::Point>& get_cached_moves() const;
    const FPC::Point& get_cached_position() const;
    const FPC::Color& get_cached_player() const;

private:
    std::vector<FPC::Point> m_cached_moves;
    FPC::Point m_position;
    FPC::Color m_player;
};

class Painter {
public:
    Painter(FPC::GameState& board, SDL_Renderer* renderer);
    void draw_board();
    void update(FPC::GameState& board);
    bool draw_valid_positions(FPC::Point position, FPC::Color player);
    std::array<SDL_Rect, 4> draw_promotion_dialog(FPC::Point position, FPC::Color player) const;

private:
    Color get_piece_color(int x, int y);
    FPC::GameState& m_board;
    SDL_Renderer* m_renderer;
    std::optional<PositionCache> m_position_cache = std::nullopt;
};

}
