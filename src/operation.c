#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/operation.h"

// merge in one array ?
static char const *opNames[NUMBER_OF_OPERATION + 1] = {
    "add",
    "sub",
    "mul",
    "div",
    // "cos",
    // "sin",
    "min",
    "max",
    "UNKNOWN"
};
// static char const opSym[NUMBER_OF_OPERATION + 1] = "+-*/csmM0";
static char const opSym[NUMBER_OF_OPERATION + 1] = "+-*/mM0";
static t_op_func opArray[NUMBER_OF_OPERATION] = {
    &op_add,
    &op_sub,
    &op_mul,
    &op_div,
    // &op_cos,
    // &op_sin,
    &op_min,
    &op_max
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

float op_cos(float left, float right) {
    return left * cosf(right);
}

float op_sin(float left, float right) {
    return left * sinf(right);
}

float op_min(float left, float right) {
    return left < right ? left : right;
}

float op_max(float left, float right) {
    return left > right ? left : right;
}