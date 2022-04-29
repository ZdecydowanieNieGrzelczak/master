#include <iostream>
#include "Generation.h"



Generation::Generation(int generationCount, Game* game): game{game} {
    members.reserve(generationCount);
    for(int i = 0; i < generationCount; ++i) {
        members.push_back(new Network(game->getStateSize(), game->getActionSize()));
    }
}

Network Generation::iterateFor(int iterationCount) {
    for(int i = 0; i < iterationCount; ++i) {
        runThroughGeneration();
    }
}

void Generation::runThroughGeneration() {
    const auto initialVector = game->getInitialState();
    for(auto & member : members) {
        float score = 0;
        for(int x = 0; x < GAMES_PER_ITER; ++x) {
            auto res = game->reset();
            while(!res.first) {
                auto bestAction = member->passThroughNetwork(initialVector);
                res = game->doAction(bestAction);
            }

            switch (res.second) {
                case Result::PlayerWin:
                    score += WIN;
                    break;
                case Result::PlayerLost:
                    score += LOSE;
                    break;
                case Result::Draw:
                    score += DRAW;
                    break;
                case Nothing:
                    std::cout << "Should not have happen!" << std::endl;
                    break;
            }
        }
        memberScores.push_back(score);
    }
}


