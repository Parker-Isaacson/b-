#include "chess.h"

std::vector<std::pair<int, int>> get_piece_moves(Piece piece) {
    switch (piece) {
        case Piece::White_King: case Piece::Black_King: return {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}}; // Does not include castling
        case Piece::White_Queen: case Piece::Black_Queen: return {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {2, 0}, {-2, 0}, {0, 2}, {0, -2}, {2, 2}, {-2, -2}, {2, -2}, {-2, 2}, {3, 0}, {-3, 0}, {0, 3}, {0, -3}, {3, 3}, {-3, -3}, {3, -3}, {-3, 3}, {4, 0}, {-4, 0}, {0, 4}, {0, -4}, {4, 4}, {-4, -4}, {4, -4}, {-4, 4}, {5, 0}, {-5, 0}, {0, 5}, {0, -5}, {5, 5}, {-5, -5}, {5, -5}, {-5, 5}, {6, 0}, {-6, 0}, {0, 6}, {0, -6}, {6, 6}, {-6, -6}, {6, -6}, {-6, 6}, {7, 0}, {-7, 0}, {0, 7}, {0, -7}, {7, 7}, {-7, -7}, {7, -7}, {-7, 7}};
        case Piece::White_Rook: case Piece::Black_Rook: return {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {2, 0}, {-2, 0}, {0, 2}, {0, -2}, {3, 0}, {-3, 0}, {0, 3}, {0, -3}, {4, 0}, {-4, 0}, {0, 4}, {0, -4}, {5, 0}, {-5, 0}, {0, 5}, {0, -5}, {6, 0}, {-6, 0}, {0, 6}, {0, -6}, {7, 0}, {-7, 0}, {0, 7}, {0, -7}};
        case Piece::White_Bishop: case Piece::Black_Bishop: return {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {2, 2}, {-2, -2}, {2, -2}, {-2, 2}, {3, 3}, {-3, -3}, {3, -3}, {-3, 3}, {4, 4}, {-4, -4}, {4, -4}, {-4, 4}, {5, 5}, {-5, -5}, {5, -5}, {-5, 5}, {6, 6}, {-6, -6}, {6, -6}, {-6, 6}, {7, 7}, {-7, -7}, {7, -7}, {-7, 7}};
        case Piece::White_Knight: case Piece::Black_Knight: return {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {-1, 2}, {1, -2}, {-1, -2}};
        case Piece::White_Pawn: case Piece::Black_Pawn: return {{1, 0}}; // Does not include two moves forward, will be include if white is on the 2nd rank, or if black is on the 7th.
        case Piece::Empty: default: return {};
    }
}

char piece_to_string(Piece p) {
    switch (p) {
        case Piece::White_King: return 'K';
        case Piece::White_Queen: return 'Q';
        case Piece::White_Bishop: return 'B';
        case Piece::White_Knight: return 'N';
        case Piece::White_Rook: return 'R';
        case Piece::White_Pawn: return 'P';
        case Piece::Black_King: return 'k';
        case Piece::Black_Queen: return 'q';
        case Piece::Black_Bishop: return 'b';
        case Piece::Black_Knight: return 'n';
        case Piece::Black_Rook: return 'r';
        case Piece::Black_Pawn: return 'p';
        default: return ' ';
    }
}

Piece string_to_piece(char c) {
    switch (c) {
        case 'K': return Piece::White_King;
        case 'Q': return Piece::White_Queen;
        case 'B': return Piece::White_Bishop;
        case 'N': return Piece::White_Knight;
        case 'R': return Piece::White_Rook;
        case 'P': return Piece::White_Pawn;
        case 'k': return Piece::Black_King;
        case 'q': return Piece::Black_Queen;
        case 'b': return Piece::Black_Bishop;
        case 'n': return Piece::Black_Knight;
        case 'r': return Piece::Black_Rook;
        case 'p': return Piece::Black_Pawn;
        default: return Piece::Empty;
    }
}

void Board::children() { } // TODO

