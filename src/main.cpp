#include <iostream>
#include "utils/chess.h"

int main(int argc, char** argv) {
    Game myGame;

    std::cout << myGame.get_board_state() << std::endl; 

    return 0;
}
