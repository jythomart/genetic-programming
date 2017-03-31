#include <stdlib.h>
#include <math.h>
#include <pthread.h>    // TODO move to its own file
#include "population.h"

// static float computeScore(float actual, float predicted) {
//   return fabsf(actual - predicted);
// }

static float logLoss(float actual, float predicted) {
  return actual * logf(predicted) + (1 - actual) * logf(1 - predicted);
}

t_population  *population_create(unsigned int elite, unsigned int crossover, unsigned int newcomer, int nbFeatures) {
    t_population *pop = malloc(sizeof(t_population));
    pop->size = elite + crossover + newcomer;
    pop->eliteSize = elite;
    pop->crossoverSize = crossover;
    pop->newcomerSize = newcomer;
    pop->nbSpecies = 0;

    pop->candidates = malloc(pop->size * sizeof(t_node*));
    pop->swapBuffer = malloc(pop->size * sizeof(t_node*));
    pop->species = malloc(pop->size * sizeof(t_species));
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
    fprintf(stdout, "------POPULATION PRINT\n");
    int idx = 0;
    for (int i = 0; i < this->size; ++i) {
        fprintf(stdout, "(Specie %i): ", this->candidates[idx]->speciesId);
        node_print(this->candidates[idx], stdout);
        fprintf(stdout, " = %f\n", this->results[idx]);
        ++idx;
    }
    // fprintf(stdout, "--- ELITES:\n");
    // for (int i = 0; i < this->eliteSize; ++i) {
    //     fprintf(stdout, "(Specie %i): ", this->candidates[idx]->speciesId);
    //     node_print(this->candidates[idx], stdout);
    //     fprintf(stdout, " = %f\n", this->results[idx]);
    //     ++idx;
    // }
    // fprintf(stdout, "--- CROSSOVER:\n");
    // for (int i = 0; i < this->crossoverSize; ++i) {
    //     fprintf(stdout, "(Specie %i): ", this->candidates[idx]->speciesId);
    //     node_print(this->candidates[idx], stdout);
    //     fprintf(stdout, " = %f\n", this->results[idx]);
    //     ++idx;
    // }
    // fprintf(stdout, "--- NEWCOMER:\n");
    // for (int i = 0; i < this->newcomerSize; ++i) {
    //     fprintf(stdout, "(Specie %i): ", this->candidates[idx]->speciesId);
    //     node_print(this->candidates[idx], stdout);
    //     fprintf(stdout, " = %f\n", this->results[idx]);
    //     ++idx;
    // }
}

void            population_speciatedPrint(t_population *this) {
    fprintf(stdout, "===POPULATION BEGIN===\n");
    for (int i = 0; i < this->nbSpecies; ++i) {
        t_species const *sp = &this->species[i];
        fprintf(stdout, "Species %i [%i to %i], avg = %f\n  |-> best: ", sp->speciesId, sp->startIdx, sp->endIdx, sp->avgFitness);
        node_print(this->candidates[sp->startIdx], stdout);
        fprintf(stdout, " = %f\n", this->results[sp->startIdx]);
    }
    fprintf(stdout, "===POPULATION END===\n");
}

void population_contest(t_population *this, float const **featureSets, int nbSets, int nbFeatures) {
    for (int i = 0; i < this->size; ++i) {
        this->results[i] = 0.0;
        for (int j = 0; j < nbSets; ++j) {
            this->results[i] += logLoss(featureSets[j][nbFeatures], node_getValue(this->candidates[i], featureSets[j]));
        }
        this->results[i] = -this->results[i] / (float) nbSets;
        if (isnan(this->results[i]))
            this->results[i] = 99999.0;
    }
}

void population_partialContest(t_population *this, float const **featureSets, int nbSets, int nbFeatures, int start, int end) {
    for (int i = start; i < end; ++i) {
        this->results[i] = 0.0;
        for (int j = 0; j < nbSets; ++j) {
            this->results[i] += logLoss(featureSets[j][nbFeatures], node_getValue(this->candidates[i], featureSets[j]));
        }
        this->results[i] = -this->results[i] / (float) nbSets;
        if (isnan(this->results[i]) || fabsf(this->results[i]) > 1)
            this->results[i] = 1;
    }
}