Board Board::update(const Board& b, const Move& m) { } // TODO

std::string Board::get_board_state() {
    std::string notation = "";
    
    int count = 0;
    for (int i = 63; i >= 0; i--) {
        char m = piece_to_string(board[i]);

        if (m == ' ') {
            count++;
        } else {
            if (count > 0) {
                notation += std::to_string(count);
                count = 0;
            }

            notation += m;
        }

        if (i % 8 == 0) { // End of a row
            if (count > 0) {
                notation += std::to_string(count);
                count = 0;
            }

            if (i != 0)
                notation += "/";
        }
    }

    notation += (toMove == White) ? " w " : " b ";

    notation += (whiteKingSide) ? "K" : "";
    notation += (whiteQueenSide) ? "Q" : "";
    notation += (blackKingSide) ? "k" : "";
    notation += (blackQueenSide) ? "q" : "";

    notation += (!(whiteKingSide || whiteQueenSide || blackKingSide || blackQueenSide)) ? "- " : " ";

    notation += enPassant.to_string() + " ";

    notation += std::to_string(halfMove) + " " + std::to_string(fullMove);

    return notation;
}

void Board::give_board_state(std::string notation) {
    toMove = White;
    whiteKingSide = whiteQueenSide = blackKingSide = blackQueenSide = false;
    enPassant = Square();
    halfMove = 0;
    fullMove = 1;
    board = std::array<Piece, 64>();

    size_t i = 0;

    {
        int rank = 0, file = 0;
        for (; i < notation.length(); i++) {
            if (notation[i] == ' ')
                break;

            if (notation[i] == '/') {
                rank += 1;
                file = 0;
                continue;
            }

            if (std::isdigit(notation[i])) {
                file += notation[i] - '0';
                continue;
            }

            board[63 - (rank * 8 + file)] = string_to_piece(notation[i]);
            file++;
        }
    }

    if ( notation[++i] == 'b' )
        toMove = Black;

    i += 2;
    if ( notation[i] != '-' ) {
        if ( notation[i] == 'K' ) {
            whiteKingSide = true;
            ++i;
        }
        if ( notation[i] == 'Q' ) {
            whiteQueenSide = true;
            ++i;
        }
        if ( notation[i] == 'k' ) {
            blackKingSide = true;
            ++i;
        }
        if ( notation[i] == 'q' ) {
            blackQueenSide = true;
            ++i;
        }
    }

    i += 1;
    if ( notation[i] != '-' ) {
        enPassant = Square(notation[i], notation[i + 1]);
        ++i;
    }

    i += 2;
    int j = 0;
    for (; std::isdigit(notation[i + j]); j++) { } // Find the length of the half move clock
    halfMove = std::stoi(notation.substr(i, j));
    i += j + 1;

    j = 0;
    for (; std::isdigit(notation[i + j]); j++) { } // Find the length of the full move clock
    fullMove = std::stoi(notation.substr(i, j));
}

std::string Board::print_moves() { } // TODO 

std::string Board::print_board() { } // TODO

Game::Game(std::string notation) {
    give_board_state(notation);
}

double Game::evaluate(const Board& board) { } // TODO

bool Game::check_moves() { } // TODO

Side Game::side_of_piece(Piece p) {
    return static_cast<Side>((int)(p / 10));
}

double Game::alphabeta(const Board& node, int depth, double alpha, double beta, bool maxPlayer, std::vector<Move>& pv) { } // TODO

std::string Game::get_board_state() {
    return curr.get_board_state();
}

void Game::give_board_state(std::string state) {
    curr.give_board_state(state);
}

Move Game::get_move() { } // TODO

bool Game::give_move(Move move) { } // TODO

bool Game::give_move(Square from, Square to, Piece promo) {
    give_move(Move(from, to, promo));
}

std::string Game::end_game() { } // TODO

std::string Game::print_moves() {
    return curr.print_moves();
}

std::string Game::print_board() {
    return curr.print_board();
}

double Game::print_score() { } // TODO

Side Game::checkmate() { } // TODO
