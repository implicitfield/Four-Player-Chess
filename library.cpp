#include "library.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace FPC {

bool is_valid_position(const FPC::Point& position) {
    if (position.x < 0 || position.y < 0 || position.x > 13 || position.y > 13)
        return false;
    if ((position.x < 3 && position.y < 3)
        || (position.x > 10 && position.y > 10)
        || (position.x < 3 && position.y > 10)
        || (position.x > 10 && position.y < 3))
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

    m_king_positions[0] = {7, 13};

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

    m_king_positions[1] = {0, 6};

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

    m_king_positions[2] = {6, 0};

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

    m_king_positions[3] = {13, 7};
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

void GameState::iterate_from(std::vector<Point>& valid_moves, const Color player, const Point original_position, const Point increment_map) const {
    int x = original_position.x;
    int y = original_position.y;
    while (is_valid_position({x += increment_map.x, y += increment_map.y})) {
        if (m_board[x][y].piece.has_value() && !point_is_of_color({x, y}, player)) {
            valid_moves.push_back({x, y});
            break;
        }
        // This is another piece that is owned by the player. The path ends here.
        if (m_board[x][y].piece.has_value() && point_is_of_color({x, y}, player))
            break;

        valid_moves.push_back({x, y});
        continue;
    }
}

bool GameState::empty_square(const Point& square) {
    if (!is_valid_position(square))
        return false;
    m_board[square.x][square.y].has_moved = false;
    m_board[square.x][square.y].piece = std::nullopt;
    m_board[square.x][square.y].color = std::nullopt;
    return true;
}

bool GameState::may_promote(const Point& position, const Color& player) const {
    const bool piece_has_value_and_is_a_pawn = (m_board[position.x][position.y].piece.has_value() && m_board[position.x][position.y].piece.value() == Piece::Pawn);
    const bool color_has_value_and_is_owned_by_player = (m_board[position.x][position.y].color.has_value() && m_board[position.x][position.y].color.value() == player);
    if (!is_valid_position(position) || !piece_has_value_and_is_a_pawn || !color_has_value_and_is_owned_by_player)
        return false;
    switch (player) {
        case Color::Red:
            if (position.y == 0)
                return true;
            return false;
        case Color::Blue:
            if (position.x == 13)
                return true;
            return false;
        case Color::Yellow:
            if (position.y == 13)
                return true;
            return false;
        case Color::Green:
            if (position.x == 0)
                return true;
            return false;
    }
}

bool GameState::move_piece_to(const Point& origin, const Point& destination, bool enforce_king_protection) {
    if (!m_board[origin.x][origin.y].piece.has_value() || !m_board[origin.x][origin.y].color.has_value() || !is_valid_position(origin) || !is_valid_position(destination))
        return false;
    bool is_valid_move = false;
    auto valid_moves = get_valid_moves_for_position(origin, m_board[origin.x][origin.y].color.value(), enforce_king_protection);
    for (auto move : valid_moves) {
        if (move == destination)
            is_valid_move = true;
    }
    if (!is_valid_move)
        return false;
    auto initial_origin_piece = m_board[origin.x][origin.y].piece;
    auto initial_destination_piece = m_board[destination.x][destination.y].piece;
    m_board[destination.x][destination.y].piece = m_board[origin.x][origin.y].piece;
    m_board[destination.x][destination.y].color = m_board[origin.x][origin.y].color;
    m_board[destination.x][destination.y].has_moved = true;
    empty_square(origin);

    if (m_board[destination.x][destination.y].piece == FPC::Piece::Pawn) {
        if (std::max(origin.x, destination.x) - std::min(origin.x, destination.x) == 2 || std::max(origin.y, destination.y) - std::min(origin.y, destination.y) == 2)
            m_board[destination.x][destination.y].just_double_jumped = true;
    }

    // Check for "en passant"
    if (initial_origin_piece.value() == FPC::Piece::Pawn && !initial_destination_piece.has_value()) {
        if ((m_player == Color::Blue || m_player == Color::Green) && origin.y != destination.y) {
            m_board[origin.x][destination.y].piece = std::nullopt;
            m_board[origin.x][destination.y].color = std::nullopt;
        } else if ((m_player == Color::Red || m_player == Color::Yellow) && origin.x != destination.x) {
            m_board[destination.x][origin.y].piece = std::nullopt;
            m_board[destination.x][origin.y].color = std::nullopt;
        }
    }

    // Check for castling
    if (m_board[destination.x][destination.y].piece == FPC::Piece::King && (std::abs(destination.x - origin.x) == 2 || std::abs(destination.y - origin.y) == 2)) {
        switch (m_player) {
            case Color::Red:
                if (destination.x == 5) {
                    empty_square({3, 13});
                    m_board[6][13].piece = Piece::Rook;
                    m_board[6][13].color = Color::Red;
                    m_board[6][13].has_moved = true;
                } else if (destination.x == 9) {
                    empty_square({10, 13});
                    m_board[8][13].piece = Piece::Rook;
                    m_board[8][13].color = Color::Red;
                    m_board[8][13].has_moved = true;
                }
                break;
            case Color::Blue:
                if (destination.y == 4) {
                    empty_square({0, 3});
                    m_board[0][5].piece = Piece::Rook;
                    m_board[0][5].color = Color::Blue;
                    m_board[0][5].has_moved = true;
                } else if (destination.y == 8) {
                    empty_square({0, 10});
                    m_board[0][7].piece = Piece::Rook;
                    m_board[0][7].color = Color::Blue;
                    m_board[0][7].has_moved = true;
                }
                break;
            case Color::Yellow:
                if (destination.x == 4) {
                    empty_square({3, 0});
                    m_board[5][0].piece = Piece::Rook;
                    m_board[5][0].color = Color::Yellow;
                    m_board[5][0].has_moved = true;
                } else if (destination.x == 8) {
                    empty_square({10, 0});
                    m_board[7][0].piece = Piece::Rook;
                    m_board[7][0].color = Color::Yellow;
                    m_board[7][0].has_moved = true;
                }
                break;
            case Color::Green:
                if (destination.y == 5) {
                    empty_square({13, 3});
                    m_board[13][6].piece = Piece::Rook;
                    m_board[13][6].color = Color::Green;
                    m_board[13][6].has_moved = true;
                } else if (destination.y == 9) {
                    empty_square({13, 10});
                    m_board[13][8].piece = Piece::Rook;
                    m_board[13][8].color = Color::Green;
                    m_board[13][8].has_moved = true;
                }
                break;
        }
    }

    // Store positions of kings.
    if (m_board[destination.x][destination.y].piece == FPC::Piece::King)
        m_king_positions[static_cast<int>(m_player)] = destination;

    return true;
}

void GameState::advance_turn() {
    if (static_cast<int>(m_player) != 3)
        m_player = static_cast<Color>(static_cast<int>(m_player) + 1);
    else
        m_player = static_cast<Color>(0);
    switch (m_player) {
        case FPC::Color::Red:
            for (int i = 3; i < 11; ++i)
                m_board[i][10].just_double_jumped = false;
            break;
        case FPC::Color::Blue:
            for (int i = 3; i < 11; ++i)
                m_board[3][i].just_double_jumped = false;
            break;
        case FPC::Color::Yellow:
            for (int i = 3; i < 11; ++i)
                m_board[i][3].just_double_jumped = false;
            break;
        case FPC::Color::Green:
            for (int i = 3; i < 11; ++i)
                m_board[10][i].just_double_jumped = false;
            break;
    }
}

Color GameState::get_current_player() const {
    return m_player;
}

std::vector<Point> GameState::generate_king_protection_moves_if_needed(const Point origin, const std::vector<Point>& valid_moves, const Color player, bool enforce_king_protection) const {
    if (!enforce_king_protection)
        return valid_moves;
    auto piece_attacking_king = square_is_under_attack_for_player(m_king_positions[static_cast<int>(player)], player);
    if (piece_attacking_king.first) {
        std::vector<Point> king_protection_moves;
        for (auto move : valid_moves) {
            GameState test_board {*this};
            test_board.move_piece_to(origin, move, false);
            if (!test_board.square_is_under_attack_for_player(test_board.m_king_positions[static_cast<int>(player)], player).first)
                king_protection_moves.push_back(move);
        }
        return king_protection_moves;
    }
    return valid_moves;
}

std::vector<Point> GameState::get_valid_moves_for_position(Point position, Color player, bool enforce_king_protection) const {
    if (!m_board[position.x][position.y].piece.has_value())
        return {};
    switch (m_board[position.x][position.y].piece.value()) {
        case Piece::Rook:
            return get_valid_moves_for_rook(position, player, enforce_king_protection);
        case Piece::Bishop:
            return get_valid_moves_for_bishop(position, player, enforce_king_protection);
        case Piece::King:
            return get_valid_moves_for_king(position, player);
        case Piece::Queen:
            return get_valid_moves_for_queen(position, player, enforce_king_protection);
        case Piece::Knight:
            return get_valid_moves_for_knight(position, player, enforce_king_protection);
        case Piece::Pawn:
            return get_valid_moves_for_pawn(position, player, enforce_king_protection);
    }
}

std::vector<Point> GameState::get_valid_moves_for_rook(const Point position, const Color player, bool enforce_king_protection) const {
    std::vector<Point> valid_moves {};

    iterate_from(valid_moves, player, position, {1, 0});
    iterate_from(valid_moves, player, position, {-1, 0});
    iterate_from(valid_moves, player, position, {0, 1});
    iterate_from(valid_moves, player, position, {0, -1});

    return generate_king_protection_moves_if_needed(position, valid_moves, player, enforce_king_protection);
}

std::vector<Point> GameState::get_valid_moves_for_bishop(Point position, Color player, bool enforce_king_protection) const {
    std::vector<Point> valid_moves {};
    iterate_from(valid_moves, player, position, {1, 1});
    iterate_from(valid_moves, player, position, {-1, -1});
    iterate_from(valid_moves, player, position, {-1, 1});
    iterate_from(valid_moves, player, position, {1, -1});

    return generate_king_protection_moves_if_needed(position, valid_moves, player, enforce_king_protection);
}

std::pair<bool, Point> GameState::square_is_under_attack_for_player(Point position, Color player) const {
    std::array<std::array<std::pair<bool, Point>, 14>, 14> attack_board;
    auto iterate_position_if_valid = [this, &attack_board, player, position](Point iterate_position) {
        if (!is_valid_position(iterate_position))
            return;
        if (!m_board[iterate_position.x][iterate_position.y].color.has_value() || !m_board[iterate_position.x][iterate_position.y].piece.has_value())
            return;
        if (m_board[iterate_position.x][iterate_position.y].color.value() == player)
            return;
        // We cannot recurse this function, so we use an alternative path to find quasi-valid moves for enemy kings, which is good enough for our purposes.
        auto moves = get_valid_moves_for_king_lite(iterate_position, m_board[iterate_position.x][iterate_position.y].color.value());
        if (m_board[iterate_position.x][iterate_position.y].piece.value() != Piece::King)
            moves = get_valid_moves_for_position(iterate_position, m_board[iterate_position.x][iterate_position.y].color.value(), false);
        for (auto move : moves) {
            if (move == position) {
                attack_board[move.x][move.y].first = true;
                attack_board[move.x][move.y].second = {iterate_position.x, iterate_position.y};
            }
        }
    };
    for (int x = 3; x < 11; ++x) {
        for (int y = 0; y < 3; ++y)
            iterate_position_if_valid({x, y});
    }

    for (int x = 3; x < 11; ++x) {
        for (int y = 11; y < 14; ++y)
            iterate_position_if_valid({x, y});
    }

    for (int x = 0; x < 14; ++x) {
        for (int y = 3; y < 11; ++y)
            iterate_position_if_valid({x, y});
    }
    return attack_board[position.x][position.y];
}

// This function does not ensure that the king is not placed in check. It is for internal use only.
std::vector<Point> GameState::get_valid_moves_for_king_lite(Point position, Color player) const {
    std::vector<Point> valid_moves {};
    for (int row = position.x - 1; row < position.x + 2; ++row) {
        for (int column = position.y - 1; column < position.y + 2; ++column) {
            Point current_position {row, column};
            if (is_valid_position(current_position) && (current_position != position) && !point_is_of_color(current_position, player))
                valid_moves.push_back(current_position);
        }
    }
    return valid_moves;
}

std::vector<Point> GameState::get_valid_moves_for_king(Point position, Color player) const {
    std::vector<Point> valid_moves {};
    for (auto move : get_valid_moves_for_king_lite(position, player)) {
        GameState test_board {*this};
        test_board.m_board[move.x][move.y].piece = Piece::King;
        test_board.m_board[move.x][move.y].color = player;
        test_board.m_board[move.x][move.y].has_moved = true;
        test_board.empty_square(position);
        if (!test_board.square_is_under_attack_for_player(move, player).first)
            valid_moves.push_back(move);
    }

    auto push_back_castling_move_if_valid = [&](Point queenside_rook, Point kingside_rook, Point axis_map) {
        bool kingside_path_blocked = false;
        bool queenside_path_blocked = false;
        if (!m_board[queenside_rook.x][queenside_rook.y].has_moved) {
            int target = position.x;
            if (axis_map.y != 0)
                target = position.y;
            for (int i = 4; i < target; ++i) {
                auto piece = m_board[i][queenside_rook.y].piece;
                if (axis_map.y != 0)
                    piece = m_board[queenside_rook.x][i].piece;
                if (piece.has_value()) {
                    queenside_path_blocked = true;
                    break;
                }
            }
        }
        if (!m_board[kingside_rook.x][kingside_rook.y].has_moved) {
            int target = position.x;
            if (axis_map.y != 0)
                target = position.y;
            for (int i = target + 1; i < 10; ++i) {
                auto piece = m_board[i][kingside_rook.y].piece;
                if (axis_map.y != 0)
                    piece = m_board[kingside_rook.x][i].piece;
                if (piece.has_value()) {
                    kingside_path_blocked = true;
                    break;
                }
            }
        }
        if (!queenside_path_blocked)
            valid_moves.push_back({position.x + axis_map.x, position.y + axis_map.y});
        if (!kingside_path_blocked)
            valid_moves.push_back({position.x - axis_map.x, position.y - axis_map.y});
    };

    // Castling
    if (!m_board[position.x][position.y].has_moved && !square_is_under_attack_for_player(position, player).first) {
        switch (player) {
            case Color::Red:
                push_back_castling_move_if_valid({3, 13}, {10, 13}, {-2, 0});
                break;
            case Color::Blue:
                push_back_castling_move_if_valid({0, 10}, {0, 3}, {0, -2});
                break;
            case Color::Yellow:
                push_back_castling_move_if_valid({3, 0}, {10, 0}, {-2, 0});
                break;
            case Color::Green:
                push_back_castling_move_if_valid({13, 10}, {13, 3}, {0, -2});
                break;
        }
    }
    return valid_moves;
}

std::vector<Point> GameState::get_valid_moves_for_queen(Point position, Color player, bool enforce_king_protection) const {
    auto valid_moves = get_valid_moves_for_bishop(position, player, enforce_king_protection);
    auto straight_moves = get_valid_moves_for_rook(position, player, enforce_king_protection);
    valid_moves.insert(valid_moves.end(), straight_moves.begin(), straight_moves.end());
    return generate_king_protection_moves_if_needed(position, valid_moves, player, enforce_king_protection);
}

std::vector<Point> GameState::get_valid_moves_for_knight(Point position, Color player, bool enforce_king_protection) const {
    std::vector<Point> valid_moves {};
    auto push_back_if_valid = [&](const Point& position) {
        if (is_valid_position(position) && !point_is_of_color(position, player))
            valid_moves.push_back(position);
    };

    push_back_if_valid({position.x - 2, position.y - 1});
    push_back_if_valid({position.x - 1, position.y - 2});
    push_back_if_valid({position.x + 1, position.y - 2});
    push_back_if_valid({position.x + 2, position.y - 1});

    push_back_if_valid({position.x - 2, position.y + 1});
    push_back_if_valid({position.x - 1, position.y + 2});
    push_back_if_valid({position.x + 1, position.y + 2});
    push_back_if_valid({position.x + 2, position.y + 1});

    return generate_king_protection_moves_if_needed(position, valid_moves, player, enforce_king_protection);
}

std::vector<Point> GameState::get_valid_moves_for_pawn(Point position, Color player, bool enforce_king_protection) const {
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

    auto push_back_if_valid = [&](Point offset) -> bool {
        if (is_valid_position({position.x + offset.x, position.y + offset.y}) && !m_board[position.x + offset.x][position.y + offset.y].piece.has_value()) {
            valid_moves.push_back({position.x + offset.x, position.y + offset.y});
            return true;
        }
        return false;
    };

    if (push_back_if_valid(direction) && may_double_jump)
        push_back_if_valid({direction.x * 2, direction.y * 2});

    auto push_back_if_valid_capture = [&](Point offset) {
        Point onward {position.x + direction.x + offset.x, position.y + direction.y + offset.y};
        if (is_valid_position(onward) && m_board[onward.x][onward.y].piece.has_value() && !point_is_of_color(onward, player))
            valid_moves.push_back(onward);
        if (m_board[position.x + offset.x][position.y + offset.y].just_double_jumped)
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

    return generate_king_protection_moves_if_needed(position, valid_moves, player, enforce_king_protection);
}

}
