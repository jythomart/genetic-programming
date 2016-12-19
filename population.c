#include <stdlib.h>
#include "population.h"

t_population  *population_create(unsigned int size, unsigned int elite, unsigned int crossover, unsigned int newcomer, int nbFeatures) {
    t_population *pop = malloc(sizeof(t_population));
    pop->size = size;
    pop->elitePercent = elite;
    pop->crossoverPercent = crossover;
    pop->newcomerPercent = newcomer;

    pop->candidates = malloc(size * sizeof(t_node*));
    pop->swapBuffer = malloc(size * sizeof(t_node*));
    pop->results = malloc(size * sizeof(float));
    for (unsigned int i = 0; i < size; ++i) {
        pop->candidates[i] = tree_generate(nbFeatures);
    }
    return pop;
}

void            population_destroy(t_population *this) {
    // TODO
}

void            population_contest(t_population *this, float const **featureSets, int nbSets, int nbFeatures, float(*scoreFunc)(float, float)) {
    for (int i = 0; i < this->size; ++i) {
        this->results[i] = 0.0;
        for (int j = 0; j < nbSets; ++j) {
            this->results[i] += (*scoreFunc)(featureSets[j][nbFeatures], node_getValue(this->candidates[i], featureSets[j]));
        }
        this->results[i] = this->results[i] / (float) nbSets;
    }
}

static int     population_orderByScorePartition(t_population *this, int leftIdx, int rightIdx) {
    int i = leftIdx;
    int j = rightIdx + 1;
    float tmp;
    
    float pivot = this->results[leftIdx];

    while(1) {
   	    do ++i; while(this->results[i] <= pivot && i <= rightIdx);
   	    do --j; while(this->results[j] > pivot);
   	    if(i >= j)
           break;
   	    tmp = this->results[i];
        this->results[i] = this->results[j];
        this->results[j] = tmp;
   }
   tmp = this->results[leftIdx];
   this->results[leftIdx] = this->results[j];
   this->results[j] = tmp;
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
    for (int i = 0; i < this->size; ++i) {
        node_toJSON(this->candidates[i], stdout);
        fprintf(stdout, " = %f\n", this->results[i]);
    }
}

void            population_increment(t_population *this) {

}