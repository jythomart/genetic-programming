#ifndef __OPERATION_H__
# define __OPERATION_H__

# define DIV_BY_O_THRESHOLD 0.001
// # define NUMBER_OF_OPERATION 8
# define NUMBER_OF_OPERATION 6

typedef float(*t_op_func)(float, float);

char const  *op_getName(t_op_func func);
char        op_getSym(t_op_func func);
t_op_func   op_pickRandom();

float op_add(float left, float right);
float op_sub(float left, float right);
float op_mul(float left, float right);
float op_div(float left, float right);
float op_mod(float left, float right);
// float op_mod(int left, int right);

// pseudo unary: ie left * op(right)
float op_cos(float left, float right);
float op_sin(float left, float right);

float op_min(float left, float right);
float op_max(float left, float right);

#endif // __OPERATION_H__