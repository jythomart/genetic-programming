#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/arg_helper.h"


int arg_parse(t_config *config, int argc, char **argv) {
  if (argc < 4) {
    arg_printUsage(argc, argv);
    if (argc >= 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-help") == 0))
      arg_printHelp(argc, argv);
    return -1;
  }

  config->datasetFile = argv[1];
  config->nbSamples = atoi(argv[2]); // TODO: compute from file
  config->nbFeatures = atoi(argv[3]); // TODO: compute from file
  config->nbDiscard   = argc >= 4 ? atoi(argv[4]) : 1;
  config->nbElites    = argc >= 5 ? atoi(argv[5]) : 3;  //TODO: create config file ?
  config->nbCrossover = argc >= 6 ? atoi(argv[6]) : 77; //TODO: create config file ?
  config->nbNewcomer  = argc >= 7 ? atoi(argv[7]) : 20; //TODO: create config file ?
  config->nbMutate    = argc >= 8 ? atoi(argv[8]) : 10; //TODO: create config file ?
  config->nbThreads   = argc >= 9 ? atoi(argv[9]) : 1;  //TODO: create config file ?

  return 0;
}

void arg_printUsage(int argc, char **argv) {
  (void) argv; // unused

  printf("Usage: %s datasetFile nbSamples nbFeatures [nbDiscard] [nbElites] [nbCrossover] [nbNewcomer] [nbMutate] [nbThreads]\n", argv[0]);
}

void arg_printHelp(int argc, char **argv) {
  (void) argc; // unused
  (void) argv; // unused

  printf("\nThis program is a small project to play around with genetic programming\n\n");
  printf("datasetFile:  .csv dataset file containing all features and results\n");
  printf("nbSamples:    number of dataset entries (number of lines)\n");
  printf("nbFeatures:   number of features in dataset (number columns excluding result and id)\n");
  printf("nbDiscard:    number of lines to discard at the beginning of the file (default 1 to discard header)\n");
  printf("nbElites:     number of top performer individuals to keep for next generation (default 3)\n");
  printf("nbCrossover:  number of individuals to generate by crossover (default 77)\n");
  printf("nbNewcomer:   number of individuals randomly generated (default 20)\n");
  printf("nbMutate:     number of individuals to randomly mutate (default 10)\n");
  printf("nbThreads:    number of threads to use (default 1)\n");
}
