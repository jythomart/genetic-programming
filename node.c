#include <stdlib.h>
#include "node.h"

t_node  *node_create() {
    t_node *this = malloc(sizeof(t_node));

    this->op = &op_add;
    this->left.type = EDGE_FEATURE;
    this->left.target.featureIdx = 0;
    this->right.type = EDGE_FEATURE;
    this->right.target.featureIdx = 0;

    return this;
}

void    node_delete(t_node **this) {
    // TODO
    // WE SHOULD ALWAYS CLEAN OUR MESS
}

float   node_getValue(t_node *this, float const *features) {
    this->result = (*this->op)(edge_getValue(&this->left, features), edge_getValue(&this->right, features));
    return this->result;
}

void    node_toJSON(t_node const *this, FILE *buffer) {
    // TODO
    fprintf(buffer, "(");
    edge_toJSON(&this->left, buffer);
    fprintf(buffer, " %c ", op_getSym(this->op));
    edge_toJSON(&this->right, buffer);
    fprintf(buffer, ")");
}
