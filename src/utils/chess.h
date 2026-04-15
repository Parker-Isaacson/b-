#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include <string>
#include <array>
#include <stack>
#include <vector>

#ifndef CHESS_SEARCH_THREADS
#define CHESS_SEARCH_THREADS 24
#endif

#define DEFAULT_BOARD "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#define MIN_SCORE -1000000
#define MAX_SCORE  1000000
#define SEARCH_DEPTH 4

#define RESET  "\033[0m"
#define WHITE_BG "\033[107m"
#define BLACK_BG "\033[40m"

#define WHITE_TEXT "\033[97m"
#define BLACK_TEXT "\033[30m"

typedef enum : int8_t{
    Empty = 0,

    White_King   = 11,
    White_Queen  = 12,
    White_Bishop = 13,
    White_Knight = 14,
    White_Rook   = 15,
    White_Pawn   = 16,

    Black_King   = 21,
    Black_Queen  = 22,
    Black_Bishop = 23,
    Black_Knight = 24, 
    Black_Rook   = 25,
    Black_Pawn   = 26,
} Piece;

typedef enum : int8_t {
    None = 0,
    White = 1,
    Black = 2,
} Side;

char piece_to_string(Piece p);
Piece string_to_piece(char c);

struct Square {
    int file = -1; // 0-7 to a-h
    int rank = -1; // 0-7 to 1-8
    int real = -1; // 0-63 for a1-h8

    constexpr Square() = default;
    constexpr Square(int rank_, int file_)
        : file(file_), rank(rank_) {
            real = rank_ * 8 + file_;
        }
    constexpr Square(int real_)
        : real(real_) {
            rank = real_ % 8;
            file = (real_ - rank) / 8;
        }
    explicit Square(const std::string& s) {
        if (s.size() != 2)
            return;

        char f = static_cast<char>(std::tolower(s[0]));
        char r = s[1];

        if (f < 'a' || f > 'h' || r < '1' || r > '8') return;

        file = f - 'a';
        rank = '8' - r;

        real = rank * 8 + file;
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
        return real == s.real;
    }
    bool is_valid() const {
        return real >= 0 && real < 64 && file >= 0 && file < 8 && rank >= 0 && rank < 8;
    }
};

struct Move {
    Square from{};
    Square to{};
    Piece promotion = Piece::Empty;

    constexpr Move() = default;
    constexpr Move(Square from_, Square to_, Piece promotion_ = Piece::Empty)
        : from(from_), to(to_), promotion(promotion_) {}
    explicit Move(const std::string& m) {
        if (m.length() != 4 && m.length() != 5)
            return;
        from = Square(m.substr(0, 2));
        to = Square(m.substr(2, 2));
        if (m.length() < 5)
            return;
        promotion = string_to_piece(m[4]);
    }

    std::string to_string() const {
        std::string ret = from.to_string() + " " + to.to_string(); 

        if (promotion != Piece::Empty)
            ret += " = " + std::string(1, piece_to_string(promotion));

        return ret;
    }
    static Move from_string(const std::string& s) {
        return Move(s);
    }
    bool operator==(const Move& m) const {
        return from == m.from && to == m.to;
    }
    bool is_valid() const {
        return from.is_valid() && to.is_valid();
    }
};

struct Board {
    std::array<Piece, 64> board{
        White_Rook, White_Knight, White_Bishop, White_Queen, White_King, White_Bishop, White_Knight, White_Rook,
        White_Pawn, White_Pawn, White_Pawn, White_Pawn, White_Pawn, White_Pawn, White_Pawn, White_Pawn, 
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 
        Black_Pawn, Black_Pawn, Black_Pawn, Black_Pawn, Black_Pawn, Black_Pawn, Black_Pawn, Black_Pawn, 
        Black_Rook, Black_Knight, Black_Bishop, Black_Queen, Black_King, Black_Bishop, Black_Knight, Black_Rook,
    };

