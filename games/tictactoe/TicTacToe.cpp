#include <stdexcept>
#include <cassert>
#include <random>
#include "TicTacToe.h"



GameEval TicTacToe::moveWhite(short actionCode) {
    assert(isWhiteMoving);
    assert((whiteState & actionCode) != whiteState);
    whiteState = whiteState & actionCode;
    for(const short winCombination : winCombinations) {
        if ((whiteState & winCombination) == winCombination) {
            return std::pair<bool, Result>{true, isPlayerWhite ? Result::PlayerWin : Result::PlayerLost};
        }
    }
    if (++counter == 9) {
        return std::pair<bool, Result>{true, Result::Draw};
    }
    isWhiteMoving = false;
    return std::pair<bool, Result>{false, Result::Nothing};


}

GameEval TicTacToe::moveBlack(short actionCode) {
    assert(!isWhiteMoving);
    assert((blackState & actionCode) != blackState);
    blackState = blackState & actionCode;
    for(const short winCombination : winCombinations) {
        if ((blackState & winCombination) == winCombination) {
            return std::pair<bool, Result>{true, !isPlayerWhite ? Result::PlayerWin : Result::PlayerLost};

        }
    }
    if (++counter == 9) {
        return std::pair<bool, Result>{true, Result::Draw};
    }
    isWhiteMoving = true;
    return std::pair<bool, Result>{false, Result::Nothing};
}

GameEval TicTacToe::reset() {
    whiteState = 0;
    blackState = 0;
    counter = 0;
    isWhiteMoving = true;
    isPlayerWhite = rand() % 100 >= 50;
    return std::pair<bool, Result>{false, Result::Nothing};
}

GameEval TicTacToe::doAction(int i) {
    int currState = whiteState & blackState;
    int nextState = currState & actions[i];
    if (currState == nextState) {
        // Do not tolerate invalid moves.
        return std::pair<bool, Result>{true, Result::PlayerLost};
    }
    GameEval ret;
    if(isPlayerWhite) {
        ret = moveWhite(actions[i]);
        if (!ret.first) {
            bool invalid = true;
            int enemyAction;
            while(invalid) {
                enemyAction = rand() % 9;
                auto actionCode = actions[i];
                invalid = (actionCode & nextState) == actionCode;
            }
            ret = moveBlack(enemyAction);
        }

    } else {
        ret = moveBlack(actions[i]);
        if (!ret.first) {
            bool invalid = true;
            int enemyAction;
            while(invalid) {
                enemyAction = rand() % 9;
                auto actionCode = actions[i];
                invalid = (actionCode & nextState) == actionCode;
            }
            ret = moveWhite(enemyAction);
        }
    }
    return ret;
}


