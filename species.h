#ifndef __SPECIES_H__
# define __SPECIES_H__

# include "node.h"
# include "tree_generator.h"
# include "population.h"

# define SPECIES_THRESHOLD 3

typedef struct s_species {
    unsigned int    speciesId;
    unsigned int    startIdx;
    unsigned int    endIdx;
    float           avgFitness;
} t_species;

typedef struct s_speciesList {
    unsigned int    nbCandidates;
    unsigned int    nbSpecies;
    t_species       *species;
} t_speciesList;

t_speciesList    *speciesList_create(unsigned int nbCandidates);

void             speciesList_assign(t_speciesList *this, t_population *population);
float            speciesList_compute(t_speciesList *this, t_population *population);

#endif //__SPECIES_H__