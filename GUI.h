#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "library.h"
#include <array>
#include <optional>
#include <string>
#include <unordered_map>

namespace GUI {

struct Color {
    int red;
    int green;
    int blue;
};

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
    Painter(FPC::GameState& board, SDL_Window* window, int window_height, int window_width);
    void draw_board();
    void update(FPC::GameState& board);
    bool draw_valid_positions(FPC::Point position, FPC::Color player);
    std::array<SDL_Rect, 4> draw_promotion_dialog(FPC::Point position, FPC::Color player);
    void refresh_surface();
    int get_window_width() { return m_window_width; };
    int get_window_height() { return m_window_height; };
    void update_window_size(int width, int height);
    int get_cell_size();
    std::optional<FPC::Point> get_square_from_pixel(FPC::Point point);

private:
    std::string get_path_to_piece_image(int x, int y);
    SDL_Surface* load_svg(std::string path, int width, int height);
    FPC::GameState& m_board;
    SDL_Window* m_window;
    SDL_Surface* m_screen_surface;
    std::optional<PositionCache> m_position_cache = std::nullopt;
    std::unordered_map<std::string, SDL_Surface*> m_image_cache;
    int m_window_height;
    int m_window_width;
};

struct GUIState {
    GUI::Painter* painter = nullptr;
    FPC::GameState* game = nullptr;
    std::array<SDL_Rect, 4> promotion_selection {};
    FPC::Point square {};
    bool draw_positions = false;
    bool promotion_dialog_active = false;
};

}
