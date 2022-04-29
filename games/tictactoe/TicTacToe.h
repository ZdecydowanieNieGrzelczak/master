
#ifndef MASTER_TICTACTOE_H
#define MASTER_TICTACTOE_H

#include <utility>

enum Result {
    Draw,
    BlackWin,
    WhiteWin,
    Nothing
};

typedef std::pair<bool, Result> GameEval;


class TicTacToe {
public:
    GameEval moveWhite(short actionCode);
    GameEval moveBlack(short actionCode);

    void reset();

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
};




#endif //MASTER_TICTACTOE_H
