#include <stdexcept>
#include <cassert>
#include "TicTacToe.h"



GameEval TicTacToe::moveWhite(short actionCode) {
    assert(isWhiteMoving);
    assert((whiteState & actionCode) != whiteState);
    whiteState = whiteState & actionCode;
    for(const short winCombination : winCombinations) {
        if ((whiteState & winCombination) == winCombination) {
            return std::pair<bool, Result>{true, Result::WhiteWin};
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
            return std::pair<bool, Result>{true, Result::BlackWin};

        }
    }
    if (++counter == 9) {
        return std::pair<bool, Result>{true, Result::Draw};
    }
    isWhiteMoving = true;
    return std::pair<bool, Result>{false, Result::Nothing};
}

void TicTacToe::reset() {
    whiteState = 0;
    blackState = 0;
    counter = 0;
    isWhiteMoving = true;
}


