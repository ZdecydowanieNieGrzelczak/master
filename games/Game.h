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

    virtual std::vector<float> getState();

    int getStateSize() const { return stateSize; }

    int getActionSize() const { return actionSize; }

    virtual std::vector<float> getInitialState();

    virtual GameEval doAction(int i);

    virtual GameEval reset();


private:
    int stateSize{0};
    int actionSize{0};

protected:

    bool isPlayerWhite;
};
#endif //MASTER_GAME_H
