#include "GUI.h"
#include <algorithm>

namespace GUI {
Painter::Painter(FPC::GameState& board, SDL_Renderer *renderer)
    : m_board(board)
    , m_renderer(renderer)
{}

static int get_cell_size() {
    int width = window_width / 14 * 2;
    int height = window_height / 14 * 2;
    return std::min(width, height);
}

Color Painter::get_piece_color(int x, int y) {
    if (!m_board.get_board()[x][y].piece.has_value())
        return {0xFF, 0xFF, 0xFF};
    switch(m_board.get_board()[x][y].piece.value()) {
        case FPC::Piece::Rook:
            return {0xCC, 0x00, 0x00};
        case FPC::Piece::Bishop:
            return {0x3E, 0x9A, 0x06};
        case FPC::Piece::King:
            return {0xC4, 0xA0, 0x00};
        case FPC::Piece::Queen:
            return {0x24, 0x65, 0xA4};
        case FPC::Piece::Knight:
            return {0x75, 0x50, 0x7B};
        case FPC::Piece::Pawn:
            return {0x06, 0x98, 0x9A};
    };
}

void Painter::draw_board() {
    int cell_size = get_cell_size();
    for (size_t row = 0; row < 14; ++row) {
        for (size_t column = 0; column < 14; ++column) {
            if (!FPC::is_valid_position(row, column))
                continue;
            int cell_x = column * cell_size + (window_width / 4) + 12;
            int cell_y = row * cell_size + 12;

            SDL_Rect cell_rect{cell_x, cell_y, cell_size, cell_size};
            
            Color current_color = get_piece_color(row, column);
            SDL_SetRenderDrawColor(m_renderer, current_color.red, current_color.green, current_color.blue, 255);
            SDL_RenderFillRect(m_renderer, &cell_rect);
            if (cell_size > 4) {
                SDL_SetRenderDrawColor(m_renderer, 68, 68, 68, 255);
                SDL_RenderDrawRect(m_renderer, &cell_rect);
            }
            // -- Start of debug code --
            auto points = m_board.get_valid_moves_for_queen({7, 7}, FPC::Color::Red);
            for(auto point : points) {
                SDL_SetRenderDrawColor(m_renderer, 68, 68, 68, 255);
                int point_x = point.x * cell_size + (window_width / 4) + 12;
                int point_y = point.y * cell_size + 12;
                SDL_Rect point_rect{point_x, point_y, cell_size, cell_size};
                SDL_RenderFillRect(m_renderer, &point_rect);
            }
            // -- End of debug code --
        }
    }
}
void Painter::update(FPC::GameState& board) {
    m_board = board;
}
}
