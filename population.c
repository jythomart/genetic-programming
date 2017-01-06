#include <stdlib.h>
#include <math.h>
#include "population.h"

t_population  *population_create(unsigned int elite, unsigned int crossover, unsigned int newcomer, int nbFeatures) {
    t_population *pop = malloc(sizeof(t_population));
    pop->size = elite + crossover + newcomer;
    pop->eliteSize = elite;
    pop->crossoverSize = crossover;
    pop->newcomerSize = newcomer;

    pop->candidates = malloc(pop->size * sizeof(t_node*));
    pop->swapBuffer = malloc(pop->size * sizeof(t_node*));
    pop->results = malloc(pop->size * sizeof(float));
    for (unsigned int i = 0; i < pop->size; ++i) {
        pop->candidates[i] = tree_generate(nbFeatures);
    }
    return pop;
}

void            population_destroy(t_population *this) {
    // TODO
}

void            population_print(t_population *this) {
    int idx = 0;
    fprintf(stdout, "--- ELITES:\n");
    for (int i = 0; i < this->eliteSize; ++i) {
        node_toJSON(this->candidates[idx], stdout);
        fprintf(stdout, " = %f\n", this->results[idx]);
        ++idx;
    }
    // fprintf(stdout, "--- CROSSOVER:\n");
    // for (int i = 0; i < this->crossoverSize; ++i) {
    //     node_toJSON(this->candidates[idx], stdout);
    //     fprintf(stdout, " = %f\n", this->results[idx]);
    //     ++idx;
    // }
    // fprintf(stdout, "--- NEWCOMER:\n");
    // for (int i = 0; i < this->newcomerSize; ++i) {
    //     node_toJSON(this->candidates[idx], stdout);
    //     fprintf(stdout, " = %f\n", this->results[idx]);
    //     ++idx;
    // }
}

void            population_contest(t_population *this, float const **featureSets, int nbSets, int nbFeatures, float(*scoreFunc)(float, float)) {
    for (int i = 0; i < this->size; ++i) {
        this->results[i] = 0.0;
        for (int j = 0; j < nbSets; ++j) {
            this->results[i] += (*scoreFunc)(featureSets[j][nbFeatures], node_getValue(this->candidates[i], featureSets[j]));
        }
        this->results[i] = this->results[i] / (float) nbSets;
        if (isnan(this->results[i]))
            this->results[i] = 99999.0;
    }
}

static int     population_orderByScorePartition(t_population *this, int leftIdx, int rightIdx) {
    int i = leftIdx;
    int j = rightIdx + 1;
    float tmp;
    t_node *tmpNode;

    float pivot = this->results[leftIdx];

    while(1) {
        do ++i; while(this->results[i] <= pivot && i <= rightIdx);
        do --j; while(this->results[j] > pivot);
        if (i >= j)
            break;
        // swap results
        tmp = this->results[i];
        this->results[i] = this->results[j];
        this->results[j] = tmp;

        // swap candidates
        tmpNode = this->candidates[i];
        this->candidates[i] = this->candidates[j];
        this->candidates[j] = tmpNode;
    }
    tmp = this->results[leftIdx];
    this->results[leftIdx] = this->results[j];
    this->results[j] = tmp;

    // swap candidates
    tmpNode = this->candidates[leftIdx];
    this->candidates[leftIdx] = this->candidates[j];
    this->candidates[j] = tmpNode;
    return j;
}

static void     population_orderByScoreQuickSort(t_population *this, int leftIdx, int rightIdx) {
    int pivot;

    if (leftIdx < rightIdx) {
        pivot = population_orderByScorePartition(this, leftIdx, rightIdx);
        population_orderByScoreQuickSort(this, leftIdx, pivot - 1);
        population_orderByScoreQuickSort(this, pivot + 1, rightIdx);
    }
}

void            population_orderByScore(t_population *this) {
    population_orderByScoreQuickSort(this, 0, this->size - 1);
}

void            population_increment(t_population *this, int nbFeatures) {
    // pick the elite
    int idx = 0;
    int j = 0;
    float lastResult = -1.0;
    while (idx < this->eliteSize && j < this->size) {
        if (j == 0 || this->results[j] > lastResult) { // maybe compare node by node instead of just the result?
            this->swapBuffer[idx] = node_duplicate(this->candidates[j]);
            ++idx;
            lastResult = this->results[j];
        }
        ++j;
    }
    // printf("blabla %i\n", idx);
    while (idx < this->eliteSize) {
        this->swapBuffer[idx] = tree_generate(nbFeatures);
        ++idx;
    }

    // for (int i = 0; i < this->eliteSize; ++i) {
    //     this->swapBuffer[i] = this->candidates[i];
    // }

    // make the elite reproduce with middle ones
    int crossoverOffset = this->eliteSize;
    int crossoverParentRange = this->eliteSize + this->crossoverSize;
    // int crossoverParentRange = this->size;
    for (int i = 0; i < this->crossoverSize; ++i) {
        int parent1Idx = rand() % crossoverParentRange;
        int parent2Idx = rand() % crossoverParentRange;
        this->swapBuffer[crossoverOffset + i] = tree_crossover(this->candidates[parent1Idx], this->candidates[parent2Idx]);
    }

    // add newcomers
    int newcomerOffset = crossoverOffset + this->crossoverSize;
    for (int i = 0; i < this->newcomerSize; ++i) {
        this->swapBuffer[newcomerOffset + i] = tree_generate(nbFeatures);
    }

    // swap back into candidates
    t_node **tmp = this->candidates;
    this->candidates = this->swapBuffer;
    this->swapBuffer = tmp;
    for (int i = 0; i < this->size; ++i) {
        node_delete(&this->swapBuffer[i]);
    }
}

void            population_mutate(t_population *this, unsigned int mutants, int nbFeatures) {
    unsigned int idxRange = this->eliteSize + this->crossoverSize;
    for (int i = 0; i < mutants; ++i) {
        unsigned int mutantIdx = rand() % idxRange;
        tree_mutateOne(this->candidates[mutantIdx], nbFeatures);
    }
}