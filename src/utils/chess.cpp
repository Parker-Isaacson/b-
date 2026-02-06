#include "chess.h"

std::vector<std::pair<int, int>> get_piece_moves(Piece piece) {
    switch (piece) {
        case Piece::White_King:
        case Piece::Black_King:
            return {{-1, 1}, {0, 1}, {1, 1}, {-1, 0}, {1, 0}, {-1, -1}, {0, -1}, {1, -1}};

        case Piece::Empty:
        default:
            return {};
    }
}

Game::Game() { } // Lovely

Game::Game(std::string notation) { }

std::string get_board_state() {
    return "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // TODO: actually implemnet
}

void Game::give_board_state(std::string state) {
    board = Board{}; // TODO: actually implement
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
