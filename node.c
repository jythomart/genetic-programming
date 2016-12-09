#include <stdlib.h>
#include "node.h"

t_node *createNode(t_op_func op, t_node *left, t_node *right) {
    t_node *node = malloc(sizeof(t_node));

    node->leaf = 0;
    node->op = op;
    node->left = left;
    node->right = right;

    return node;
}

t_node *createLeaf(t_op_func op, unsigned int leftIdx, unsigned int rightIdx) {
    t_node *node = malloc(sizeof(t_node));

    node->leaf = 1;
    node->op = op;
    node->leftIdx = leftIdx;
    node->rightIdx = rightIdx;

    return node;
}

float applyDFS(float const *features, t_node *node) {
    if (node->leaf == TRUE) {
        // on a leaf, end condition compute
        node->result = (*node->op)(features[node->leftIdx], features[node->rightIdx]);
        return node->result;
    }
    else {
        node->result = (*node->op)(applyDFS(features, node->left), applyDFS(features, node->right));
        return node->result;
    }
}

float applyDFSDebug(float const *features, t_node *node) {
    if (node->leaf == TRUE) {
        // on a leaf, end condition compute
        node->result = (*node->op)(features[node->leftIdx], features[node->rightIdx]);
        printf("leaf: %s(%f, %f) = %f\n", getOpName(node->op), features[node->leftIdx], features[node->rightIdx], node->result);
        return node->result;
    }
    else {
        float left = applyDFSDebug(features, node->left);
        float right = applyDFSDebug(features, node->right);
        node->result = (*node->op)(left, right);
        printf("node: %s(%f, %f) = %f\n", getOpName(node->op), left, right, node->result);
        return node->result;
    }
}

// float customDFS(t_node *node, void (*customFunc)(t_node *));
void debugToJSON(FILE *buffer, t_node *node) {
    // on a leaf, end condition compute
    if (node->leaf == TRUE) {
        fprintf(buffer, "{\n\"op\": \"%s\",\n\"leftIdx\": %i,\n\"rightIdx\": %i\n}", getOpName(node->op), node->leftIdx, node->rightIdx);
        return ;
    }
    else {
        fprintf(buffer, "{\n\"op\": \"%s\",\n\"left\": ", getOpName(node->op));
        debugToJSON(buffer, node->left);
        fprintf(buffer, ",\n\"right\": ");
        debugToJSON(buffer, node->right);
        fprintf(buffer, "}\n");
    }
}
