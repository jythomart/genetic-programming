#include <stdlib.h>
#include <string.h>
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
    if ((*this)->left.type == EDGE_NODE) {
        node_delete(&(*this)->left.target.node);
    }
    if ((*this)->right.type == EDGE_NODE) {
        node_delete(&(*this)->right.target.node);
    }
    free(*this);
    *this = 0;
}

t_node  *node_duplicate(t_node const *this) {
    t_node *copy = malloc(sizeof(t_node));
    memcpy(copy, this, sizeof(t_node));
    if (this->left.type == EDGE_NODE) {
        copy->left.target.node = node_duplicate(this->left.target.node);
    }
    if (this->right.type == EDGE_NODE) {
        copy->right.target.node = node_duplicate(this->right.target.node);
    }
    return copy;
}

static void  node_pickRandomRecursive(t_node *this, t_node **picked, int index) {
    // first pick or not
    if (rand() % index == 0) {
        *picked = this;
    }

    if (this->left.type == EDGE_NODE) {
        ++index;
        node_pickRandomRecursive(this->left.target.node, picked, index);
    }
    if (this->right.type == EDGE_NODE) {
        ++index;
        node_pickRandomRecursive(this->right.target.node, picked, index);
    }
}

t_node  *node_pickRandom(t_node *this) {
    t_node **picked = &this;
    node_pickRandomRecursive(this, picked, 2);
    return (*picked);
}

float   node_getValue(t_node *this, float const *features) {
    float result = (*this->op)(edge_getValue(&this->left, features), edge_getValue(&this->right, features));
    this->result = result;
    return result;
}

void    node_toJSON(t_node const *this, FILE *buffer) {
    fprintf(buffer, "(");
    edge_toJSON(&this->left, buffer);
    fprintf(buffer, " %c ", op_getSym(this->op));
    edge_toJSON(&this->right, buffer);
    fprintf(buffer, ")");
}
