#include "library.h"
#include <iostream>

namespace FPC {

bool is_valid_position(int row, int column) {
    if (row < 0 || column < 0 || row > 13 || column > 13)
        return false;
    if (((row == 0 || row == 1 || row == 2) && (column == 0 || column == 1 || column == 2))
        || ((row == 13 || row == 12 || row == 11) && (column == 13 || column == 12 || column == 11))
        || ((row == 0 || row == 1 || row == 2) && (column == 13 || column == 12 || column == 11))
        || ((row == 13 || row == 12 || row == 11) && (column == 0 || column == 1 || column == 2)))
        return false;
    return true;
}

GameState::GameState() {
    reset();
}

void GameState::reset() {
    // Setup red.
    for (int x = 3; x < 11; ++x) {
        for (int y = 12; y < 14; ++y) {
            m_board[x][y].color = Color::Red;
        }
    }
    m_board[3][13].piece = Piece::Rook;
    m_board[4][13].piece = Piece::Knight;
    m_board[5][13].piece = Piece::Bishop;
    m_board[6][13].piece = Piece::Queen;
    m_board[7][13].piece = Piece::King;
    m_board[8][13].piece = Piece::Bishop;
    m_board[9][13].piece = Piece::Knight;
    m_board[10][13].piece = Piece::Rook;
    for (int i = 3; i < 11; ++i)
        m_board[i][12].piece = Piece::Pawn;

    // Setup yellow.
    for (int x = 3; x < 11; ++x) {
        for (int y = 0; y < 2; ++y) {
            m_board[x][y].color = Color::Yellow;
        }
    }
    m_board[3][0].piece = Piece::Rook;
    m_board[4][0].piece = Piece::Knight;
    m_board[5][0].piece = Piece::Bishop;
    m_board[6][0].piece = Piece::King;
    m_board[7][0].piece = Piece::Queen;
    m_board[8][0].piece = Piece::Bishop;
    m_board[9][0].piece = Piece::Knight;
    m_board[10][0].piece = Piece::Rook;
    for (int i = 3; i < 11; ++i)
        m_board[i][1].piece = Piece::Pawn;

    // Setup blue.
    for (int x = 0; x < 2; ++x) {
        for (int y = 3; y < 11; ++y) {
            m_board[x][y].color = Color::Blue;
        }
    }
    m_board[0][3].piece = Piece::Rook;
    m_board[0][4].piece = Piece::Knight;
    m_board[0][5].piece = Piece::Bishop;
    m_board[0][6].piece = Piece::King;
    m_board[0][7].piece = Piece::Queen;
    m_board[0][8].piece = Piece::Bishop;
    m_board[0][9].piece = Piece::Knight;
    m_board[0][10].piece = Piece::Rook;
    for (int i = 3; i < 11; ++i)
        m_board[1][i].piece = Piece::Pawn;

    // Setup green.
    for (int x = 12; x < 14; ++x) {
        for (int y = 3; y < 11; ++y) {
            m_board[x][y].color = Color::Green;
        }
    }
    m_board[13][3].piece = Piece::Rook;
    m_board[13][4].piece = Piece::Knight;
    m_board[13][5].piece = Piece::Bishop;
    m_board[13][6].piece = Piece::Queen;
    m_board[13][7].piece = Piece::King;
    m_board[13][8].piece = Piece::Bishop;
    m_board[13][9].piece = Piece::Knight;
    m_board[13][10].piece = Piece::Rook;
    for (int i = 3; i < 11; ++i)
        m_board[12][i].piece = Piece::Pawn;
}

const std::array<std::array<Square, 14>, 14>& GameState::get_board() const {
    return m_board;
}

std::array<std::array<Square, 14>, 14>& GameState::get_board() {
    return m_board;
}

bool GameState::point_is_of_color(Point point, Color color) const {
    if (!m_board[point.x][point.y].color.has_value())
        return false;
    return m_board[point.x][point.y].color.value() == color;
}

void get_piece_name(const GameState& game, int x, int y) {
    if (!game.get_board()[x][y].piece.has_value()) {
        std::cout << "No value.\n";
        return;
    }
    switch (game.get_board()[x][y].piece.value()) {
        case Piece::Rook:
            std::cout << "Rook\n";
            break;
        case Piece::Bishop:
            std::cout << "Bishop\n";
            break;
        case Piece::King:
            std::cout << "King\n";
            break;
        case Piece::Queen:
            std::cout << "Queen\n";
            break;
        case Piece::Knight:
            std::cout << "Knight\n";
            break;
        case Piece::Pawn:
            std::cout << "Pawn\n";
            break;
    }
}

void GameState::iterate_from(std::vector<Point>& valid_moves, const Color player, const Point original_position, const Point increment_map) {
    int x = original_position.x;
    int y = original_position.y;
    while (is_valid_position(x += increment_map.x, y += increment_map.y)) {
        if (m_board[x][y].piece.has_value() && !point_is_of_color({x, y}, player)) {
            valid_moves.push_back({x, y});
            break;
        }
        // This is another piece that is owned by the player The path ends here.
        if (m_board[x][y].piece.has_value() && point_is_of_color({x, y}, player))
            break;

        valid_moves.push_back({x, y});
        continue;
    }
}

bool GameState::move_piece_to(const Point& origin, const Point& destination) {
    if (!m_board[origin.x][origin.y].piece.has_value() || !m_board[origin.x][origin.y].color.has_value() || !is_valid_position(origin.x, origin.y) || !is_valid_position(destination.x, destination.y))
        return false;
    bool is_valid_move = false;
    auto valid_moves = get_valid_moves_for_position(origin, m_board[origin.x][origin.y].color.value());
    for (auto move : valid_moves) {
        if (move.x == destination.x && move.y == destination.y)
            is_valid_move = true;
    }
    if (!is_valid_move)
        return false;
    m_board[destination.x][destination.y].piece = m_board[origin.x][origin.y].piece;
    m_board[destination.x][destination.y].color = m_board[origin.x][origin.y].color;
    m_board[origin.x][origin.y].piece = std::nullopt;
    m_board[origin.x][origin.y].color = std::nullopt;
    return true;
}

void GameState::advance_turn() {
    if (static_cast<int>(m_player) != 3)
        m_player = static_cast<Color>(static_cast<int>(m_player) + 1);
    else
        m_player = static_cast<Color>(0);
}

Color GameState::get_current_player() const {
    return m_player;
}

std::vector<Point> GameState::get_valid_moves_for_position(Point position, Color player) {
    if (!m_board[position.x][position.y].piece.has_value())
        return {};
    switch (m_board[position.x][position.y].piece.value()) {
        case Piece::Rook:
            return get_valid_moves_for_rook(position, player);
        case Piece::Bishop:
            return get_valid_moves_for_bishop(position, player);
        case Piece::King:
            return get_valid_moves_for_king(position, player);
        case Piece::Queen:
            return get_valid_moves_for_queen(position, player);
        case Piece::Knight:
            return get_valid_moves_for_knight(position, player);
        case Piece::Pawn:
            return get_valid_moves_for_pawn(position, player);
    }
}

std::vector<Point> GameState::get_valid_moves_for_rook(const Point position, const Color player) {
    std::vector<Point> valid_moves {};

    iterate_from(valid_moves, player, position, {1, 0});
    iterate_from(valid_moves, player, position, {-1, 0});
    iterate_from(valid_moves, player, position, {0, 1});
    iterate_from(valid_moves, player, position, {0, -1});

    return valid_moves;
}

std::vector<Point> GameState::get_valid_moves_for_bishop(Point position, Color player) {
    std::vector<Point> valid_moves {};
    iterate_from(valid_moves, player, position, {1, 1});
    iterate_from(valid_moves, player, position, {-1, -1});
    iterate_from(valid_moves, player, position, {-1, 1});
    iterate_from(valid_moves, player, position, {1, -1});

    return valid_moves;
}

std::vector<Point> GameState::get_valid_moves_for_king(Point position, Color player) {
    std::vector<Point> valid_moves {};
    for (int row = position.x - 1; row < position.x + 2; ++row) {
        for (int column = position.y - 1; column < position.y + 2; ++column) {
            if (is_valid_position(row, column) && (row != position.x || column != position.y) && !point_is_of_color({row, column}, player))
                // TODO: Verify that this isn't a position that is a valid target for other players (checkmate.)
                valid_moves.push_back({row, column});
        }
    }
    return valid_moves;
}

std::vector<Point> GameState::get_valid_moves_for_queen(Point position, Color player) {
    auto valid_moves = get_valid_moves_for_bishop(position, player);
    auto straight_moves = get_valid_moves_for_rook(position, player);
    valid_moves.insert(valid_moves.end(), straight_moves.begin(), straight_moves.end());
    return valid_moves;
}

std::vector<Point> GameState::get_valid_moves_for_knight(Point position, Color player) {
    std::vector<Point> valid_moves {};
    auto push_back_if_valid = [&](int x, int y) {
        if (is_valid_position(x, y) && !point_is_of_color({x, y}, player))
            valid_moves.push_back({x, y});
    };

    push_back_if_valid(position.x - 2, position.y - 1);
    push_back_if_valid(position.x - 1, position.y - 2);
    push_back_if_valid(position.x + 1, position.y - 2);
    push_back_if_valid(position.x + 2, position.y - 1);

    push_back_if_valid(position.x - 2, position.y + 1);
    push_back_if_valid(position.x - 1, position.y + 2);
    push_back_if_valid(position.x + 1, position.y + 2);
    push_back_if_valid(position.x + 2, position.y + 1);

    return valid_moves;
}

std::vector<Point> GameState::get_valid_moves_for_pawn(Point position, Color player) {
    std::vector<Point> valid_moves {};
    Point direction {};
    Axis capture_axis = Axis::X;
    bool may_double_jump = false;
    switch (player) {
        case Color::Red:
            direction = {0, -1};
            if (position.y == 12)
                may_double_jump = true;
            break;
        case Color::Blue:
            direction = {1, 0};
            capture_axis = Axis::Y;
            if (position.x == 1)
                may_double_jump = true;
            break;
        case Color::Yellow:
            direction = {0, 1};
            if (position.y == 1)
                may_double_jump = true;
            break;
        case Color::Green:
            direction = {-1, 0};
            capture_axis = Axis::Y;
            if (position.x == 12)
                may_double_jump = true;
    }

    auto push_back_if_valid = [&](Point offset) {
        if (is_valid_position(position.x + offset.x, position.y + offset.y) && !m_board[position.x + offset.x][position.y + offset.y].piece.has_value())
            valid_moves.push_back({position.x + offset.x, position.y + offset.y});
    };

    push_back_if_valid(direction);
    if (may_double_jump)
        push_back_if_valid({direction.x * 2, direction.y * 2});

    auto push_back_if_valid_capture = [&](Point offset) {
        Point onward {position.x + direction.x + offset.x, position.y + direction.y + offset.y};
        if (is_valid_position(onward.x, onward.y) && m_board[onward.x][onward.y].piece.has_value() && !point_is_of_color(onward, player))
            valid_moves.push_back(onward);
    };

    switch (capture_axis) {
        case Axis::X:
            push_back_if_valid_capture({1, 0});
            push_back_if_valid_capture({-1, 0});
            break;
        case Axis::Y:
            push_back_if_valid_capture({0, 1});
            push_back_if_valid_capture({0, -1});
            break;
    }

    return valid_moves;
}

}
