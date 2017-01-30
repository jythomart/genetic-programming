#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "test.h"
#include "node.h"
#include "operation.h"
#include "tree_generator.h"
#include "population.h"
#include "feature_parser.h"

typedef unsigned long long timestamp_t;
static timestamp_t get_timestamp() {
  struct timeval now;
  gettimeofday (&now, NULL);

  return now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

void test(FILE *logFile) {
  int nbThreads = 31; // not including main thread

  int nbSamples = 100000;
  int nbFeatures = 50;

  int nbElites = 10;
  int nbCrossover = 490;
  int nbNewcomer = 500;
  int nbMutate = 50;

  // float const **featuresPtr = (float const **)test_sphereVolume(100);
  // float const **featuresPtr = (float const **)test_cubicXYZ(100);

  FILE *datasetFile = fopen("./datasets.csv", "r");
  float const **featuresPtr = (float const **)feature_fromFile(datasetFile, nbSamples, nbFeatures);

  t_population *pop = population_create(nbElites, nbCrossover, nbNewcomer, nbFeatures);
  pop->results[0] = 1;

  int generation = 0;

  timestamp_t start = get_timestamp();
  timestamp_t end = get_timestamp();
  
  while (pop->results[0] > 0.0001) {
    // clock_t start = clock();
    population_threadedContest(pop, featuresPtr, nbSamples, nbFeatures, nbThreads);
    // population_contest(pop, featuresPtr, nbSamples, nbFeatures);
    // clock_t end = clock();
    // float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    // fprintf(stdout, "contest took %fs\n", seconds);

    population_orderByScore(pop);

    population_increment(pop, nbFeatures);

    population_mutate(pop, nbMutate, nbFeatures);
    
    if (generation % 1 == 0) {
      end = get_timestamp();
      double seconds = (float)(end - start) / 1000000.0L;
      fprintf(stdout, "------------------------------------------------------------------------------------------------------\n");
      fprintf(stdout, "contest took %.5f s\n", seconds);
      fprintf(stdout, "result found after %i generations\n", generation);
      population_print(pop);
      start = get_timestamp();
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