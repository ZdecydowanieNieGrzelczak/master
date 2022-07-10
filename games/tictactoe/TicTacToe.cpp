#include "TicTacToe.h"


GameEval TicTacToe::moveWhite(short actionCode) {
//    assert(isWhiteMoving);
//    assert((whiteState | actionCode) != whiteState);
    whiteState = whiteState | actionCode;
    for(const short winCombination : winCombinations) {
        if ((whiteState & winCombination) == winCombination) {
            return std::pair<bool, float>{true, isPlayerWhite ? WIN : LOSE};
        }
    }
    if (++counter == 9) {
        return std::pair<bool, float>{true, DRAW};
    }
//    isWhiteMoving = false;
    return std::pair<bool, float>{false, 0};


}

GameEval TicTacToe::moveBlack(short actionCode) {
//    assert(!isWhiteMoving);
//    assert((blackState | actionCode) != blackState);
    blackState = blackState | actionCode;
    for(const short winCombination : winCombinations) {
        if ((blackState & winCombination) == winCombination) {
            return std::pair<bool, float>{true, !isPlayerWhite ? WIN : LOSE};

        }
    }
    if (++counter == 9) {
        return std::pair<bool, float>{true, DRAW};
    }
//    isWhiteMoving = true;
    return std::pair<bool, float>{false, 0};
}

GameEval TicTacToe::reset() {
    whiteState = 0;
    blackState = 0;
    counter = 0;
//    isWhiteMoving = true;
    isPlayerWhite = HelperMethods::getCoinFlip();
    if (!isPlayerWhite) {
        int enemyAction = rand() % 9;
        auto actionCode = actions[enemyAction];
        moveWhite(actionCode);
    }
    return std::pair<bool, float>{false, 0};
}

GameEval TicTacToe::doAction(int i) {
    int currState = whiteState | blackState;
    int nextState = currState | actions[i];
    if (currState == nextState) {
        // Do not tolerate invalid moves.
        return std::pair<bool, float>{true, INVALID_PENALTY / (counter + 1.0)};
    }
    GameEval ret;
    if(isPlayerWhite) {
        ret = moveWhite(actions[i]);
        if (!ret.first) {
            bool invalid = true;
            int enemyAction;
            while(invalid) {
                enemyAction = rand() % 9;
                auto actionCode = actions[enemyAction];
                invalid = (actionCode | nextState) == nextState;
            }
            ret = moveBlack(actions[enemyAction]);
        }

    } else {
        ret = moveBlack(actions[i]);
        if (!ret.first) {
            bool invalid = true;
            int enemyAction;
            while(invalid) {
                enemyAction = rand() % 9;
                auto actionCode = actions[enemyAction];
                invalid = (actionCode | nextState) == nextState;
            }
            ret = moveWhite(actions[enemyAction]);
        }
    }
    return ret;
}

std::vector<float> TicTacToe::getInitialState() {
    return initialVector;
}

std::vector<float> TicTacToe::getState() {
    std::vector<float> state;
//    if (isPlayerWhite) {
    state.push_back(isPlayerWhite);
    for (auto action : actions) {
        state.push_back((action & whiteState) == action);
    }
    for (auto action : actions) {
        state.push_back((action & blackState) == action);
    }
//        }
//    } else {
//        for (auto action : actions) {
//            state.push_back((action & blackState) == action);
//        }
//        for (auto action : actions) {
//            state.push_back((action & whiteState) == action);
//        }
//    }
    return state;
}




