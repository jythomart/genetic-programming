#include <stdlib.h>
#include <string.h>
#include "tree_generator.h"
#include "edge.h"

static void tree_generateEdge(t_edge *edge, int nbFeatures, unsigned int height);
static t_node *tree_generateNode(int nbFeatures, unsigned int height);

static void tree_generateEdge(t_edge *edge, int nbFeatures, unsigned int height) {
    // chance to have a sub node = height / MAX_TREE_HEIGHT
    if (height > 0 && rand() % 100 < height * 100 / MAX_TREE_HEIGHT) {
        // create sub node
        edge->type = EDGE_NODE;
        edge->target.node = tree_generateNode(nbFeatures, height - 1);
    }
    else if (rand() % 100 < CONSTANT_CHANCE_PERCENTAGE) {
        // create constant
        edge->type = EDGE_CONSTANT;
        edge->target.constant = ((float)rand()/(float)(RAND_MAX)) * CONSTANT_RANGE - CONSTANT_RANGE / 2;
    }
    else {
        // pick feature
        edge->type = EDGE_FEATURE;
        edge->target.featureIdx = rand() % nbFeatures;
    }
}

static t_node *tree_generateNode(int nbFeatures, unsigned int height) {
    t_node *node = node_create();
    node->op = op_pickRandom();
    tree_generateEdge(&node->left, nbFeatures, height);
    tree_generateEdge(&node->right, nbFeatures, height);

    return node;
}

t_node *tree_generate(int nbFeatures) {
    unsigned int height = rand() % MAX_TREE_HEIGHT + 1;
    return tree_generateNode(nbFeatures, height);
}

t_node *tree_crossover(t_node *first, t_node *second) {
    t_node *child = node_create();
    memcpy(child, first, sizeof(t_node));
    child->right.type = second->right.type;
    child->right.target = second->right.target;
    if (child->left.type == EDGE_NODE) {
        child->left.target.node = node_duplicate(first->left.target.node);
    }
    if (child->right.type == EDGE_NODE) {
        child->right.target.node = node_duplicate(second->right.target.node);
    }
    return child;
}