#include <problems/Problem.h>
#include "pch.h"
#include "Individual.h"

#pragma once

#ifndef POPULATION_H
#define POPULATION_H

using namespace std;

class Population {
 public:
  vector<Individual> individualSet;
  int populationSize;
  int geneSize;
  float crossover_prob;
  float mutation_prob;
  Problem* problem_;

  Population(int _popSize,
             float probCrossover,
             float probMutation,
             Problem *problem);
  void initialize();
  void clear();
  Population copy();
  Population copy_all();
  Population combination(Population);

  void evaluation();
};

#endif // !POPULATION_H