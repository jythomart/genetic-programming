#ifndef __TREE_GENERATOR_H__
# define __TREE_GENERATOR_H__

# include "node.h"

# define MAX_TREE_HEIGHT 5
# define CONSTANT_CHANCE_PERCENTAGE 5
# define CONSTANT_RANGE 5.0

t_node *tree_generate(int nbFeatures);
t_node *tree_crossover(t_node *first, t_node *second);  // create new one, does not mutate

#endif //__TREE_GENERATOR_H__