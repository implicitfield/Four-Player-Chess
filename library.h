#pragma once

#include <array>
#include <functional>
#include <optional>
#include <utility>
#include <vector>

namespace FPC {

enum class Axis {
    X,
    Y
};

// In order of turns.
enum class Color {
    Red,
    Blue,
    Yellow,
    Green
};

// First four in order of promotion.
enum class Piece {
    Queen,
    Rook,
    Bishop,
    Knight,
    King,
    Pawn
};

enum class IterationDecision {
    Continue,
    Break,
};

struct Square {
    std::optional<Piece> piece = std::nullopt;
    std::optional<Color> color = std::nullopt;
    bool just_double_jumped = false; // Used to validate whether performing an "en passant" move is legal.
    bool has_moved = false;          // Used to validate whether castling is legal.
};

struct Point {
    int x = 0;
    int y = 0;

    bool operator==(const Point& rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    bool operator!=(const Point& rhs) const {
        return !(*this == rhs);
    }
};

class GameState {
public:
    GameState();
    void reset();
    const std::array<std::array<Square, 14>, 14>& get_board() const;
    std::array<std::array<Square, 14>, 14>& get_board();
    bool point_is_of_color(Point point, Color color) const;
    bool move_piece_to(const Point& origin, const Point& destination, bool enforce_king_protection);
    bool may_promote(const Point& position, const Color& player) const;
    void advance_turn();
    Color get_current_player() const;
    const std::vector<Color>& get_current_players() const;
    bool player_exists(Color player) const;
    std::vector<Point> get_valid_moves_for_position(Point position, Color player, bool enforce_king_protection) const;
    std::vector<Point> get_valid_moves_for_rook(Point position, Color player, bool enforce_king_protection) const;
    std::vector<Point> get_valid_moves_for_bishop(const Point position, Color player, bool enforce_king_protection) const;
    std::vector<Point> get_valid_moves_for_king(Point position, Color player) const;
    std::vector<Point> get_valid_moves_for_queen(Point position, Color player, bool enforce_king_protection) const;
    std::vector<Point> get_valid_moves_for_knight(Point position, Color player, bool enforce_king_protection) const;
    std::vector<Point> get_valid_moves_for_pawn(Point position, Color player, bool enforce_king_protection) const;

private:
    std::pair<bool, Point> square_is_under_attack_for_player(Point position, Color player) const;
    std::vector<Point> get_valid_moves_for_king_lite(Point position, Color player) const;
    std::vector<Point> filter_moves(const Point origin, std::vector<Point>& valid_moves, const Color player, bool enforce_king_protection) const;
    void unsafe_move_piece_to(const Point& origin, const Point& destination);
    bool empty_square(const Point& square);
    void iterate_from(std::vector<Point>& valid_moves, const Color player, const Point original_position, const Point increment_map) const;
    std::array<std::array<Square, 14>, 14> m_board;
    Color m_player {Color::Red};
    // Must be accessed in the same order as the 'Color' enum.
    std::array<Point, 4> m_king_positions {Point {7, 13}, Point {0, 6}, Point {6, 0}, Point {13, 7}};
    std::vector<Color> m_current_players {
        Color::Red,
        Color::Blue,
        Color::Yellow,
        Color::Green,
    };
};

void get_piece_name(const GameState& game, int x, int y);
bool is_valid_position(const FPC::Point& position);

}
