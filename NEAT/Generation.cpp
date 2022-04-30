#include <iostream>
#include "Generation.h"



Generation::Generation(int generationCount, Game* game): game{game} {
    members.reserve(generationCount);
    for(int i = 0; i < generationCount; ++i) {
        members.push_back(new Network(game->getStateSize(), game->getActionSize()));
    }
}

Network Generation::iterateFor(int iterationCount) {
    std::cout << "Beginning iteration!" << std::endl;

    for(int i = 0; i < iterationCount; ++i) {
        runThroughGeneration();
    }
    return *members.at(0);
}

void Generation::runThroughGeneration() {
    const auto initialVector = game->getInitialState();
    float bestScore{GAMES_PER_ITER * INVALID_PENALTY - 10  };
    int bestIndex;

    for(int x = 0; x < members.size(); ++x) {
        auto member = members[x];
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
                case Result::InvalidMove:
                    score += INVALID_PENALTY;
                    break;
                case Nothing:
                    std::cout << "Should not have happen!" << std::endl;
                    break;
            }
        }
        memberScores.push_back(score);
        if (score > bestScore) {
            bestScore = score;
            bestIndex = x;
        }
    }
    std::cout << "Iteration: " << ++generationCounter << " done. Best Score: " << bestScore << std::endl;
    members = createNewGeneration(bestIndex);


}

std::vector<Network *> Generation::createNewGeneration(int bestIndex) {
    auto newMembers = std::vector<Network*>();
    auto bestNetwork = members.at(bestIndex);

    newMembers.push_back(new Network(*bestNetwork));

    std::cout << "Created best member for!: " << generationCounter << std::endl;


    for (int i = 1; i < members.size(); ++i) {
        int first = rand() % members.size();
        int second = rand() % members.size();
        int index;
        assert(first >= 0);
        assert(first < memberScores.size());
        assert(second >= 0);
        assert(second < memberScores.size());
        if (memberScores.at(first) > memberScores.at(second)) {
            index = first;
        } else {
            index = second;
        }

        assert(index >= 0);
        assert(index < members.size());
        auto newMember = new Network(*members.at(index));
        if (rand() % 100 / 100 > NETWORK_MUTATION_CHANCE) {
            newMember->mutate();
        }
        newMembers.push_back(newMember);
    }

    for (auto member : members) {
        delete member;
    }

    std::cout << "Created generation: " << generationCounter << std::endl;

    return newMembers;
}

double Generation::testFor(int iterationCount, Network network) {
    double score = 0.0;
    for(int x = 0; x < iterationCount; ++x) {
        auto res = game->reset();
        while(!res.first) {
            auto currentState = game->getState();
            auto bestAction = network.passThroughNetwork(currentState);
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
    return score;
}


