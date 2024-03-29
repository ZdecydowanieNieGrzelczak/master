#ifndef MASTER_SHAREDDEFINITIONS_H
#define MASTER_SHAREDDEFINITIONS_H

#define CONN_MUTATION_FACTOR 1.5f
#define NEURON_MUTATION_FACTOR 1.5f

#define GAMES_PER_ITER 1000
#define NETWORK_MUTATION_CHANCE 95
#define NETWORK_MUTATION_CHANCE_AFTER_RECOMBINATION 20
#define CONNECTION_MUTATION_RATE 0.30
#define NEURON_MUTATION_RATE 0.05


// Chances
#define WEIGHTS_MUTATION_RATE 75
#define CONN_TOGGLING_RATE 5
#define STRUCTURE_MUTATION_RATE 2
#define MAX_TRIES 6

#define NEURON_CREATION_RATE 20

// How many children we will have
#define POPULATION_COUNT 1500

#define CROSS_BREED_POPULATION 0

#define GENERATION_COUNT 4000
#define TEST_BATTLES_COUNT 1

#define TIC_TAC_TOE_ACTION_SIZE 9
#define TIC_TAC_TOE_STATE_SIZE 18

#define BEST_COPY_COUNT 3
#define BEST_COPY_WITH_PROCESSING 0

#define WIN 1.0
#define DRAW 0.0
#define LOSE -2
#define INVALID_PENALTY -10

#define NOT_FOUND_CODE 404


// Simplified neuron things
#define SCORE_PENALTY_BASE 0.995
#define GENERATION_MUTATION_PERC 0.15
#define NO_EXPANSION_INTERVAL 100
#define EXPANSION_INTERVAL 50

#define FILENAME_BASE "opp-vol3-stan-centre"

#define PRUNE_EVERY_N 100

#define REMOVE_NEURONS_COUNT 3

#define SIMILARITY_THRESHOLD 5

#define THREAD_NUM 8


#endif //MASTER_SHAREDDEFINITIONS_H
