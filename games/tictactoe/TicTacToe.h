
#ifndef MASTER_TICTACTOE_H
#define MASTER_TICTACTOE_H

#include <utility>
#include "../Game.h"


#define TIC_TAC_TOE_ACTION_SIZE 9
#define TIC_TAC_TOE_STATE_SIZE 18

#define WIN 1.0
#define DRAW 0.0
#define LOSE -1
#define INVALID_PENALTY -10

class TicTacToe : public Game {
public:
    GameEval moveWhite(short actionCode);
    GameEval moveBlack(short actionCode);

    GameEval reset() override;
    GameEval doAction(int i) override;

    std::vector<float> getInitialState() override;
    virtual std::vector<float> getState();

    [[nodiscard]] int getStateSize() const override { return TIC_TAC_TOE_STATE_SIZE; }

    [[nodiscard]] int getActionSize() const override { return TIC_TAC_TOE_ACTION_SIZE; };

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
    constexpr const static short actions[TIC_TAC_TOE_ACTION_SIZE] = {
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
    std::vector<float> initialVector =  {0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0};
};




#endif //MASTER_TICTACTOE_H
