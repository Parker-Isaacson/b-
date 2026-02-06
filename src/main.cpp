#include <iostream>
#include "utils/chess.h"

int main(int argc, char** argv) {
    Game myGame;

    myGame.give_board_state("r1bq1bnr/ppp2p1p/2n5/3pp1p1/kP1P1B2/2P1P3/1PQ2PPP/RN2KBNR b KQ c5 10 11"); // Random Board State

    std::cout << myGame.get_board_state() << std::endl; 

    return 0;
}
