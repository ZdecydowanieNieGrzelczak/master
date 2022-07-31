
#ifndef MASTER_TICTACTOE_H
#define MASTER_TICTACTOE_H

#include <utility>
#include <cassert>
#include <map>

#include "../Game.h"
#include "../../utils/HelperMethods.h"
#include "../../utils/SharedDefinitions.h"


class TicTacToe : public Game {
public:
    GameEval movePlayer(int actionCode);
    GameEval moveEnemy(int actionCode);

    GameEval reset() override;
    GameEval doAction(int i) override;

    std::vector<float> getInitialState() override;
    virtual std::vector<float> getState();

    [[nodiscard]] int getStateSize() const override { return TIC_TAC_TOE_STATE_SIZE; }

    [[nodiscard]] int getActionSize() const override { return TIC_TAC_TOE_ACTION_SIZE; };

    GameEval doBestAction(const std::vector<std::pair<int, float>> &actionsVec);

    std::vector<int> getAvalActions(int state) {return avalAction2[state]; }

private:
    short counter{0};
    bool isWhiteMoving{true};
    short playerState{0};
    short enemyState{0};
    constexpr const static int winCombinations[8] = {
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
    constexpr const static int actions[TIC_TAC_TOE_ACTION_SIZE] = {
            0b100000000,
            0b010000000,
            0b001000000,
            0b000100000,
            0b000010000,
            0b000001000,
            0b000000100,
            0b000000010,
            0b000000001,
    };

//   1, 0, 1
//   1, 0, 0
//   0, 0, 0
//    constexpr const static int forkCombination[8] = {
//            0b110100000,
//            0b110100000,
//            0b110100000,
//            0b110100000,
//            0b110100000,
//            0b110100000,
//            0b110100000,
//            0b110100000,
//    };

//
//    const std::map<std::pair<int, int>, int> actionMap = {
//            {{0b000000000, 0b000000000 }, 4},
//            {{0b000000000, 0b000000001 }, 4},
//            {{0b000000000, 0b000000010 }, 4},
//            {{0b000000000, 0b000000100 }, 4},
//            {{0b000000000, 0b000001000 }, 4},
//            {{0b000000000, 0b000010000 }, 2},
//            {{0b000000000, 0b000100000 }, 4},
//            {{0b000000000, 0b001000000 }, 4},
//            {{0b000000000, 0b010000000 }, 4},
//            {{0b000000000, 0b100000000 }, 4},
//            //
//            {{0b100000000, 0b000010000 }, 1},
//            {{0b100000000, 0b010010000 }, 7},
//            {{0b100000000, 0b001010000 }, 6},
//            {{0b100000000, 0b000110000 }, 5},
//            {{0b100000000, 0b000011000 }, 3},
//            {{0b100000000, 0b000010100 }, 3},
//            {{0b100000000, 0b000010010 }, 1},
//            {{0b100000000, 0b000010001 }, 1},
//            //
//            {{0b010000000, 0b000010000 }, 1},
//            {{0b010000000, 0b010010000 }, 7},
//            {{0b010000000, 0b001010000 }, 6},
//            {{0b010000000, 0b000110000 }, 5},
//            {{0b010000000, 0b000011000 }, 3},
//            {{0b010000000, 0b000010100 }, 3},
//            {{0b010000000, 0b000010010 }, 1},
//            {{0b010000000, 0b000010001 }, 1},
//
//    };

    static const std::vector<std::vector<int>> avalAction;
    static const std::vector<std::vector<int>> avalAction2;

    std::vector<float> initialVector =  {0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0};

    bool isLegal(int action);

    int getRandomAction();

    int getSmartAction();

    void printState();
};




#endif //MASTER_TICTACTOE_H
