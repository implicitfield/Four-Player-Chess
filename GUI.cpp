#include "GUI.h"
#include <algorithm>
#include <exception>
#include <iostream>

namespace GUI {

Painter::Painter(FPC::GameState& board, SDL_Window* window, int window_height, int window_width)
    : m_board(board)
    , m_window(window)
    , m_window_height(window_height)
    , m_window_width(window_width) {
    m_screen_surface = SDL_GetWindowSurface(m_window);
    if (!m_screen_surface) {
        std::cout << "Screen surface could not be created!\nSDL_Error: " << SDL_GetError() << '\n';
        std::terminate();
    }
}

int Painter::get_cell_size() {
    return std::min(m_window_width / 14, m_window_height / 14);
}

PositionCache::PositionCache(FPC::Point position, FPC::Color player, const std::vector<FPC::Point>& cached_moves) {
    m_position = position;
    m_player = player;
    m_cached_moves = cached_moves;
}

const std::vector<FPC::Point>& PositionCache::get_cached_moves() const {
    return m_cached_moves;
}

const FPC::Point& PositionCache::get_cached_position() const {
    return m_position;
}

const FPC::Color& PositionCache::get_cached_player() const {
    return m_player;
}

std::string Painter::get_path_to_piece_image(int x, int y) {
    if (!m_board.get_board()[x][y].piece.has_value() || !m_board.get_board()[x][y].color.has_value())
        return "assets/grey_square.svg";

    std::string path = "assets/shapes/";

    if (!m_board.player_exists(m_board.get_board()[x][y].color.value())) {
        path.append("grey");
    } else {
        switch (m_board.get_board()[x][y].color.value()) {
            case FPC::Color::Red:
                path.append("r");
                break;
            case FPC::Color::Blue:
                path.append("b");
                break;
            case FPC::Color::Yellow:
                path.append("y");
                break;
            case FPC::Color::Green:
                path.append("g");
                break;
        }
    }

    switch (m_board.get_board()[x][y].piece.value()) {
        case FPC::Piece::Rook:
            path.append("R.svg");
            break;
        case FPC::Piece::Bishop:
            path.append("B.svg");
            break;
        case FPC::Piece::King:
            path.append("K.svg");
            break;
        case FPC::Piece::Queen:
            path.append("Q.svg");
            break;
        case FPC::Piece::Knight:
            path.append("N.svg");
            break;
        case FPC::Piece::Pawn:
            path.append("P.svg");
            break;
    };

    return path;
}

SDL_Surface* Painter::load_svg(std::string path, int width, int height) {
    for (const auto& [cached_path, cached_surface] : m_image_cache) {
        if (cached_path == path)
            return cached_surface;
    }

    SDL_RWops* file = SDL_RWFromFile(path.c_str(), "rb");
    if (!file) {
        std::cout << "Image file could not be opened!\nSDL_Error: " << IMG_GetError() << '\n';
        return nullptr;
    }

    SDL_Surface* image = IMG_LoadSizedSVG_RW(file, width, height);
    if (!image) {
        std::cout << "Image could not be loaded!\nSDL_Error: " << IMG_GetError() << '\n';
        return nullptr;
    }
    SDL_RWclose(file);

    m_image_cache[path] = image;
    return image;
}

std::optional<FPC::Point> Painter::get_square_from_pixel(FPC::Point point) {
    const auto cell_size = get_cell_size();

    const auto output = FPC::Point {(point.x - (m_window_width - 14 * cell_size) / 2) / cell_size, (point.y - (m_window_height - 14 * cell_size) / 2) / cell_size};
    // Filter out positions outside of the board.
    if (!FPC::is_valid_position(output))
        return std::nullopt;
    return output;
}

void Painter::draw_board() {
    const int cell_size = get_cell_size();
    SDL_FillRect(m_screen_surface, nullptr, SDL_MapRGB(m_screen_surface->format, 255, 255, 255));
    for (int row = 0; row < 14; ++row) {
        for (int column = 0; column < 14; ++column) {
            if (!FPC::is_valid_position({row, column}))
                continue;
            int cell_x = row * cell_size + (m_window_width - 14 * cell_size) / 2;
            int cell_y = column * cell_size + (m_window_height - 14 * cell_size) / 2;

            SDL_Rect cell_rect {cell_x, cell_y, cell_size, cell_size};

            std::string piece_image_path = get_path_to_piece_image(row, column);

            SDL_BlitSurface(load_svg("assets/grey_square.svg", cell_size, cell_size), nullptr, m_screen_surface, &cell_rect);
            if (piece_image_path != "assets/grey_square.svg")
                SDL_BlitSurface(load_svg(piece_image_path, cell_size, cell_size), nullptr, m_screen_surface, &cell_rect);
        }
    }
}

bool Painter::draw_valid_positions(FPC::Point position, FPC::Color player) {
    std::vector<FPC::Point> points;
    if (!m_position_cache.has_value() || m_position_cache.value().get_cached_position() != position || m_position_cache.value().get_cached_player() != player) {
        points = m_board.get_valid_moves_for_position(position, player, true);
        m_position_cache = {position, player, points};
    } else
        points = m_position_cache.value().get_cached_moves();

    int cell_size = get_cell_size();
    if (points.empty())
        return false;
    for (const auto& point : points) {
        int point_x = point.x * cell_size + (m_window_width - 14 * cell_size) / 2;
        int point_y = point.y * cell_size + (m_window_height - 14 * cell_size) / 2;
        SDL_Rect point_rect {point_x, point_y, cell_size, cell_size};
        SDL_Surface* black_square = load_svg("assets/black_square.svg", cell_size, cell_size);
        SDL_BlitSurface(black_square, nullptr, m_screen_surface, &point_rect);
    }
    return true;
}

std::array<SDL_Rect, 4> Painter::draw_promotion_dialog(FPC::Point position, FPC::Color player) {
    std::array<SDL_Rect, 4> promotion_selection {};
    const int cell_size = get_cell_size();
    std::string piece_image_path = "assets/shapes/";
    switch (player) {
        case FPC::Color::Blue:
            piece_image_path.append("b");
            --position.x;
            if (position.y > 7)
                position.y = 7;
            break;
        case FPC::Color::Green:
            piece_image_path.append("g");
            ++position.x;
            if (position.y > 7)
                position.y = 7;
            break;
        case FPC::Color::Red:
            piece_image_path.append("r");
            if (position.x == 3)
                ++position.x;
            else
                --position.x;
            position.y = 0;
            break;
        case FPC::Color::Yellow:
            piece_image_path.append("y");
            if (position.x == 10)
                --position.x;
            else
                ++position.x;
            position.y = 10;
    }
    auto piece_char = [](FPC::Piece piece) -> std::string {
        switch (piece) {
            case FPC::Piece::Queen:
                return "Q";
            case FPC::Piece::Rook:
                return "R";
            case FPC::Piece::Bishop:
                return "B";
            case FPC::Piece::Knight:
                return "N";
            default:
                std::terminate();
        }
    };
    for (int i = 0; i < 4; ++i) {
        const FPC::Point screen_position {position.x * cell_size + (m_window_width - 14 * cell_size) / 2, position.y * cell_size + (m_window_height - 14 * cell_size) / 2};
        SDL_Rect point_rect {screen_position.x, screen_position.y, cell_size, cell_size};
        promotion_selection[i] = point_rect;
        SDL_Surface* black_square = load_svg("assets/black_square.svg", cell_size, cell_size);
        SDL_Surface* piece_image = load_svg(piece_image_path + piece_char(static_cast<FPC::Piece>(i)) + ".svg", cell_size, cell_size);
        SDL_BlitSurface(black_square, nullptr, m_screen_surface, &point_rect);
        SDL_BlitSurface(piece_image, nullptr, m_screen_surface, &point_rect);
        ++position.y;
    }

    return promotion_selection;
}

void Painter::update_window_size(int height, int width) {
    if (height <= 0 || width <= 0)
        return;
    m_image_cache = {};
    m_window_height = height;
    m_window_width = width;
}

void Painter::update(FPC::GameState& board) {
    m_board = board;
}

void Painter::refresh_surface() {
    m_screen_surface = SDL_GetWindowSurface(m_window);
}

}