static void *population_threadWrapper(void *data) {
    t_thread_contest *contestData = (t_thread_contest *) data;
    population_partialContest(contestData->this, contestData->featureSets, contestData->nbSets, contestData->nbFeatures, contestData->start, contestData->end);
    return 0;
}

void population_threadedContest(t_population *this, float const **featureSets, int nbSets, int nbFeatures, int nbThreads) {
    pthread_t thread[nbThreads];
    t_thread_contest **partials = malloc(sizeof(t_thread_contest *) * nbThreads);
    int partialSet = this->size / nbThreads;
    for (int i = 0; i < nbThreads; ++i) {
        partials[i] = malloc(sizeof(t_thread_contest));
        partials[i]->this = this;
        partials[i]->featureSets = featureSets;
        partials[i]->nbSets = nbSets;
        partials[i]->nbFeatures = nbFeatures;
        partials[i]->start = i * partialSet;
        partials[i]->end = (i + 1) * partialSet;
        // fprintf(stdout, "thread%i running from %i to %i\n", i, partials[i]->start, partials[i]->end);
        pthread_create(&thread[i], NULL, population_threadWrapper, partials[i]);
    }

    // fprintf(stdout, "main thread running from %i to %i\n", this->size - this->size % nbThreads, this->size);
    population_partialContest(this, featureSets, nbSets, nbFeatures, this->size - this->size % nbThreads, this->size);

    for (int i = 0; i < nbThreads; ++i) {
      pthread_join(thread[i], NULL);
    }

    for (int i = 0; i < nbThreads; ++i) {
        free(partials[i]);
    }
    free(partials);
}

static float      population_compareCandidates(t_population const *this, int i, int j) {
    // // fprintf(stdout, "population_compareCandidates %i and %i\n", i, j);
    // fprintf(stdout, "population_compareCandidates [%i] = %i and [%i] = %i -> %f\n", i, this->candidates[i]->speciesId, j, this->candidates[j]->speciesId, (float)(this->candidates[i]->speciesId) - (float)(this->candidates[j]->speciesId));
    // return (float)(this->candidates[i]->speciesId) - (float)(this->candidates[j]->speciesId);
    if (this->candidates[i]->speciesId != this->candidates[j]->speciesId) {
        return (float)(this->candidates[i]->speciesId) - (float)(this->candidates[j]->speciesId);
    }
    return this->results[i] - this->results[j];
}

