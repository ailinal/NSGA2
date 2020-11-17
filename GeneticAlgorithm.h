#pragma once
#include "pch.h"
#include "Population.h"
#include "Individual.h"

#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

typedef vector<Individual *> Front;

using namespace std;

class GeneticAlgorithm {
 public:
  void SBX(int, Population &);
  void PLM(int, Population &);
  vector<Front> fastNonDominatedSort(Population &);
  void crowdingDistanceAssignment(Front, Population &);
  void tournament_selection(Population &);
  Population next_new_pop(Population &, int, int);
  void elitism(Population &);

};

#endif // !GENETIC_ALGORITHM_H
