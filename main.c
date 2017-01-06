#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include "test.h"
#include "node.h"
#include "operation.h"
#include "tree_generator.h"
#include "population.h"
#include "feature_parser.h"

float computeScore(float expected, float actual) {
  return fabsf(expected - actual);
}

void test(FILE *logFile) {
  // float features[22] = {
  //   0.919013169504469,
  //   0.175979075128653,
  //   0.10135850159251,
  //   0.666597541086836,
  //   0.580599131401996,
  //   0.921770864359415,
  //   0.38959978404349,
  //   0.48172527594393,
  //   0.0249343165510615,
  //   0.90927849667423,
  //   0.743939251587636,
  //   0.0481614143534195,
  //   0.604953709134513,
  //   0.25949453653137,
  //   0.761187626490504,
  //   0.92258608236257,
  //   0.615988823047026,
  //   0.4061376325687,
  //   0.372771556780623,
  //   0.112987600494221,
  //   0.65669198265475,
  //   1.0
  // };

  // float const *featuresPtr[4] = {
  //   features,
  //   features,
  //   features,
  //   features
  // };

  // float const **featuresPtr = (float const **)test_sphereVolume(100);
  // float const **featuresPtr = (float const **)test_cubicXYZ(100);
  FILE *datasetFile = fopen("./datasets.csv", "r");
  float const **featuresPtr = (float const **)feature_fromFile(datasetFile, 100, 21);

  t_population *pop = population_create(10, 690, 300, 21);
  pop->results[0] = 1;
  // population_contest(pop, featuresPtr, 100, 4, &computeScore);
  // population_orderByScore(pop);

  int generation = 0;
  
  while (pop->results[0] > 0.00001) {
    population_contest(pop, featuresPtr, 100, 21, &computeScore);
    population_orderByScore(pop);
    population_increment(pop, 21);
    population_mutate(pop, 10, 21);
    ++generation;
    if (generation % 100 == 0) {
      fprintf(stdout, "------------------------------------------------------------------------------------------------------\n");
      fprintf(stdout, "result found after %i generations\n", generation);
      population_print(pop);
    }
  }

  fprintf(stdout, "------------------------------------------------------------------------------------------------------\n");
  fprintf(stdout, "FINAL RESULT !!!\n");
  fprintf(stdout, "------------------------------------------------------------------------------------------------------\n");
  fprintf(stdout, "result found after %i generations\n", generation);
  population_print(pop);

  fclose(datasetFile);
  

  // t_node *root = tree_generate(21);
  // t_node *copy = tree_generate(21);
  // node_toJSON(root, stdout);
  // fprintf(stdout, "\n");
  // node_toJSON(copy, stdout);
  // fprintf(stdout, "\n");
  // t_node *child = tree_crossover(root, copy);
  // fprintf(stdout, "\n");
  // node_toJSON(child, stdout);
  // fprintf(stdout, "\n");
  // node_toJSON(root, logFile);
  // node_getValue(root, features);
  // // printf("Test result = %f\n", node_getValue(root, features));
  // node_delete(&root);
  // node_delete(&copy);
}

int main(int argc, char **argv) {
  srand(time(NULL));
  // srand(23);
  FILE *logFile = fopen("./debug.json", "w");
  // for (i = 0; i < 100; ++i) {
    test(logFile);
    // fprintf(logFile, "\n");
  // }
  fclose(logFile);
  return 0;
}