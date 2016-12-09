#ifndef __OPERATION_H__
# define __OPERATION_H__

# define DIV_BY_O_THRESHOLD 0.001
# define NUMBER_OF_OPERATION 4

typedef float(*t_op_func)(float, float);

char const *getOpName(t_op_func func);
t_op_func getRandomOp();

float op_add(float left, float right);
float op_sub(float left, float right);
float op_mul(float left, float right);
float op_div(float left, float right);
float op_mod(float left, float right);
// float op_mod(int left, int right);

#endif // __OPERATION_H__