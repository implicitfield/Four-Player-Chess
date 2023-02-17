#pragma once

#include <array>
#include <optional>
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

// Alpahbetically sorted.
enum class Piece {
    Bishop,
    King,
    Knight,
    Pawn,
    Queen,
    Rook
};

enum class IterationDecision {
    Continue,
    Break,
};

struct Square {
    std::optional<Piece> piece = std::nullopt;
    std::optional<Color> color = std::nullopt;
};

struct Point {
    int x = 0;
    int y = 0;
};

class GameState {
public:
    GameState();
    void reset();
    const std::array<std::array<Square, 14>, 14>& get_board() const;
    std::array<std::array<Square, 14>, 14>& get_board();
    bool point_is_of_color (Point point, Color color) const;
    IterationDecision position_decision(std::vector<Point>& valid_moves, const Color player, const Point original_position, const Point test_position);
    std::vector<Point> get_valid_moves_for_position(Point position, Color player);
    std::vector<Point> get_valid_moves_for_rook(Point position, Color player);
    std::vector<Point> get_valid_moves_for_bishop(const Point position, Color player);
    std::vector<Point> get_valid_moves_for_king(Point position, Color player);
    std::vector<Point> get_valid_moves_for_queen(Point position, Color player);
    std::vector<Point> get_valid_moves_for_knight(Point position, Color player);
    std::vector<Point> get_valid_moves_for_pawn(Point position, Color player);
private:
    std::array<std::array<Square, 14>, 14> m_board;
    Color m_player {Color::Red};
};

void get_piece_name(const GameState& game, int x, int y);
bool is_valid_position(int row, int column);

}
