#include "chess.h"
#include <cctype>
#include <string>

std::vector<std::pair<int, int>> get_piece_moves(Piece piece) {
    switch (piece) {
        case Piece::White_King:
        case Piece::Black_King:
            return {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

        case Piece::White_Queen:
        case Piece::Black_Queen:
            return {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {2, 0}, {-2, 0}, {0, 2}, {0, -2}, {2, 2}, {-2, -2}, {2, -2}, {-2, 2}, {3, 0}, {-3, 0}, {0, 3}, {0, -3}, {3, 3}, {-3, -3}, {3, -3}, {-3, 3}, {4, 0}, {-4, 0}, {0, 4}, {0, -4}, {4, 4}, {-4, -4}, {4, -4}, {-4, 4}, {5, 0}, {-5, 0}, {0, 5}, {0, -5}, {5, 5}, {-5, -5}, {5, -5}, {-5, 5}, {6, 0}, {-6, 0}, {0, 6}, {0, -6}, {6, 6}, {-6, -6}, {6, -6}, {-6, 6}, {7, 0}, {-7, 0}, {0, 7}, {0, -7}, {7, 7}, {-7, -7}, {7, -7}, {-7, 7}};

        case Piece::White_Rook:
        case Piece::Black_Rook:
            return {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {2, 0}, {-2, 0}, {0, 2}, {0, -2}, {3, 0}, {-3, 0}, {0, 3}, {0, -3}, {4, 0}, {-4, 0}, {0, 4}, {0, -4}, {5, 0}, {-5, 0}, {0, 5}, {0, -5}, {6, 0}, {-6, 0}, {0, 6}, {0, -6}, {7, 0}, {-7, 0}, {0, 7}, {0, -7}};

        case Piece::White_Bishop:
        case Piece::Black_Bishop:
            return {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {2, 2}, {-2, -2}, {2, -2}, {-2, 2}, {3, 3}, {-3, -3}, {3, -3}, {-3, 3}, {4, 4}, {-4, -4}, {4, -4}, {-4, 4}, {5, 5}, {-5, -5}, {5, -5}, {-5, 5}, {6, 6}, {-6, -6}, {6, -6}, {-6, 6}, {7, 7}, {-7, -7}, {7, -7}, {-7, 7}};

        case Piece::White_Knight:
        case Piece::Black_Knight:
            return {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {-1, 2}, {1, -2}, {-1, -2}};

        case Piece::White_Pawn:
        case Piece::Black_Pawn:
            return {{1, 0}, {2, 0}};

        case Piece::Empty:
        default:
            return {};
    }
}

char piece_to_string(Piece p) {
    switch (p) {
        case Piece::White_King:
            return 'K';

        case Piece::White_Queen:
            return 'Q';

        case Piece::White_Bishop:
            return 'B';

        case Piece::White_Knight:
            return 'N';

        case Piece::White_Rook:
            return 'R';

        case Piece::White_Pawn:
            return 'P';

        case Piece::Black_King:
            return 'k';

        case Piece::Black_Queen:
            return 'q';

        case Piece::Black_Bishop:
            return 'b';

        case Piece::Black_Knight:
            return 'n';

        case Piece::Black_Rook:
            return 'r';

        case Piece::Black_Pawn:
            return 'p';

        default:
            return 'z';
    }
}

Piece string_to_piece(char c) {
    switch (c) {
        case 'K':
            return Piece::White_King;

        case 'Q':
            return Piece::White_Queen;

        case 'B':
            return Piece::White_Bishop;

        case 'N':
            return Piece::White_Knight;

        case 'R':
            return Piece::White_Rook;

        case 'P':
            return Piece::White_Pawn;

        case 'k':
            return Piece::Black_King;

        case 'q':
            return Piece::Black_Queen;

        case 'b':
            return Piece::Black_Bishop;

        case 'n':
            return Piece::Black_Knight;

        case 'r':
            return Piece::Black_Rook;

        case 'p':
            return Piece::Black_Pawn;

        default:
            return Piece::Empty;
    }
}

std::string square_to_string(Square s) {
    if ( s.file == -1 || s.rank == -1 )
        return "-";
    
    // Since the file is 0-7 representing a-h, we can add 97 from the ASCII table to turn 0 -> a and 7 -> h
    return static_cast<char>(s.file + 97) + std::to_string(s.rank);
}

Game::Game() {
    board = {
        std::array<Piece, 8>{Piece::Black_Rook, Piece::Black_Knight, Piece::Black_Bishop, Piece::Black_Queen, Piece::Black_King, Piece::Black_Bishop, Piece::Black_Knight, Piece::Black_Rook},
        std::array<Piece, 8>{Piece::Black_Pawn, Piece::Black_Pawn, Piece::Black_Pawn, Piece::Black_Pawn, Piece::Black_Pawn, Piece::Black_Pawn, Piece::Black_Pawn, Piece::Black_Pawn},
        std::array<Piece, 8>{Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty},
        std::array<Piece, 8>{Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty},
        std::array<Piece, 8>{Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty},
        std::array<Piece, 8>{Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty},
        std::array<Piece, 8>{Piece::White_Pawn, Piece::White_Pawn, Piece::White_Pawn, Piece::White_Pawn, Piece::White_Pawn, Piece::White_Pawn, Piece::White_Pawn, Piece::White_Pawn},
        std::array<Piece, 8>{Piece::White_Rook, Piece::White_Knight, Piece::White_Bishop, Piece::White_Queen, Piece::White_King, Piece::White_Bishop, Piece::White_Knight, Piece::White_Rook},
    };
}

Game::Game(std::string notation) { give_board_state(notation); }

std::string Game::get_board_state() {
    std::string state = "";

    int rankCount = 0;
    for (std::array<Piece, 8> rank : board) {
        int count = 0;
        for (Piece p : rank) {
            char m = piece_to_string(p);
            if ( m == 'z' ) {
                count++;
                continue;
            }

            if ( count > 0 ) {
                state += std::to_string(count);
                count = 0;
            }
            
            state += m;
        }
        
        if ( count > 0 )
            state += std::to_string(count);
        
        rankCount++;

        if ( rankCount != 8 )
            state += "/";
    }

    state += ( ToMove ) ? " w " : " b ";

    state += ( WhiteCastle ) ? "K" : "";
    state += ( WhiteLongCastle ) ? "Q" : "";
    state += ( BlackCastle ) ? "k" : "";
    state += ( BlackLongCastle ) ? "q" : "";

    state += ( ! (WhiteCastle && WhiteLongCastle && BlackCastle && BlackLongCastle ) ) ? "- " : " ";

    state += square_to_string(en_passant) + " ";

    state += std::to_string(halfMove) + " " + std::to_string(fullMove);

    return state;
}

void Game::give_board_state(std::string state) {
    ToMove = true;

    WhiteCastle = true;
    WhiteLongCastle = true;
    BlackCastle = true;
    BlackLongCastle = true;

    en_passant = Square();

    int halfMove = 0;
    int fullMove = 1;

    board = Board{};
    
    int rank = 0, file = 0;

    for (int i = 0; i < state.length(); i++) {
        if ( state[i] == ' ' )
            break;

        if ( state[i] ==  '/' ) {
            rank += 1;
            file = 0;
            std::cout << get_board_state() << std::endl;
            continue;
        }

        if (std::isdigit(state[i])) {
            int n = state[i] - '0'; // char "cast" to int
            for (int j = 0; j < n; j++) {
                board[rank][file] = Piece::Empty;
                file++;
            }
            continue;
        }

        board[rank][file] = string_to_piece(state[i]);
        file++;
    }
    // TODO: actually implement
}

Move Game::get_move() {
    return Move(Square(0, 0), Square(0, 0)); // TODO: actually implemnet
}

void Game::give_move(Move move) {
    update_board(move);
}

bool Game::update_board(Move move) {
    moves.clear();
    return false; // TODO: Actually implement
}

bool Game::check_moves() {
    return false; // TODO: Actually implement
}
