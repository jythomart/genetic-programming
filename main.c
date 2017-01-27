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
#include <sys/time.h>

float computeScore(float actual, float predicted) {
  return fabsf(actual - predicted);
}

float logLoss(float actual, float predicted) {
  return actual * logf(predicted) + (1 - actual) * logf(1 - predicted);
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

  int nbSamples = 1000;
  int nbFeatures = 50;

  int nbElites = 10;
  int nbCrossover = 490;
  int nbNewcomer = 500;
  int nbMutate = 50;

  // struct timeval stop, start;

  // float const **featuresPtr = (float const **)test_sphereVolume(100);
  // float const **featuresPtr = (float const **)test_cubicXYZ(100);

  FILE *datasetFile = fopen("./datasets.csv", "r");
  float const **featuresPtr = (float const **)feature_fromFile(datasetFile, nbSamples, nbFeatures);

  t_population *pop = population_create(nbElites, nbCrossover, nbNewcomer, nbFeatures);
  pop->results[0] = 1;

  int generation = 0;

  clock_t start = clock();
  clock_t end = clock();
  
  while (pop->results[0] > 0.0001) {
    // clock_t start = clock();
    population_contest(pop, featuresPtr, nbSamples, nbFeatures, &logLoss);
    // clock_t end = clock();
    // float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    // fprintf(stdout, "contest took %fs\n", seconds);

    population_orderByScore(pop);

    population_increment(pop, nbFeatures);

    population_mutate(pop, nbMutate, nbFeatures);
    
    if (generation % 100 == 0) {
      end = clock();
      float seconds = (float)(end - start) / CLOCKS_PER_SEC;
      fprintf(stdout, "------------------------------------------------------------------------------------------------------\n");
      fprintf(stdout, "contest took %fs\n", seconds);
      fprintf(stdout, "result found after %i generations\n", generation);
      population_print(pop);
      start = clock();
    }
    ++generation;
  }

  fprintf(stdout, "------------------------------------------------------------------------------------------------------\n");
  fprintf(stdout, "FINAL RESULT !!!\n");
  fprintf(stdout, "------------------------------------------------------------------------------------------------------\n");
  fprintf(stdout, "result found after %i generations\n", generation);
  population_print(pop);

  fclose(datasetFile);
  

  // t_node *root = tree_generate(21);
  // t_node *copy = tree_generate(21);
  // node_toSymbols(root, stdout);
  // fprintf(stdout, "\n");
  // node_toSymbols(copy, stdout);
  // fprintf(stdout, "\n");
  // t_node *child = tree_crossover(root, copy);
  // fprintf(stdout, "\n");
  // node_toSymbols(child, stdout);
  // fprintf(stdout, "\n");
  // node_toSymbols(root, logFile);
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