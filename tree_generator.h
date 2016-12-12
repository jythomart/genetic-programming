#ifndef __TREE_GENERATOR_H__
# define __TREE_GENERATOR_H__

# include "node.h"

# define MAX_TREE_DEEPNESS 5
# define CONSTANT_CHANCE_PERCENTAGE 5
# define MAX_CONSTANT_RANGE 5.0

t_node *generateTree(int nbFeatures);

#endif //__TREE_GENERATOR_H__