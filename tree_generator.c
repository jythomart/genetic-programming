#include <stdlib.h>
#include "tree_generator.h"

// static t_node *generateTreeMeshRec(t_node **subLayer, int nbSubLayerNodes) {
//     int i;
//     t_node *layer[nbSubLayerNodes - 1];
//     for (i = 0; i < nbSubLayerNodes - 1; ++i) {
//         layer[i] = createNode(getRandomOp(), subLayer[i], subLayer[i + 1]);
//         layer[i]->id = (nbSubLayerNodes - 2) * 1000 + i;
//     }
//     if (nbSubLayerNodes == 2) {
//         // end of recursion, this is the root node
//         return layer[0];
//     } else {
//         return generateTreeRec(layer, nbSubLayerNodes - 1);
//     }
// }

// t_node *generateTree(float *features, int nbFeatures) {
//     int i;
//     int layerLength = rand() % (nbFeatures / 2 + nbFeatures % 2) + nbFeatures / 2;
//     t_node *layer[layerLength];
//     for (i = 0; i < layerLength; ++i) {
//         unsigned int leftIdx = rand() % nbFeatures;
//         unsigned int rightIdx = rand() % nbFeatures;
//         layer[i] = createLeaf(getRandomOp(), &features[leftIdx], &features[rightIdx]);
//         layer[i]->leftIdx = leftIdx;
//         layer[i]->rightIdx = rightIdx;
//         layer[i]->id = (layerLength - 1) * 1000 + i;
//     }

//     return generateTreeRec(layer, layerLength);
// }

static t_node *generateTreeRec(int nbFeatures, unsigned int height) {
    if (height <= 0) {
        unsigned int leftIdx = rand() % nbFeatures;
        unsigned int rightIdx = rand() % nbFeatures;
        t_node *node = createLeaf(getRandomOp(), leftIdx, rightIdx);
        node->leftIdx = leftIdx;
        node->rightIdx = rightIdx;
        return node;
    }
    else {
        return createNode(getRandomOp(), generateTreeRec(nbFeatures, height - 1), generateTreeRec(nbFeatures, height - 1));
    }
}

t_node *generateTree(int nbFeatures) {
    unsigned int height = rand() % MAX_TREE_DEEPNESS + 1;
    t_node *root = createNode(getRandomOp(), generateTreeRec(nbFeatures, height - 1), generateTreeRec(nbFeatures, height - 1));

    return root;
}
