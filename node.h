#ifndef __NODE_H__
# define __NODE_H__

# include <stdio.h>
# include "operation.h"
# include "edge.h"

# define FALSE 0
# define TRUE !FALSE
typedef char t_bool;

typedef struct s_node {
    float result;
    t_op_func op;
    t_edge left;
    t_edge right;
} t_node;

t_node  *node_create();
void    node_delete(t_node **this);
t_node  *node_duplicate(t_node const *this);

t_node  *node_pickRandom(t_node *this);
float   node_getValue(t_node *this, float const *features);
void    node_toJSON(t_node const *this, FILE *buffer);
void    node_print(t_node const *this, FILE *buffer);

int     node_cmp(t_node const *this, t_node const *other);

#endif //__NODE_H__