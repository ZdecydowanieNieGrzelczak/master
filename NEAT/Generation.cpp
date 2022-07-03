#include <iostream>
#include <fstream>
#include "Generation.h"
#include "../games/tictactoe/TicTacToe.h"
#include "Spiecie.h"
#include "SimplifiedNeat.h"
#include "StandardNeat.h"
#include <omp.h>
#include <chrono>
#include <ctime>   
#include <filesystem>

StructureMutator* ledger;

Generation::Generation(int generationCount, Game* game): game{game} {
    ledger = new StructureMutator();
    members.reserve(generationCount);
    memberScores = std::vector<float>(generationCount);
    ledger->neuronInnovationCounter = game->getStateSize() + game->getActionSize();
    for(int i = 0; i < generationCount; ++i) {
        auto net = new StandardNeat(game->getStateSize(), game->getActionSize(), i);
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

    std::vector<float> _scores(THREAD_NUM, -999999999999999.0f);
    std::vector<float> _rawScores( THREAD_NUM, -999999999999.0f);
    std::vector<int> _bestIndex( THREAD_NUM, 0);


    #pragma omp parallel for default(none) shared(_scores, _rawScores, _bestIndex, members, spiecies)
    for(int x = 0; x < members.size(); ++x) {
        TicTacToe privateGame = TicTacToe();
        auto member = members[x];
        float score = 0;
        for(int y = 0; y < GAMES_PER_ITER; ++y) {
            auto res = privateGame.reset();
            while(!res.first) {
                auto currentState = privateGame.getState();
                auto bestAction = member->passThroughNetwork(currentState);
                res = privateGame.doAction(bestAction);
            }
            score += res.second;
        }
        float rawScore = score;
        score = score > 0 ? score / spiecies.at(member->spiecieID).getCount()
                : score * spiecies.at(member->spiecieID).getCount();
        memberScores.at(x) = score;
        if (score > _scores[omp_get_thread_num()]) {
            _scores[omp_get_thread_num()] = score;
        }
        if (rawScore > _rawScores[omp_get_thread_num()]) {
            _bestIndex[omp_get_thread_num()] = x;
            _rawScores[omp_get_thread_num()] = rawScore;
        }
    }

    float bestScore = -999999999.0f;
    float bestRawScore = -999999999.0f;
    int bestIndex = -999999999;
    for (int i = 0; i < THREAD_NUM; ++i) {
        if (bestScore < _scores[i]) {
            bestScore = _scores[i];
        }
        if (bestRawScore < _rawScores[i]) {
            bestIndex = _bestIndex[i];
            bestRawScore = _rawScores[i];
        }
    }

    std::cout << "Iteration: " << ++generationCounter << " done. Best Score: " << bestScore << std::endl;
    std::cout << "Best Raw Score: " << bestRawScore << std::endl;
    generationScores.push_back(bestScore);
    generationRawScores.push_back(bestRawScore);
    generationSizes.emplace_back(members[bestIndex]->getHiddenSize(), members[bestIndex]->getConnectionsSize());
    members = createNewGeneration(bestIndex);


}

std::vector<Network *> Generation::createNewGeneration(int bestIndex) {
    auto newMembers = std::vector<Network*>();
    std::vector<Spiecie> newSpiecies;
    auto bestNetwork = members.at(bestIndex);


    for (int x = 0; x < BEST_COPY_COUNT; ++x) {
        addToSpiecies(bestNetwork, newSpiecies);
        newMembers.push_back(new StandardNeat(*bestNetwork, x));
    }

    std::time_t end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::cout << "Created best member for: " << generationCounter << std::endl;
    std::cout << "Size of hidden layer: " << bestNetwork->getHiddenSize() << std::endl;
    std::cout << "Size of connections: " << bestNetwork->getConnectionsSize() << std::endl;
    std::cout << "Timestamp: " << std::ctime(&end_time) << std::endl;
    std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++: " << std::endl;



    for (int i = BEST_COPY_COUNT; i < members.size(); ++i) {
        int first = rand() % members.size();
        int second = rand() % members.size();
        int index;
//        assert(first >= 0);
//        assert(first < memberScores.size());
//        assert(second >= 0);
//        assert(second < memberScores.size());
        if (memberScores.at(first) > memberScores.at(second)) {
            index = first;
        } else {
            index = second;
        }

//        assert(index >= 0);
//        assert(index < members.size());
        auto newMember = new StandardNeat(*members.at(index), i);
        addToSpiecies(newMember, newSpiecies);

        if (rand() % 100 <= NETWORK_MUTATION_CHANCE) {
            newMember->mutate(generationCounter);
        }
        newMembers.push_back(newMember);
    }

    for (auto member : members) {
        delete member;
    }

    spiecies.clear();
    spiecies = newSpiecies;

    spieciesCounter.push_back(spiecies.size());

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

void Generation::saveTheScore(const std::string &filename, std::vector<float> &scoreVec) {
    std::ofstream scoreFile;
    const auto originalName = filename + "_scores";
    auto name = originalName + ".csv";
    int counter{0};
    while (HelperMethods::nameTest(name)) {
        name = originalName + std::to_string(counter++) + ".csv";
    }
    scoreFile.open(name);
    for (const auto score : scoreVec) {
        scoreFile << score << ";";
    }

    scoreFile << std::endl;
    scoreFile.close();
}

void Generation::saveTheNetwork(const std::string &filename) const {
    std::ofstream networkFile;
    const auto originalName = filename + "_network";
    auto name = originalName + ".csv";
    int counter{0};
    while (HelperMethods::nameTest(name)) {
        name = originalName + std::to_string(counter++) + ".csv";
    }
    networkFile.open(name);
    for (const auto& network : members) {
        networkFile << "Network:" << std::endl;
        networkFile << network->getSaveData() << std::endl;
    }
    networkFile << std::endl;
    networkFile.close();
}

void Generation::saveTheSize(const std::string &filename) const {
    std::ofstream scoreFile;
    const auto originalName = filename + "_sizes";
    auto name = originalName + ".csv";
    int counter{0};
    while (HelperMethods::nameTest(name)) {
        name = originalName + std::to_string(counter++) + ".csv";
    }
    scoreFile.open(name);
    for (const auto score : generationSizes) {
        scoreFile << score.first << ";" << score.second << ";" << std::endl;
    }
    scoreFile << std::endl;
    scoreFile.close();
}

void Generation::saveTheSpiecies(const std::string &filename) const {
    std::ofstream scoreFile;
    const auto originalName = filename + "_spiecies";
    auto name = originalName + ".csv";
    int counter{0};
    while (HelperMethods::nameTest(name)) {
        name = originalName + std::to_string(counter++) + ".csv";
    }
    scoreFile.open(name);
    std::cout << name << std::endl;
    for (const auto score : spieciesCounter) {
        scoreFile << score << ";" << std::endl;
    }
    scoreFile << std::endl;
    scoreFile.close();
}

void Generation::saveTheScores(const std::string& string) {
    saveTheScore(string, generationScores);
    saveTheScore(string + "_raw_", generationRawScores);
    saveTheSize(string + "_sizes_");
    saveTheSpiecies(string);
}


