#include <iostream>
#include "utils/chess.h"

// Tests, en passant, castling, pins, checkmate, check
// Valid only if nothing prints
void test_game() {
    auto move = [&](Game& game, Square a, Square b) {
        bool ret = game.give_move(a, b);
        if (!ret) {
            std::cout << "Invalid Move: " << a.to_string() << " " << b.to_string() << "!\n" << game.print_moves() << std::endl;
        }
    };

    // en passant
    Game myGame(DEFAULT_BOARD);
    move(myGame, E2, E4);
    move(myGame, A7, A6);
    move(myGame, E4, E5);
    move(myGame, D7, D5);
    move(myGame, E5, D6);

    // Castling
    myGame.give_board_state(DEFAULT_BOARD);
    move(myGame, E2, E4);
    move(myGame, E7, E5);
    move(myGame, F1, C4);
    move(myGame, G8, F6);
    move(myGame, G1, F3);
    move(myGame, F6, E4);
    move(myGame, E1, G1);

    // Pin
    myGame.give_board_state(DEFAULT_BOARD);
    move(myGame, E2, E4);
    move(myGame, E7, E5);
    move(myGame, D1, H5);
    // std::cout << myGame.print_moves() << std::endl; // Note there are no moves from f7
    
    // Checkmate
    myGame.give_board_state(DEFAULT_BOARD);
    move(myGame, E2, E4);
    move(myGame, F7, F5);
    move(myGame, E4, F5);
    move(myGame, G7, G5); // en passant is valid here
    move(myGame, D1, H5);
    // std::cout << myGame.print_moves() << std::endl; // Note there are no moves
    
    // Check
    myGame.give_board_state(DEFAULT_BOARD);
    move(myGame, E2, E4);
    move(myGame, F7, F5);
    move(myGame, D1, H5);
    // std::cout << myGame.print_moves() << std::endl; // Note there is just one move
}

int main(/*int argc, char** argv*/) {
    test_game();

    Game g;

    // std::cout << g.print_moves() << std::endl;

    while(g.checkmate() == Side::Empty) {
        Move m = g.get_move();
        for (Move mx : g.bestMoves)
            std::cout << mx.to_string() << std::endl;
        if (!g.give_move(m)) {
            std::cout << "Invalid Move: " << m.to_string() << "!\n" << g.print_moves() << std::endl;
            return 1;
        }
        std::cout << g.get_board_state() << std::endl;
    }

    return 0;
}
