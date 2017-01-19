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

  int nbSamples = 10000;
  int nbFeatures = 50;

  int nbElites = 10;
  int nbCrossover = 490;
  int nbNewcomer = 500;
  int nbMutate = 50;

  struct timeval stop, start;

  // float const **featuresPtr = (float const **)test_sphereVolume(100);
  // float const **featuresPtr = (float const **)test_cubicXYZ(100);

  gettimeofday(&start, NULL);
  FILE *datasetFile = fopen("./datasets.csv", "r");
  float const **featuresPtr = (float const **)feature_fromFile(datasetFile, nbSamples, nbFeatures);
  gettimeofday(&stop, NULL);
  printf("took %lu to parse features\n", stop.tv_usec - start.tv_usec);

  gettimeofday(&start, NULL);
  t_population *pop = population_create(nbElites, nbCrossover, nbNewcomer, nbFeatures);
  pop->results[0] = 1;
  gettimeofday(&stop, NULL);
  printf("took %lu to create population\n", stop.tv_usec - start.tv_usec);

  int generation = 0;
  
  while (pop->results[0] > 0.0001) {
    gettimeofday(&start, NULL);
    population_contest(pop, featuresPtr, nbSamples, nbFeatures, &logLoss);
    gettimeofday(&stop, NULL);
    printf("took %lu to contest\n", stop.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    population_orderByScore(pop);
    gettimeofday(&stop, NULL);
    printf("took %lu to order by score\n", stop.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    population_increment(pop, nbFeatures);
    gettimeofday(&stop, NULL);
    printf("took %lu to increment population\n", stop.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    population_mutate(pop, nbMutate, nbFeatures);
    gettimeofday(&stop, NULL);
    printf("took %lu to mutate population\n----------------\n", stop.tv_usec - start.tv_usec);
    
    if (generation % 100 == 0) {
      fprintf(stdout, "------------------------------------------------------------------------------------------------------\n");
      fprintf(stdout, "result found after %i generations\n", generation);
      population_print(pop);
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