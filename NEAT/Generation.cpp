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
        if (i > changeAt) {
            isStructureMutationPermitted = !isStructureMutationPermitted;
            changeAt += isStructureMutationPermitted ? EXPANSION_INTERVAL : NO_EXPANSION_INTERVAL;
        }
        runThroughGeneration();
    }
    return members.at(0);
}

void Generation::runThroughGeneration() {

    std::vector<float> _scores(THREAD_NUM, -999999999999999.0f);
    std::vector<float> _rawScores( THREAD_NUM, -999999999999.0f);
    std::vector<int> _bestIndex( THREAD_NUM, 0);
    std::vector<float> scoreSums(THREAD_NUM, 0.0f);
    std::vector<float> rawScoreSums(THREAD_NUM, 0.0f);
    std::vector<int> additionalSizes(THREAD_NUM, 0);


    #pragma omp parallel for default(none) shared(_scores, _rawScores, _bestIndex, members, spiecies, scoreSums, rawScoreSums, additionalSizes)
    for(int x = 0; x < members.size(); ++x) {
        TicTacToe privateGame = TicTacToe();
        auto member = members[x];
        int threadNumber = omp_get_thread_num();
        additionalSizes[threadNumber] += member->getAdditionalSize();
        float score = 0;
        for(int y = 0; y < GAMES_PER_ITER; ++y) {
            auto res = privateGame.reset();
            while(!res.first) {
                auto currentState = privateGame.getState();
//                auto bestAction = member->passThroughNetwork(currentState);
//                res = privateGame.doAction(bestAction);
                auto actions = member->passThroughNetworkWithActions(currentState);
                res = privateGame.doBestAction(actions);
            }
            score += res.second;
        }
        float rawScore = score;
//        score = score > 0 ? score / spiecies.at(member->spiecieID).getCount()
//                : score * spiecies.at(member->spiecieID).getCount();
        score = score * member->getScoreModifier();
        scoreSums[threadNumber] += score;
        rawScoreSums[threadNumber] += rawScore;
        memberScores.at(x) = score;
        if (score > _scores[threadNumber]) {
            _scores[threadNumber] = score;
        }
        if (rawScore > _rawScores[threadNumber]) {
            _bestIndex[threadNumber] = x;
            _rawScores[threadNumber] = rawScore;
        }
    }

    float bestScore = -999999999.0f;
    float bestRawScore = -999999999.0f;
    int bestIndex = -999999999;
    float scoreSum = 0;
    float rawScoreSum = 0;
    float averageAdditionalSize = 0;
    for (int i = 0; i < THREAD_NUM; ++i) {
        scoreSum += scoreSums[i];
        rawScoreSum += rawScoreSums[i];
        averageAdditionalSize += additionalSizes[i];

        if (bestScore < _scores[i]) {
            bestScore = _scores[i];
        }
        if (bestRawScore < _rawScores[i]) {
            bestIndex = _bestIndex[i];
            bestRawScore = _rawScores[i];
        }
    }

    scoreSum = scoreSum / members.size();
    rawScoreSum = rawScoreSum / members.size();
    averageAdditionalSize = averageAdditionalSize / members.size();

    std::cout << "Iteration: " << ++generationCounter << " done. Best Score: " << bestScore << std::endl;
    std::cout << "Best Raw Score: " << bestRawScore << std::endl;
    std::cout << "Average Raw Score: " << rawScoreSum << std::endl;
    std::cout << "Average Additional Size: " << averageAdditionalSize << std::endl;
    generationScores.push_back(bestScore);
    generationRawScores.push_back(bestRawScore);
    generationScoresAvg.push_back(scoreSum);
    generationRawScoresAvg.push_back(rawScoreSum);
    generationSizesAvg.push_back(averageAdditionalSize);
    generationSizes.emplace_back(members[bestIndex]->getHiddenSize(), members[bestIndex]->getConnectionsSize());
    members = createNewGeneration(bestIndex);


}

std::vector<Network *> Generation::createNewGeneration(int bestIndex) {
    auto newMembers = std::vector<Network*>();
    std::vector<Spiecie> newSpiecies;
    auto bestNetwork = members.at(bestIndex);


    for (int x = 0; x < BEST_COPY_COUNT; ++x) {
//        addToSpiecies(bestNetwork, newSpiecies);
        newMembers.push_back(new StandardNeat(*bestNetwork, x));
    }

    std::time_t end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::cout << "Created best member for: " << generationCounter << std::endl;
    std::cout << "Size of hidden layer: " << bestNetwork->getHiddenSize() << std::endl;
    std::cout << "Size of connections: " << bestNetwork->getConnectionsSize() << std::endl;
    std::cout << "Timestamp: " << std::ctime(&end_time) << std::endl;
    std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++: " << std::endl;

    for (int i = 0; i < BEST_COPY_WITH_PROCESSING; ++i) {
        auto newMember = new StandardNeat(*bestNetwork, i + BEST_COPY_COUNT);
        newMember->processBestNetwork(0);
        newMembers.push_back(newMember);
    }

    for (int i = newMembers.size(); i < CROSS_BREED_POPULATION; ++i) {
        int index1 = getTournamentIndex();
        int index2 = getTournamentIndex();

        auto newMember = new StandardNeat(*members.at(index1), *members.at(index2), i);

        if (HelperMethods::getRandomChance() <= NETWORK_MUTATION_CHANCE_AFTER_RECOMBINATION) {
            newMember->mutate(generationCounter, false);
        }

        newMembers.push_back(newMember);
    }


    for (int i = newMembers.size(); i < POPULATION_COUNT; ++i) {
        int index = getTournamentIndex();

        auto newMember = new StandardNeat(*members.at(index), i);
//        addToSpiecies(newMember, newSpiecies);

        if (HelperMethods::getRandomChance() <= NETWORK_MUTATION_CHANCE) {
            newMember->mutate(generationCounter, isStructureMutationPermitted);
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

void Generation::saveTheScore(const std::string& filename, std::vector<float> &scoreVec) {
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

void Generation::saveTheNetwork(const std::string& filename) const {
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

void Generation::saveTheSize(const std::string& filename) const {
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

void Generation::saveTheSpiecies(const std::string& filename) const {
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

void Generation::saveTheScores(const std::filesystem::path& path) {
    auto string = path.string();
    saveTheScore(string, generationScores);
    saveTheScore(string + "_raw_", generationRawScores);
    saveTheScore(string + "_score_avg_", generationScoresAvg);
    saveTheScore(string + "_raw__avg", generationRawScoresAvg);
    saveTheScore(string + "_sizes_avg", generationSizesAvg);
    saveTheSize(string + "_sizes_");
    saveTheSpiecies(string);
}

int Generation::getTournamentIndex() {
    int first = rand() % members.size();
    int second = rand() % members.size();
    return memberScores.at(first) > memberScores.at(second) ? first : second;
}


