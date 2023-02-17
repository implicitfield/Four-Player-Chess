#include "GUI.h"
#include <algorithm>

namespace GUI {

Painter::Painter(FPC::GameState& board, SDL_Renderer* renderer)
    : m_board(board)
    , m_renderer(renderer) {
}

static int get_cell_size() {
    int width = window_width / 14 * 2;
    int height = window_height / 14 * 2;
    return std::min(width, height);
}

static Color blend_color(Color first, Color second) {
    constexpr int scale = 2;
    return {(first.red + second.red) / scale, (first.green + second.green) / scale, (first.blue + second.blue) / scale};
}

Color Painter::get_piece_color(int x, int y) {
    if (!m_board.get_board()[x][y].piece.has_value() || !m_board.get_board()[x][y].color.has_value())
        return {0xFF, 0xFF, 0xFF};

    Color additive_color {0xFF, 0xFF, 0xFF};
    switch (m_board.get_board()[x][y].color.value()) {
        case FPC::Color::Red:
            additive_color.blue = 0x0;
            additive_color.green = 0x0;
            break;
        case FPC::Color::Blue:
            additive_color.red = 0x0;
            additive_color.green = 0x0;
            break;
        case FPC::Color::Yellow:
            additive_color.blue = 0x0;
            break;
        case FPC::Color::Green:
            additive_color.red = 0x0;
            additive_color.blue = 0x0;
            break;
    }
    switch (m_board.get_board()[x][y].piece.value()) {
        case FPC::Piece::Rook:
            return blend_color({0xEF, 0x29, 0x29}, additive_color); // Red
        case FPC::Piece::Bishop:
            return blend_color({0x8A, 0xE2, 0x34}, additive_color); // Green
        case FPC::Piece::King:
            return blend_color({0xFC, 0xE9, 0x4F}, additive_color); // Yellow
        case FPC::Piece::Queen:
            return blend_color({0x72, 0x9F, 0xCF}, additive_color); // Blue
        case FPC::Piece::Knight:
            return blend_color({0xAD, 0x7F, 0xA8}, additive_color); // Magenta
        case FPC::Piece::Pawn:
            return blend_color({0x34, 0xE2, 0xE2}, additive_color); // Cyan
    };
}

std::optional<FPC::Point> get_square_from_pixel(FPC::Point point) {
    const auto cell_size = get_cell_size();
    const auto board_width = 14 * cell_size;
    const auto board_height = 14 * cell_size;

    if (point.x <= (window_width / 4) + 12 || point.x >= board_width + (window_width / 4) + 12)
        return std::nullopt;
    if (point.y <= 12 || point.y >= 12 + board_height)
        return std::nullopt;
    auto output = FPC::Point {(point.x - ((window_width / 4) + 12)) / cell_size, (point.y - 12) / cell_size};
    // Filter out positions outside the board.
    if (!FPC::is_valid_position(output.x, output.y))
        return std::nullopt;
    return output;
}

void Painter::draw_board() {
    int cell_size = get_cell_size();
    for (size_t row = 0; row < 14; ++row) {
        for (size_t column = 0; column < 14; ++column) {
            if (!FPC::is_valid_position(row, column))
                continue;
            int cell_x = row * cell_size + (window_width / 4) + 12;
            int cell_y = column * cell_size + 12;

            SDL_Rect cell_rect {cell_x, cell_y, cell_size, cell_size};

            Color current_color = get_piece_color(row, column);
            SDL_SetRenderDrawColor(m_renderer, current_color.red, current_color.green, current_color.blue, 255);
            SDL_RenderFillRect(m_renderer, &cell_rect);
            if (cell_size > 4) {
                SDL_SetRenderDrawColor(m_renderer, 68, 68, 68, 255);
                SDL_RenderDrawRect(m_renderer, &cell_rect);
            }
        }
    }
}

void Painter::draw_valid_positions(FPC::Point position, FPC::Color player) const {
    int cell_size = get_cell_size();
    auto points = m_board.get_valid_moves_for_position(position, player);
    for (auto point : points) {
        SDL_SetRenderDrawColor(m_renderer, 68, 68, 68, 255);
        int point_x = point.x * cell_size + (window_width / 4) + 12;
        int point_y = point.y * cell_size + 12;
        SDL_Rect point_rect {point_x, point_y, cell_size, cell_size};
        SDL_RenderFillRect(m_renderer, &point_rect);
    }
}

void Painter::update(FPC::GameState& board) {
    m_board = board;
}

}
