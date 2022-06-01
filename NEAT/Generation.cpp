#include <iostream>
#include <fstream>
#include "Generation.h"
#include "../games/tictactoe/TicTacToe.h"
#include "Spiecie.h"

StructureMutator* ledger;

Generation::Generation(int generationCount, Game* game): game{game} {
    ledger = new StructureMutator();
    members.reserve(generationCount);
    ledger->neuronInnovationCounter = game->getStateSize() + game->getActionSize();
    for(int i = 0; i < generationCount; ++i) {
        auto net = new Network(game->getStateSize(), game->getActionSize(), i);
        members.push_back(net);
        spiecies.emplace_back(i, net);
    }
}

Network * Generation::iterateFor(int iterationCount) {
    std::cout << "Beginning iteration!" << std::endl;

    for(int i = 0; i < iterationCount; ++i) {
        runThroughGeneration();
    }
    return members.at(0);
}

void Generation::runThroughGeneration() {
    float bestScore{-999999999999999999999999.0f};
    int bestIndex;
    memberScores.clear();
    for(int x = 0; x < members.size(); ++x) {
        auto member = members[x];
        float score = 0;
        for(int y = 0; y < GAMES_PER_ITER; ++y) {
            auto res = game->reset();
            while(!res.first) {
                auto currentState = game->getState();
                auto bestAction = member->passThroughNetwork(currentState);
                res = game->doAction(bestAction);
            }
            score += res.second;
        }
        score = score > 0 ? score / spiecies.at(member->spiecieID).getCount()
                : score * spiecies.at(member->spiecieID).getCount();
        memberScores.push_back(score);
        if (score > bestScore) {
            bestScore = score;
            bestIndex = x;
        }
    }
    std::cout << "Iteration: " << ++generationCounter << " done. Best Score: " << bestScore << std::endl;
    generationScores.push_back(bestScore);
    members = createNewGeneration(bestIndex);


}

std::vector<Network *> Generation::createNewGeneration(int bestIndex) {
    auto newMembers = std::vector<Network*>();
    std::vector<Spiecie> newSpiecies;
    auto bestNetwork = members.at(bestIndex);


    for (int x = 0; x < BEST_COPY_COUNT; ++x) {
        addToSpiecies(bestNetwork, newSpiecies);
        newMembers.push_back(new Network(*bestNetwork, x));

    }

    std::cout << "Created best member for: " << generationCounter << std::endl;
    std::cout << "Size of hidden layer: " << bestNetwork->getHiddenSize() << std::endl;
    std::cout << "Size of connections: " << bestNetwork->getConnectionsSize() << std::endl;
    std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++: " << std::endl;



    for (int i = BEST_COPY_COUNT; i < members.size(); ++i) {
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
        auto newMember = new Network(*members.at(index), i);
        addToSpiecies(newMember, newSpiecies);

        if (rand() % 100 <= NETWORK_MUTATION_CHANCE) {
            newMember->mutate();
        }
        newMembers.push_back(newMember);
    }

    for (auto member : members) {
        delete member;
    }

    spiecies.clear();
    spiecies = newSpiecies;

    std::cout << "Created generation: " << generationCounter << std::endl;
    std::cout << "Species size : " << GENERATION_COUNT << "/" << spiecies.size() << std::endl;

    return newMembers;
}

void Generation::addToSpiecies(Network *net, std::vector<Spiecie> &newSpiecies) {
    for (auto & spiecie : newSpiecies) {
        if(spiecie.testAndAdd(*net)) {
            net->spiecieID = spiecie.getID();
            return;
        }
    }
    net->spiecieID = newSpiecies.size();
    newSpiecies.emplace_back(newSpiecies.size(), net);
}

double Generation::testFor(int iterationCount, Network &network) {
    double score = 0.0;
    for(int x = 0; x < iterationCount; ++x) {
        auto res = game->reset();
        while(!res.first) {
            auto currentState = game->getState();
            auto bestAction = network.passThroughNetwork(currentState);
            res = game->doAction(bestAction);
        }

        score += res.second;
    }
    return score;
}

void Generation::saveTheScore(const std::string &filename) const {
    std::ofstream scoreFile;
    const auto originalName = "../savedData/" + filename + "_scores.csv";
    auto name = originalName;
    int counter{0};
    while (HelperMethods::nameTest(name)) {
        name = originalName + std::to_string(counter++);
    }
    scoreFile.open(name);
    for (const auto score : generationScores) {
        scoreFile << score << ";";
    }
    scoreFile << std::endl;
    scoreFile.close();
}

void Generation::saveTheNetwork(const std::string &filename) const {
    std::ofstream networkFile;
    const auto originalName = "../savedData/" + filename + "_network.csv";
    auto name = originalName;
    int counter{0};
    while (HelperMethods::nameTest(name)) {
        name = originalName + std::to_string(counter++);
    }
    networkFile.open(name);
    for (const auto& network : members) {
        networkFile << "Network:" << std::endl;
        networkFile << network->getSaveData() << std::endl;
    }
    networkFile << std::endl;
    networkFile.close();
}


