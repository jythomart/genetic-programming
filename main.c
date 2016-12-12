#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "node.h"
#include "operation.h"
#include "tree_generator.h"

void test(FILE *logFile) {
  float features[21] = {
    0.919013169504469,
    0.175979075128653,
    0.10135850159251,
    0.666597541086836,
    0.580599131401996,
    0.921770864359415,
    0.38959978404349,
    0.48172527594393,
    0.0249343165510615,
    0.90927849667423,
    0.743939251587636,
    0.0481614143534195,
    0.604953709134513,
    0.25949453653137,
    0.761187626490504,
    0.92258608236257,
    0.615988823047026,
    0.4061376325687,
    0.372771556780623,
    0.112987600494221,
    0.65669198265475
  };

  t_node *root = generateTree(21);
  node_toJSON(root, logFile);
  printf("Test result = %f\n", node_getValue(root, features));
}

int main(int argc, char **argv) {
  srand(time(NULL));
  int i;
  FILE *logFile = fopen("./debug.json", "w");
  for (i = 0; i < 100; ++i) {
    test(logFile);
    fprintf(logFile, "\n");
  }
  fclose(logFile);
  return 0;
}