    bool whiteKingSide  = true;
    bool whiteQueenSide = true;
    bool blackKingSide  = true;
    bool blackQueenSide = true;

    Side toMove = White;
    Square enPassant{};

    int halfMove = 0;
    int fullMove = 1;

    std::vector<Move> moves{};

    constexpr Board() = default; // TODO: Update? Might not work good because this is the copy

    void children();
    static Board update(const Board& b, const Move& m);
    std::string get_board_state();
    void give_board_state(std::string notation);
    std::string print_moves();
    std::string print_board();
};

class Game {
    public:
        Board curr{};

        std::vector<Move> bestMoves{};
        std::vector<Move> completed{};

        static double evaluate(const Board& board);
        bool check_moves(); // Clear and recalculate valid moves
        static Side side_of_piece(Piece p); // Checks if the current piece is part of the current player.

        static double alphabeta(const Board& node, int depth, double alpha, double beta, bool maxPlayer, std::vector<Move>& pv);
    public:
        constexpr Game() = default; // TODO
        Game(std::string notation);

        std::string get_board_state(); // Creates and returns the board state in Forsyth-Edwards Notation
        void give_board_state(std::string state); // Returns a new game board from the state provided

        Move get_move(); // Gets the best move, this is the chess bot
        bool give_move(Move move); // Updates the board with a provided 
        bool give_move(Square from, Square to, Piece promo = Piece::Empty);

        std::string end_game(); // Print all moves made, and current board state
        std::string print_moves(); // Print the listing of moves and current board state
        std::string print_board(); // Print the board as nicely as possible!
        double print_score(); // Prints the score of the current board
        Side checkmate(); // Returns the winning side if possible
};

// Inline squares
inline constexpr Square A1{0};
inline constexpr Square A2{1};
inline constexpr Square A3{2};
inline constexpr Square A4{3};
inline constexpr Square A5{4};
inline constexpr Square A6{5};
inline constexpr Square A7{6};
inline constexpr Square A8{7};

inline constexpr Square B1{8};
inline constexpr Square B2{9};
inline constexpr Square B3{10};
inline constexpr Square B4{11};
inline constexpr Square B5{12};
inline constexpr Square B6{13};
inline constexpr Square B7{14};
inline constexpr Square B8{15};

inline constexpr Square C1{16};
inline constexpr Square C2{17};
inline constexpr Square C3{18};
inline constexpr Square C4{19};
inline constexpr Square C5{20};
inline constexpr Square C6{21};
inline constexpr Square C7{22};
inline constexpr Square C8{23};

inline constexpr Square D1{24};
inline constexpr Square D2{25};
inline constexpr Square D3{26};
inline constexpr Square D4{27};
inline constexpr Square D5{28};
inline constexpr Square D6{29};
inline constexpr Square D7{30};
inline constexpr Square D8{31};

inline constexpr Square E1{32};
inline constexpr Square E2{33};
inline constexpr Square E3{34};
inline constexpr Square E4{35};
inline constexpr Square E5{36};
inline constexpr Square E6{37};
inline constexpr Square E7{38};
inline constexpr Square E8{39};

inline constexpr Square F1{40};
inline constexpr Square F2{41};
inline constexpr Square F3{42};
inline constexpr Square F4{43};
inline constexpr Square F5{44};
inline constexpr Square F6{45};
inline constexpr Square F7{46};
inline constexpr Square F8{47};

inline constexpr Square G1{48};
inline constexpr Square G2{49};
inline constexpr Square G3{50};
inline constexpr Square G4{51};
inline constexpr Square G5{52};
inline constexpr Square G6{53};
inline constexpr Square G7{54};
inline constexpr Square G8{55};

inline constexpr Square H1{56};
inline constexpr Square H2{57};
inline constexpr Square H3{58};
inline constexpr Square H4{59};
inline constexpr Square H5{60};
inline constexpr Square H6{61};
inline constexpr Square H7{62};
inline constexpr Square H8{63};

#endif // CHESS_H
