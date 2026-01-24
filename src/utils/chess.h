#include <cstdint>
#include <cassert>
#include <array>

// enum class Rank : std::uint8_t { }; // This is not real, they are already numbers 1-8

enum class File : std::uint8_t { a = 1, b, c, d, e, f, g, h, };

struct Square {
    File file = File::a;
    uint8_t rank = 1;

    constexpr Square() = default;

    constexpr Square(std::uint8_t rank_, File file_)
        : file(file_), rank(rank_) { }

};

struct Move {
    Square from{};
    Square to{};

    constexpr Move() = default;

    constexpr Move(Square from_, Square to_)
        : from(from_), to(to_) { }

    constexpr Move(std::uint8_t from_rank, File from_file, std::uint8_t to_rank, File to_file)
        : Move(Square(from_rank, from_file), Square(to_rank, to_file)) {}
};

enum class Piece : std::uint8_t {
    Empty,

    White_King,
    White_Queen,
    White_Bishop,
    White_Knight,
    White_Rook,
    White_Pawn,

    Black_King,
    Black_Queen,
    Black_Bishop,
    Black_Knight,
    Black_Rook,
    Black_Pawn,
};

using Board = std::array<std::array<Piece, 8>, 8>;

class Game {
    private:
        Board board{};

        bool WhiteToMove = true;
        bool BlackToMove = false;

        bool WhiteCastle = false; // O-O
        bool WhiteLongCastle = false; // O-O-O
        bool BlackCastle = false;
        bool BlackLongCastle = false;

    public:
        Game() {}
};
