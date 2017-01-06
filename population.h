#ifndef __POPULATION_H__
# define __POPULATION_H__

# include "node.h"
# include "tree_generator.h"

# define POPULATION_SIZE 100
# define POPULATION_ELITE_SIZE 20
# define POPULATION_CROSSOVER_SIZE 60
# define POPULATION_NEWCOMER_SIZE 20

typedef struct s_population {
    unsigned int    size;
    unsigned int    eliteSize;
    unsigned int    crossoverSize;
    unsigned int    newcomerSize;
    t_node          **candidates;
    t_node          **swapBuffer;
    float           *results;
} t_population;

t_population    *population_create(unsigned int elite, unsigned int crossover, unsigned int newcomer, int nbFeatures);
void            population_destroy(t_population *this);

void            population_print(t_population *this);

void            population_contest(t_population *this, float const **featureSets, int nbSets, int nbFeatures, float(*scoreFunc)(float, float));
void            population_orderByScore(t_population *this);
void            population_increment(t_population *this, int nbFeatures);
void            population_mutate(t_population *this, unsigned int mutants, int nbFeatures);

#endif //__POPULATION_H__