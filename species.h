#ifndef __SPECIES_H__
# define __SPECIES_H__

# include "node.h"
# include "tree_generator.h"

typedef struct s_species {
    unsigned int    speciesId;
    unsigned int    startIdx;
    unsigned int    endIdx;
} t_species;

#endif //__SPECIES_H__