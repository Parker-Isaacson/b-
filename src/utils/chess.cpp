#include "chess.h"

Game::Game() { } // Lovely

Game::Game(std::string notation) { }

std::string get_board_state() {
    return "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // todo: actually implemnet

}

Move Game::get_move() {
    return Move(Square(0, 0), Square(0, 0)); // todo: actually implemnet
}

void Game::give_move(Move move) {
    update_board(move);
}

bool Game::update_board(Move move) {
    return false; // TODO: Actually implement
}
