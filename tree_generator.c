#include <stdlib.h>
#include "tree_generator.h"
#include "edge.h"

static void generateEdge(t_edge *edge, int nbFeatures, unsigned int height);
static t_node *generateNode(int nbFeatures, unsigned int height);

static void generateEdge(t_edge *edge, int nbFeatures, unsigned int height) {
    // chance to have a sub node = height / MAX_TREE_DEEPNESS
    if (height > 0 && rand() % 100 < height * 100 / MAX_TREE_DEEPNESS) {
        // create sub node
        edge->type = EDGE_NODE;
        edge->target.node = generateNode(nbFeatures, height - 1);
    }
    else if (rand() % 100 < CONSTANT_CHANCE_PERCENTAGE) {
        // create constant
        edge->type = EDGE_CONSTANT;
        edge->target.constant = ((float)rand()/(float)(RAND_MAX)) * MAX_CONSTANT_RANGE - MAX_CONSTANT_RANGE / 2;
    }
    else {
        // pick feature
        edge->type = EDGE_FEATURE;
        edge->target.featureIdx = rand() % nbFeatures;
    }
}

static t_node *generateNode(int nbFeatures, unsigned int height) {
    t_node *node = node_create();
    node->op = op_pickRandom();
    generateEdge(&node->left, nbFeatures, height);
    generateEdge(&node->right, nbFeatures, height);

    return node;
}

t_node *generateTree(int nbFeatures) {
    unsigned int height = rand() % MAX_TREE_DEEPNESS + 1;
    return generateNode(nbFeatures, height);
}
