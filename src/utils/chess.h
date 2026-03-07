#include <array>
#include <string>
#include <vector>
#include <stdlib.h>
#include <functional>
#include <optional>

#include <iostream> //TODO: Remove

#define DEFAULT_BOARD "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#define MIN_SCORE -1000000
#define MAX_SCORE  1000000
#define SEARCH_DEPTH 4

struct Square {
    int file = -1; // 0-7 for a-h
    int rank = -1; // 0-7 for 1-8

    constexpr Square() = default;

    constexpr Square(int rank_, int file_)
        : file(file_), rank(rank_) {}

    explicit Square(const std::string& s) {
        if (s.size() != 2) return;

        char f = static_cast<char>(std::tolower(s[0]));
        char r = s[1];

        if (f < 'a' || f > 'h' || r < '1' || r > '8') return;

        file = f - 'a';
        rank = '8' - r;
    }

    std::string to_string() const {
        if (file < 0 || file > 7 || rank < 0 || rank > 7)
            return "-";

        char f = static_cast<char>('a' + file);
        char r = static_cast<char>('8' - rank);
        return std::string(1, f) + r;
    }

    static Square from_string(const std::string& s) {
        return Square(s);
    }

    bool operator==(const Square& s) const {
        return file == s.file && rank == s.rank;
    }

    bool is_valid() const {
        return file >= 0 && file < 8 && rank >= 0 && rank < 8;
    }
};

// Squares
inline constexpr Square A1{7, 0};
inline constexpr Square B1{7, 1};
inline constexpr Square C1{7, 2};
inline constexpr Square D1{7, 3};
inline constexpr Square E1{7, 4};
inline constexpr Square F1{7, 5};
inline constexpr Square G1{7, 6};
inline constexpr Square H1{7, 7};

inline constexpr Square A2{6, 0};
inline constexpr Square B2{6, 1};
inline constexpr Square C2{6, 2};
inline constexpr Square D2{6, 3};
inline constexpr Square E2{6, 4};
inline constexpr Square F2{6, 5};
inline constexpr Square G2{6, 6};
inline constexpr Square H2{6, 7};

inline constexpr Square A3{5, 0};
inline constexpr Square B3{5, 1};
inline constexpr Square C3{5, 2};
inline constexpr Square D3{5, 3};
inline constexpr Square E3{5, 4};
inline constexpr Square F3{5, 5};
inline constexpr Square G3{5, 6};
inline constexpr Square H3{5, 7};

inline constexpr Square A4{4, 0};
inline constexpr Square B4{4, 1};
inline constexpr Square C4{4, 2};
inline constexpr Square D4{4, 3};
inline constexpr Square E4{4, 4};
inline constexpr Square F4{4, 5};
inline constexpr Square G4{4, 6};
inline constexpr Square H4{4, 7};

inline constexpr Square A5{3, 0};
inline constexpr Square B5{3, 1};
inline constexpr Square C5{3, 2};
inline constexpr Square D5{3, 3};
inline constexpr Square E5{3, 4};
inline constexpr Square F5{3, 5};
inline constexpr Square G5{3, 6};
inline constexpr Square H5{3, 7};

inline constexpr Square A6{2, 0};
inline constexpr Square B6{2, 1};
inline constexpr Square C6{2, 2};
inline constexpr Square D6{2, 3};
inline constexpr Square E6{2, 4};
inline constexpr Square F6{2, 5};
inline constexpr Square G6{2, 6};
inline constexpr Square H6{2, 7};

inline constexpr Square A7{1, 0};
inline constexpr Square B7{1, 1};
inline constexpr Square C7{1, 2};
inline constexpr Square D7{1, 3};
inline constexpr Square E7{1, 4};
inline constexpr Square F7{1, 5};
inline constexpr Square G7{1, 6};
inline constexpr Square H7{1, 7};

inline constexpr Square A8{0, 0};
inline constexpr Square B8{0, 1};
inline constexpr Square C8{0, 2};
inline constexpr Square D8{0, 3};
inline constexpr Square E8{0, 4};
inline constexpr Square F8{0, 5};
inline constexpr Square G8{0, 6};
inline constexpr Square H8{0, 7};

enum class Side { Empty, White, Black, Draw };

enum class Piece : int8_t {
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

char piece_to_string(Piece p);
Piece string_to_piece(char c);

// Using https://en.wikipedia.org/wiki/Universal_Chess_Interface
struct Move {
    Square from{};
    Square to{};
    Piece promotion = Piece::Empty;

    constexpr Move() = default;

    constexpr Move(Square from_, Square to_, Piece promotion_ = Piece::Empty)
        : from(from_), to(to_), promotion(promotion_) { }

    constexpr Move(int from_rank, int from_file, int to_rank, int to_file)
        : Move(Square(from_rank, from_file), Square(to_rank, to_file), Piece::Empty) {}

    constexpr Move(int from_rank, int from_file, int to_rank, int to_file, Piece promotion_)
        : Move(Square(from_rank, from_file), Square(to_rank, to_file), promotion_) {}

    std::string to_string() const {
        std::string ret = from.to_string() + " " + to.to_string(); 

        if (promotion != Piece::Empty)
            ret += " = " + std::string(1, piece_to_string(promotion));

        return ret;
    }

    bool operator==(const Move& m) const {
        return this->from == m.from && this->to == m.to && this->promotion == m.promotion;
    }
};

// [0][0] is a8, and [7][7] is h1, [rank][8 - file] (up-down)(left-right)
using Board = std::array<std::array<Piece, 8>, 8>; // Defaults as Piece::Empty

struct CastlingRights {
    CastlingRights() = default;
    CastlingRights(bool wk, bool wq, bool bk, bool bq)
        : whiteKingSide(wk), whiteQueenSide(wq), blackKingSide(bk), blackQueenSide(bq) {}
    bool whiteKingSide  = false;
    bool whiteQueenSide = false;
    bool blackKingSide  = false;
    bool blackQueenSide = false;
};

struct PositionState {
    PositionState() = default;
    PositionState(Side tm, Square ep, CastlingRights cr)
        : toMove(tm), enPassant(ep), castle(cr) {}
    PositionState(Side tm, Square ep, CastlingRights cr, int hm, int fm)
        : toMove(tm), enPassant(ep), castle(cr), halfMove(hm), fullMove(fm) {}
    Side toMove = Side::Empty;
    Square enPassant;
    CastlingRights castle;
    int halfMove = 0;
    int fullMove = 1;
};

// This will take https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
class Game {
    private:
        // Board and related state
        Board board{};
        PositionState state;

        // Listing of best moves for optimal position

        // Inner functions
        std::vector<Move> moves; // Legal move list
        static std::optional<std::pair<Board, PositionState>> update_board(const Board& board, const PositionState& state, const Move& move, const std::vector<Move>& moves);
        bool check_moves(); // Clear and recalculate valid moves
        static std::vector<Move> children(const Board& board, const PositionState& st); // Find all children moves of current position
        static Side side_of_piece(Piece p); // Checks if the current peice is part of the current player.

    public:
        std::vector<Move> bestMoves;
        // Required notation
        Game();
        Game(std::string notation);

        std::string get_board_state(); // Creates and returns the board state in Forsyth-Edwards Notation
        void give_board_state(std::string state); // Returns a new game board from the state provided
        
        Move get_move(); // Gets the best move, this is the chess bot
        bool give_move(Move move); // Updates the board with a provided 
        bool give_move(Square from, Square to, Piece promo = Piece::Empty);
        
        // Debug functions
        std::string print_moves();
        Side checkmate(); // Returns the winning side if possible
};
