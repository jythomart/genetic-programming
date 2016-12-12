#include <stdlib.h>
#include "edge.h"
#include "node.h"

float edge_getValue(t_edge const *this, float const *features) {
    if (this->type == EDGE_FEATURE) {
        return features[this->target.featureIdx];
    }
    else if (this->type == EDGE_NODE) {
        return node_getValue(this->target.node, features);
    }
    else { // EDGE_CONSTANT
        return this->target.constant;
    }
}

void    edge_toJSON(t_edge const *this, FILE *buffer) {
    if (this->type == EDGE_FEATURE) {
       fprintf(buffer, "_%i", this->target.featureIdx);
    }
    else if (this->type == EDGE_NODE) {
        return node_toJSON(this->target.node, buffer);
    }
    else { // EDGE_CONSTANT
        fprintf(buffer, "%f", this->target.constant);
    }
}