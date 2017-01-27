#ifndef __EDGE_H__
# define __EDGE_H__

# include <stdio.h>

struct s_node;

# define EDGE_FEATURE   0
# define EDGE_NODE      1
# define EDGE_CONSTANT  2
typedef char t_edge_type;

typedef union u_edge_target {
    unsigned int featureIdx;
    struct s_node *node;
    float constant;
} t_edge_target;

typedef struct s_edge {
    t_edge_type type;
    t_edge_target target;
} t_edge;

float   edge_getValue(t_edge const *this, float const *features);
void    edge_toJSON(t_edge const *this, FILE *buffer);
void    edge_print(t_edge const *this, FILE *buffer);

int     edge_cmp(t_edge const *this, t_edge const *other);

#endif //__EDGE_H__
