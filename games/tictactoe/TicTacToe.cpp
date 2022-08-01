#include <queue>
#include <bitset>
#include "TicTacToe.h"

typedef std::pair<int, float> act;

GameEval TicTacToe::movePlayer(int actionCode) {
//    assert(isWhiteMoving);
//    assert((playerState | actionCode) != playerState);
//    std::bitset<16> y(actionCode);
//
//    std::cout << "Moving White with action: " << actionCode << std::endl;
//    std::cout << y << std::endl;
//    std::cout << "before" << std::endl;
//    printState();


    playerState = playerState | actionCode;

//    std::cout << "After" << std::endl;
//    printState();

    for(const short winCombination : winCombinations) {
        if ((playerState & winCombination) == winCombination) {
//            std::cout << playerState << std::endl;
//            std::cout << y << std::endl;
//            std::cout << winCombination << std::endl;
//            std::cout << x << std::endl;
            return std::pair<bool, float>{true, WIN};
        }
    }
    if (++counter == 9) {

        return std::pair<bool, float>{true, DRAW};
    }
//    isWhiteMoving = false;
    return std::pair<bool, float>{false, 0};


}

GameEval TicTacToe::moveEnemy(int actionCode) {
//    assert(!isWhiteMoving);
//    assert((enemyState | actionCode) != enemyState);
//    std::bitset<16> y(actionCode);
//
//
//    std::cout << "Moving Black with action: " << actionCode << std::endl;
//    std::cout << y << std::endl;
//    std::cout << "before" << std::endl;
//    printState();

    enemyState = enemyState | actionCode;

//    std::cout << "After" << std::endl;
//    printState();


    for(const short winCombination : winCombinations) {
        if ((enemyState & winCombination) == winCombination) {
            return std::pair<bool, float>{true, LOSE};

        }
    }
    if (++counter == 9) {
        return std::pair<bool, float>{true, DRAW};
    }
//    isWhiteMoving = true;
    return std::pair<bool, float>{false, 0};
}

GameEval TicTacToe::reset() {
    playerState = 0;
    enemyState = 0;
    counter = 0;
//    isWhiteMoving = true;
    isPlayerWhite = HelperMethods::getCoinFlip();
    if (!isPlayerWhite) {
        int enemyAction = rand() % 8;
        enemyAction = enemyAction + enemyAction > 3;
        auto actionCode = actions[enemyAction];
        initAction = actionCode;
        moveEnemy(actionCode);
    }
    return std::pair<bool, float>{false, 0};
}

GameEval TicTacToe::doAction(int i) {
    int currState = playerState | enemyState;
    int nextState = currState | actions[i];
    if (currState == nextState) {
        // Do not tolerate invalid moves.
        return std::pair<bool, float>{true, INVALID_PENALTY / (counter + 1.0) + LOSE};
    }
    GameEval ret;
    ret = movePlayer(actions[i]);
    if (!ret.first) {
        int enemyAction = getSmartAction();
        ret = moveEnemy(actions[enemyAction]);
    }

    return ret;
}


int TicTacToe::getRandomAction() const {
    int currentBoard = playerState | enemyState;

    auto &possibleActions = avalAction2.at(currentBoard);
    return possibleActions.at(HelperMethods::getRandomInt(0, (int)possibleActions.size()));
}

std::vector<float> TicTacToe::getInitialState() {
    return initialVector;
}

std::vector<float> TicTacToe::getState() {
    std::vector<float> state;
    for (auto action : actions) {
        state.push_back((action & playerState) == action);
    }
    for (auto action : actions) {
        state.push_back((action & enemyState) == action);
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

    std::cout << enemyState << std::endl;
    std::cout << playerState << std::endl;

    throw std::invalid_argument( "That was wrong" );
}

bool TicTacToe::isLegal(int action) {
    int currState = playerState | enemyState;
    int nextState = currState | actions[action];
    return nextState != currState;
}

int TicTacToe::getSmartAction() {


    int currentBoard = playerState | enemyState;

    auto &possibleActions = avalAction2.at(currentBoard);
    for (auto win : winCombinations) {
        for (auto action: possibleActions) {
            if (((enemyState | actions[action]) & win) == win) {
                return action;
            }
        }
    }
    for (auto win : winCombinations) {
        for (auto action : possibleActions) {
            if (((playerState | actions[action]) & win) == win) {
                return action;
            }
        }
    }

    if (!isPlayerWhite && counter == 2) {
        if(playerState != 0b000010000) {
            return 0b000010000;
        }
    }

    return getRandomAction();

}


void TicTacToe::printState() {
    auto state = getState();
    for (int i = 0; i < 3; ++i) {
        std::cout << "| ";
        for (int x = 0; x < 3; ++x) {
            std::string val = " ";
            if (state[x + i * 3]) {
                val = "x";
            }
            if (state[x + i * 3 + 9] ) {
                val = "o";
            }
            std::cout << val << " | ";
        }
        std::cout << std::endl;
    }

}

