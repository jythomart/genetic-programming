#ifndef __ARG_HELPER_H__
# define __ARG_HELPER_H__

typedef struct s_config {
  char *datasetFile;
  int nbSamples;
  int nbFeatures;
  int nbDiscard;
  int nbElites;
  int nbCrossover;
  int nbNewcomer;
  int nbMutate;
  int nbThreads; // not including main thread
} t_config;

int   arg_parse(t_config *config, int argc, char **argv);
void  arg_printUsage(int argc, char **argv);
void  arg_printHelp(int argc, char **argv);

#endif //__ARG_HELPER_H__
