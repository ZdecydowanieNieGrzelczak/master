
#ifndef MASTER_TICTACTOE_H
#define MASTER_TICTACTOE_H

#include <utility>
#include "../Game.h"

std::random_device dev;



class TicTacToe : Game {
public:
    TicTacToe();

    GameEval moveWhite(short actionCode);
    GameEval moveBlack(short actionCode);

    GameEval reset() override;
    GameEval doAction(int i) override;

private:
    short counter{0};
    bool isWhiteMoving{true};
    short whiteState{0};
    short blackState{0};
    constexpr const static short winCombinations[8] = {
            // each row
            0b000000111,
            0b000111000,
            0b111000000,
            // each column
            0b100100100,
            0b010010010,
            0b001001001,
            // each diagonal
            0b100010001,
            0b001010100
    };
    constexpr const static short actions[9] = {
            0b000000001,
            0b000000010,
            0b000000100,
            0b000001000,
            0b000010000,
            0b000100000,
            0b001000000,
            0b010000000,
            0b100000000,
    };
};




#endif //MASTER_TICTACTOE_H
