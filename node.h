#ifndef __NODE_H__
# define __NODE_H__

# include <stdio.h>
# include "operation.h"

# define FALSE 0
# define TRUE !FALSE
typedef char t_bool;

struct s_node;

union u_edge {
    unsigned int featureIdx;
    struct s_node *node;
    float constant;
} t_edge;

typedef struct s_node {
    t_bool leaf;
    float result;
    unsigned int leftIdx;
    unsigned int rightIdx;
    t_op_func op;
    struct s_node *left;
    struct s_node *right;
} t_node;

t_node *createNode(t_op_func op, t_node *left, t_node *right);
t_node *createLeaf(t_op_func op, unsigned int leftIdx, unsigned int rightIdx);

float applyDFS(float const *features, t_node *node);
float applyDFSDebug(float const *features, t_node *node);
// float customDFS(t_node *node, void (*customFunc)(t_node *));

void debugToJSON(FILE *buffer, t_node *node);

#endif //__NODE_H__