static int     population_orderByScorePartition(t_population *this, int leftIdx, int rightIdx, float(*cmp_func)(t_population const *, int, int)) {
    int i = leftIdx;
    int j = rightIdx + 1;
    float tmp;
    t_node *tmpNode;
    while(1) {
        do {
            ++i;
            // fprintf(stdout, "i = %i\n", i);
        } while(cmp_func(this, i, leftIdx) <= 0.f && i < rightIdx); // this->results[i] <= pivot
        do {
            --j;
            // fprintf(stdout, "j = %i\n", j);
        } while(cmp_func(this, j, leftIdx) > 0.f); // this->results[j] > pivot
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
        pivot = population_orderByScorePartition(this, leftIdx, rightIdx, &population_compareCandidates);
        // population_print(this);
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
        // if (j == 0 || node_cmp(this->swapBuffer[idx - 1], this->candidates[j]) > 3) {
            this->swapBuffer[idx] = node_duplicate(this->candidates[j]);
            ++idx;
            lastResult = this->results[j];
        // }
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

// void            population_speciatedIncrement(t_population *this, int nbFeatures) {
//     // pick the elite
//     int idx = 0;
//     int j = 0;
//     float lastResult = -1.0;
//     while (idx < this->eliteSize && j < this->size) {
//         // if (j == 0 || node_cmp(this->swapBuffer[idx - 1], this->candidates[j]) > 3) {
//             this->swapBuffer[idx] = node_duplicate(this->candidates[j]);
//             ++idx;
//             lastResult = this->results[j];
//         // }
//         ++j;
//     }
//     // printf("blabla %i\n", idx);
//     while (idx < this->eliteSize) {
//         this->swapBuffer[idx] = tree_generate(nbFeatures);
//         ++idx;
//     }

//     // for (int i = 0; i < this->eliteSize; ++i) {
//     //     this->swapBuffer[i] = this->candidates[i];
//     // }

//     // make the elite reproduce with middle ones
//     int crossoverOffset = this->eliteSize;
//     int crossoverParentRange = this->eliteSize + this->crossoverSize;
//     // int crossoverParentRange = this->size;
//     for (int i = 0; i < this->crossoverSize; ++i) {
//         int parent1Idx = rand() % crossoverParentRange;
//         int parent2Idx = rand() % crossoverParentRange;
//         this->swapBuffer[crossoverOffset + i] = tree_crossover(this->candidates[parent1Idx], this->candidates[parent2Idx]);
//     }

//     // add newcomers
//     int newcomerOffset = crossoverOffset + this->crossoverSize;
//     for (int i = 0; i < this->newcomerSize; ++i) {
//         this->swapBuffer[newcomerOffset + i] = tree_generate(nbFeatures);
//     }

//     // swap back into candidates
//     t_node **tmp = this->candidates;
//     this->candidates = this->swapBuffer;
//     this->swapBuffer = tmp;
//     for (int i = 0; i < this->size; ++i) {
//         node_delete(&this->swapBuffer[i]);
//     }
// }

void            population_mutate(t_population *this, unsigned int mutants, int nbFeatures) {
    unsigned int idxRange = this->eliteSize + this->crossoverSize - 1;
    for (int i = 0; i < mutants; ++i) {
        unsigned int mutantIdx = rand() % idxRange + 1; // top element cannot mutate
        tree_mutateOne(this->candidates[mutantIdx], nbFeatures);
    }
}

static void population_assignSpeciesIndividual(t_population *this, int individualIdx) {
    const int speciesThreshold = 3;
    // go through every species
    for (int j = 0; j < this->nbSpecies; ++j) {
        int diff = node_cmp(this->swapBuffer[j], this->candidates[individualIdx]);
        if (diff <= speciesThreshold) {
            // existing species
            this->candidates[individualIdx]->speciesId = this->swapBuffer[j]->speciesId;
            return ;
        }
    }
    this->candidates[individualIdx]->speciesId = this->nbSpecies;
    this->swapBuffer[this->nbSpecies] = this->candidates[individualIdx];
    this->nbSpecies += 1;
}

void population_assignSpecies(t_population *this) {
    this->nbSpecies = 1;
    this->candidates[0]->speciesId = 0;
    this->swapBuffer[0] = this->candidates[0];

    // go through every candidates
    for (int i = 1; i < this->size; ++i) {
        population_assignSpeciesIndividual(this, i);
    };
}

float   population_computeSpecies(t_population *this) {
    float avgSum = 0.f
    int speciesId = -1;
    for (int i = 0; i < this->size; ++i) {
        if (this->candidates[i]->speciesId != speciesId) {
            // new species detected
            if (speciesId >= 0) {
                this->species[speciesId].endIdx = i;
                this->species[speciesId].avgFitness = this->species[speciesId].avgFitness / (this->species[speciesId].endIdx - this->species[speciesId].startIdx);
                avgSum += this->species[speciesId].avgFitness;
            }
            ++speciesId;
            this->species[speciesId].speciesId = speciesId;
            this->species[speciesId].startIdx = i;
            this->species[speciesId].avgFitness = this->results[i];
        }
        else {
            this->species[speciesId].avgFitness += this->results[i];
        }
    }
    this->species[speciesId].endIdx = this->size;
    this->species[speciesId].avgFitness = this->species[speciesId].avgFitness / (this->species[speciesId].endIdx - this->species[speciesId].startIdx);
    avgSum += this->species[speciesId].avgFitness;
    return avgSum;
}
