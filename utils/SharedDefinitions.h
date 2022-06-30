#ifndef MASTER_SHAREDDEFINITIONS_H
#define MASTER_SHAREDDEFINITIONS_H

#define CONN_MUTATION_FACTOR 0.25f
#define NEURON_MUTATION_FACTOR 0.25f

#define GAMES_PER_ITER 2000
#define NETWORK_MUTATION_CHANCE 65
#define CONNECTION_MUTATION_RATE 0.30
#define NEURON_MUTATION_RATE 0.05


// Chances
#define WEIGHTS_MUTATION_RATE 75
#define CONN_TOGGLING_RATE 10
#define STRUCTURE_MUTATION_RATE 1
#define MAX_TRIES 6

#define NEURON_CREATION_RATE 20

// How many children we will have
#define POPULATION_COUNT 2000

#define GENERATION_COUNT 100
#define TEST_BATTLES_COUNT 1

#define TIC_TAC_TOE_ACTION_SIZE 9
#define TIC_TAC_TOE_STATE_SIZE 18

#define HIDDEN_LAYER_COUNT 40

#define BEST_COPY_COUNT 1
#define BEST_COPY_WITH_PROCESSING 10

#define WIN 1.0
#define DRAW 0.0
#define LOSE -1
#define INVALID_PENALTY -10

#define NOT_FOUND_CODE 404


#define FILENAME_BASE "16cores_job_standard-10k-HPC"

#define PRUNE_EVERY_N 25

#define REMOVE_NEURONS_COUNT 3

#define SIMILARITY_THRESHOLD 5

#define THREAD_NUM 8

#endif //MASTER_SHAREDDEFINITIONS_H
