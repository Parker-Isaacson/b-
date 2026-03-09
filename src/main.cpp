#include <iostream>
#include "utils/chess.h"

enum class Action {
    // Housekeeping
    NONE,
    EXIT, // q

    // Game
    START_GAME, // s
    END_GAME, // e

    // Move
    GIVE_MOVE, // g
    GET_MOVE, // t
    GIVE_GET_MOVE, // m

    // Board
    GIVE_BOARD, // p
    GET_BOARD, // b
};

// Tests, en passant, castling, pins, checkmate, check
bool test_game();
Action get_action();
void perform_action(const Action& action, Game& game);

int main(/*int argc, char** argv*/) {
    if(!test_game()) {
        std::cout << "Test fail, make sure you are up to date!\n";
        return 1;
    }

    Game g(DEFAULT_BOARD);

    Action action = Action::NONE;
    do {
        action = get_action();
        if (action == Action::EXIT)
            break;
        perform_action(action, g);
    } while (action != Action::EXIT);

    std::cout << std::endl;

    return 0;
}

void perform_action(const Action& action, Game& game) {
    auto give_move = [&]() -> bool {
        std::string m;
        do {
            std::cout << "Give a move in long algebraic notation: ";
            std::cin >> m;
        } while (m.length() != 5 || m.length() != 4);
        if (game.give_move(Move(m)))
            std::cout << "Move " << m << " has been made.\n";
        else {
            std::cout << "Move " << m << " has not been made. Valid moves below:\n";
            std::cout << game.print_moves();
            return false;
        }
        return true;
    };
    auto get_move = [&]() {
        Move m = game.get_move();
        game.give_move(m);
        std::cout << "Move " << m.to_string() << " has been made by the computer.\n";
    };
    switch (action) {
        case Action::NONE:
        case Action::EXIT: {
                               std::cout << "No Action performed\n";
                               break;
                           }
        case Action::END_GAME: {
                                   std::cout << "Game Ended!\n";
                                   std::cout << game.end_game() << std::endl;
                                   game = Game();
                                   break;
                               }
        case Action::START_GAME: {
                                     std::cout << "New Game started, board blank, white to move.\n";
                                     game = Game();
                                     break;
                                 }
        case Action::GIVE_MOVE:
                                 give_move();
                                 break;
        case Action::GET_MOVE:
                                 get_move();
                                 break;
        case Action::GIVE_GET_MOVE:
                                 if (!give_move()) {
                                     std::cout << "Give move failed will not get move.\n";
                                     break;
                                 }
                                 get_move();
                                 break;
        case Action::GIVE_BOARD: {
                                     std::string s;
                                     std::cout << "What is the board state: ";
                                     std::cin >> s;
                                     game = Game(s);
                                     std::cout << "Game has been updated to the provided board state.\n";
                                     break;
                                 }
        case Action::GET_BOARD: {
                                    std::cout << "The current board state is: " << game.get_board_state() << "\n";
                                    break;
                                }
    }
}

Action get_action() {
    std::string s = "";
    do {
        std::cout << "What action to do? (h: help): ";
        std::cin >> s;
        if (s.length() != 1) {
            std::cout << "Bad action.";
            continue;
        }
        if (s == "h") {
            std::cout << "See README.md for more information.\n";
        }
        switch (s[0]) {
            case 'q':
                return Action::EXIT;
            case 's':
                return Action::START_GAME;
            case 'e':
                return Action::END_GAME;
            case 'g':
                return Action::GIVE_MOVE;
            case 't':
                return Action::GET_MOVE;
            case 'm':
                return Action::GIVE_GET_MOVE;
            case 'p':
                return Action::GIVE_BOARD;
            case 'b':
                return Action::GET_BOARD;
            default:
                return Action::NONE;
        }
    } while (s == "" || s  == "h");

    return Action::NONE;
}

bool test_game() {
    bool result = true;
    auto move = [&](Game& game, Square a, Square b) {
        bool ret = game.give_move(a, b);
        if (!ret) {
            std::cout << "Invalid Move: " << a.to_string() << " " << b.to_string() << "!\n" << game.print_moves() << std::endl;
            result = false;
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

    return result;
}
