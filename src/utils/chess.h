#include <array>
#include <string>
#include <vector>
#include <stdlib.h>

#include <iostream>

struct Square {
    int file = -1; // a-h
    int rank = -1; // 0-8

    constexpr Square() = default;

    constexpr Square(int rank_, int file_)
        : file(file_), rank(rank_) { }

};

// Using https://en.wikipedia.org/wiki/Universal_Chess_Interface
struct Move {
    Square from{};
    Square to{};

    constexpr Move() = default;

    constexpr Move(Square from_, Square to_)
        : from(from_), to(to_) { }

    constexpr Move(int from_rank, int from_file, int to_rank, int to_file)
        : Move(Square(from_rank, from_file), Square(to_rank, to_file)) {}
};

enum class Piece {
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

// [0][0] is a8, and [7][7] is h1
using Board = std::array<std::array<Piece, 8>, 8>; // Defaults as Piece::Empty

// This will take https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
class Game {
    private:
        // -- REQUIRED
        Board board{};

        bool ToMove = true; // true for white, false for black

        bool WhiteCastle = true;
        bool WhiteLongCastle = true;
        bool BlackCastle = true;
        bool BlackLongCastle = true;

        Square en_passant{};

        int halfMove = 0;
        int fullMove = 1;

        // -- Inner functions
        std::vector<Move> moves; // Legal move list

        bool update_board(Move move); // Checks validity of move provided, and updates board as provided
        bool check_moves(); // Recalculate valid moves

    public:
        // Required notation
        Game();
        Game(std::string notation);

        std::string get_board_state(); // Creates and returns the board state in Forsyth-Edwards Notation
        void give_board_state(std::string state); // Returns a new game board from the state provided
        
        Move get_move(); // Gets the best move, this is the chess bot
        void give_move(Move move); // Updates the board with a provided move
};
