#include <stdio.h>
#include <stdlib.h>

#include "operation.h"

// merge in one array ?
static char const *opNames[NUMBER_OF_OPERATION + 1] = {
    "add",
    "sub",
    "mul",
    "div",
    "UNKNOWN"
};
static char const opSym[NUMBER_OF_OPERATION + 1] = "+-*/0";
static t_op_func opArray[NUMBER_OF_OPERATION] = {
    &op_add,
    &op_sub,
    &op_mul,
    &op_div
};

char const *op_getName(t_op_func func) {
    int i = 0;
    while (opArray[i] != func && i < NUMBER_OF_OPERATION) {
        ++i;
    }
    return opNames[i]; // if overflow return UNKNOWN
}

char op_getSym(t_op_func func) {
    int i = 0;
    while (opArray[i] != func && i < NUMBER_OF_OPERATION) {
        ++i;
    }
    return opSym[i]; // if overflow return UNKNOWN
}

t_op_func op_pickRandom() {
    return opArray[rand() % NUMBER_OF_OPERATION];
}

float op_add(float left, float right) {
    return left + right;
}

float op_sub(float left, float right) {
    return left - right;
}

float op_mul(float left, float right) {
    return left * right;
}

float op_div(float left, float right) {
    // if (right >= -DIV_BY_O_THRESHOLD && right <= DIV_BY_O_THRESHOLD)
    //     return 1.0;
    return left / right;
}

float op_mod(float left, float right) {
    if (right >= DIV_BY_O_THRESHOLD && right <= DIV_BY_O_THRESHOLD)
        return 0.0;
    return ((int) left) % ((int) right);
}
