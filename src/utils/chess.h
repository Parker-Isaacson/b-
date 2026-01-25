#include <cstdint>
#include <cassert>
#include <array>
#include <string>

struct Square {
    uint8_t file = 0;
    uint8_t rank = 0;

    constexpr Square() = default;

    constexpr Square(uint8_t rank_, uint8_t file_)
        : file(file_), rank(rank_) { }

};

// Using https://en.wikipedia.org/wiki/Universal_Chess_Interface
struct Move {
    Square from{};
    Square to{};

    constexpr Move() = default;

    constexpr Move(Square from_, Square to_)
        : from(from_), to(to_) { }

    constexpr Move(uint8_t from_rank, uint8_t from_file, uint8_t to_rank, uint8_t to_file)
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

using Board = std::array<std::array<Piece, 8>, 8>; // Defaults as Piece::Empty

// This will take https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
class Game {
    private:
        Board board{};

        bool WhiteToMove = true;
        bool BlackToMove = false;

        bool WhiteCastle = false;
        bool WhiteLongCastle = false;
        bool BlackCastle = false;
        bool BlackLongCastle = false;

        Square en_passant{};

        // Non-required, but very useful
        bool update_board(Move move); // Checks validity of move provided, and updates board as provided

    public:
        // Required notation
        Game();
        Game(std::string notation);

        std::string get_board_state(); // Creates and returns the board state in Forsyth-Edwards Notation
        Game give_board_state(std::string state); // Returns a new game board from the state provided
        
        Move get_move(); // Gets the best move, this is the chess bot
        void give_move(Move move); // Updates the board with a provided move
};
