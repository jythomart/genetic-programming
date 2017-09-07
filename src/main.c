#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "../include/test.h"
#include "../include/node.h"
#include "../include/operation.h"
#include "../include/tree_generator.h"
#include "../include/population.h"
#include "../include/feature_parser.h"
#include "../include/arg_helper.h"

typedef unsigned long long timestamp_t;
static timestamp_t getTimestamp() {
  struct timeval now;
  gettimeofday (&now, NULL);

  return now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}


void compute() {
  int nbThreads = 15; // not including main thread

  int nbSamples = 108405;
  int nbDiscard = 3;
  int nbFeatures = 21;

  int nbElites = 6;
  int nbCrossover = 94;
  int nbNewcomer = 100;
  int nbMutate = 10;

  // float const **featuresPtr = (float const **)test_sphereVolume(100);
  // float const **featuresPtr = (float const **)test_cubicXYZ(100);

  FILE *datasetFile = fopen("./datasets.csv", "r");
  float const **featuresPtr = (float const **)feature_fromFile(datasetFile, nbDiscard, nbSamples, nbFeatures);

  t_population *pop = population_create(nbElites, nbCrossover, nbNewcomer, nbFeatures);
  pop->results[0] = 1;

  int generation = 0;

  timestamp_t start = getTimestamp();
  timestamp_t end = getTimestamp();
  
  while (pop->results[0] > 0.0001) {
    population_threadedContest(pop, featuresPtr, nbSamples, nbFeatures, nbThreads);

    population_orderByScore(pop);

    population_increment(pop, nbFeatures);

    population_mutate(pop, nbMutate, nbFeatures);
    
    if (generation % 1 == 0) {
      end = getTimestamp();
      double seconds = (float)(end - start) / 1000000.0L;
      fprintf(stdout, "------------------------------------------------------------------------------------------------------\n");
      fprintf(stdout, "contest took %.5f s\n", seconds);
      fprintf(stdout, "result found after %i generations\n", generation);
      population_print(pop);
      start = getTimestamp();
    }
    ++generation;
  }

  fprintf(stdout, "------------------------------------------------------------------------------------------------------\n");
  fprintf(stdout, "FINAL RESULT !!!\n");
  fprintf(stdout, "------------------------------------------------------------------------------------------------------\n");
  fprintf(stdout, "result found after %i generations\n", generation);
  population_print(pop);

  fclose(datasetFile);
}

int main(int argc, char **argv) {
  srand(time(NULL));

  t_config config;
  if (arg_parse(&config, argc, argv) < 0)
    return 0; // error while parsing args

  compute();
  return 0;
}