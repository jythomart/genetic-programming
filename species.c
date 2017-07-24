#include "species.h"

t_speciesList    *speciesList_create(unsigned int nbCandidates) {
    t_speciesList *this = malloc(sizeof(t_speciesList));
    this->nbCandidates = nbCandidates;
    this->nbSpecies = 0;
    this->species = malloc(nbCandidates * sizeof(t_species));
    return this;
}

// TODO: NOT FINISHED YET, CONTIONUE HERE
static void population_assignSpeciesIndividual(t_speciesList *this, t_population *pop, int individualIdx) {
    // go through every species
    for (int j = 0; j < this->nbSpecies; ++j) {
        int diff = node_cmp(this->swapBuffer[j], this->candidates[individualIdx]);
        if (diff <= SPECIES_THRESHOLD) {
            // existing species
            this->candidates[individualIdx]->speciesId = this->swapBuffer[j]->speciesId;
            return ;
        }
    }
    this->candidates[individualIdx]->speciesId = this->nbSpecies;
    this->swapBuffer[this->nbSpecies] = this->candidates[individualIdx];
    this->nbSpecies += 1;
}

void             speciesList_assign(t_speciesList *this, t_population *population) {
    this->nbSpecies = 1;
    this->species[0]->speciesId = 0;

    // go through every candidates
    for (int i = 1; i < population->size; ++i) {
        population_assignSpeciesIndividual(this, population, i);
    };
}

float            speciesList_compute(t_speciesList *this, t_population *population) {

}
