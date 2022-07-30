#include <queue>
#include "TicTacToe.h"

typedef std::pair<int, float> act;

GameEval TicTacToe::moveWhite(int actionCode) {
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

GameEval TicTacToe::moveBlack(int actionCode) {
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
        return std::pair<bool, float>{true, INVALID_PENALTY / (counter + 1.0) + LOSE};
    }
    GameEval ret;
    if(isPlayerWhite) {
        ret = moveWhite(actions[i]);
        if (!ret.first) {
            int roll = HelperMethods::getRandomInt(0, 100);
            int enemyAction;
//            if (roll < RANDOM_ACTION_CHANCE) {
//                enemyAction = getRandomAction(nextState);
//            } else {
            enemyAction = getSmartAction(false);
//            }
            ret = moveBlack(actions[enemyAction]);
        }

    } else {
        ret = moveBlack(actions[i]);
        if (!ret.first) {
            bool invalid = true;
            int enemyAction = getSmartAction(false);
            ret = moveWhite(actions[enemyAction]);
        }
    }
    return ret;
}


int TicTacToe::getRandomAction() {
    int nextState = blackState | whiteState;
    bool invalid = true;
    int enemyAction;
    while(invalid) {
        enemyAction = rand() % 9;
        auto actionCode = actions[enemyAction];
        invalid = (actionCode | nextState) == nextState;
    }
    return enemyAction;
}

std::vector<float> TicTacToe::getInitialState() {
    return initialVector;
}

std::vector<float> TicTacToe::getState() {
    std::vector<float> state;
    if (isPlayerWhite) {
        for (auto action : actions) {
            state.push_back((action & whiteState) == action);
        }
        for (auto action : actions) {
            state.push_back((action & blackState) == action);
        }
    } else {
        for (auto action : actions) {
            state.push_back((action & blackState) == action);
        }
        for (auto action : actions) {
            state.push_back((action & whiteState) == action);
        }
    }
    return state;
}

GameEval TicTacToe::doBestAction(const std::vector<std::pair<int, float>> &actionsVec) {
    auto cmp = [](act left, act right) { return left.second < right.second; };
    std::priority_queue<act, std::vector<act>, decltype(cmp)> q3(actionsVec.begin(), actionsVec.end(), cmp);
//    std::priority_queue<act, std::vector<act>, decltype(cmp)> q3(cmp);


    while (!q3.empty()) {
        const auto [action, prob] = q3.top();
        q3.pop();
//        std::cout << action << " " << prob << std::endl;
        if (isLegal(action)) {
            return doAction(action);
        }
    }
    std::cout << "Invalid all actions!" << std::endl;

    std::cout << blackState << std::endl;
    std::cout << whiteState << std::endl;

    throw std::invalid_argument( "That was wrong" );
}

bool TicTacToe::isLegal(int action) {
    int currState = whiteState | blackState;
    int nextState = currState | actions[action];
    return nextState != currState;
}

int TicTacToe::getSmartAction(bool forWhite) {
    int playerBoard = forWhite ? whiteState : blackState;
    int enemyBoard = forWhite ? blackState : whiteState;

    int currentBoard = playerBoard | enemyBoard;
    auto &possibleActions = avalAction[currentBoard];
    int goodAction = -1;
    for (auto win : winCombinations) {
        for (auto action : possibleActions) {
            if (((playerBoard | actions[action]) & win) == win) {
                return action;
            }
        }
        for (auto action : possibleActions) {
            if (((enemyBoard | actions[action]) & win) == win) {
                return action;
            }
        }
    }

    return getRandomAction();

}


