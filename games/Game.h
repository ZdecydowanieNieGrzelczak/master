#ifndef MASTER_GAME_H
#define MASTER_GAME_H

#include <vector>

enum Result {
    Draw,
    PlayerWin,
    PlayerLost,
    Nothing
};

typedef std::pair<bool, Result> GameEval;

class Game {
public:

    virtual std::vector<float> getState() = 0;

    int getStateSize() const { return stateSize; }

    int getActionSize() const { return actionSize; }

    virtual std::vector<float> getInitialState() = 0;

    virtual GameEval doAction(int i) = 0;

    virtual GameEval reset() = 0;


private:
    int stateSize{0};
    int actionSize{0};

protected:

    bool isPlayerWhite;
};
#endif //MASTER_GAME_H
