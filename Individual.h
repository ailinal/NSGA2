#pragma once
#include <problems/Problem.h>
#include "pch.h"

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

using namespace std;

class Individual {
 public:
  using Comparator = bool (Individual::*)(Individual, Individual);

  vector<float> genes;
  vector<float> objectiveSet;
  vector<Individual *> dominatedSet;
  int dominatedCount;
  int rank;
  double distance;
  Problem *problem_;

  Individual();
  Individual(int, Problem *);
  void evaluation();
  bool dominate(Individual);
};

#endif // !INDIVIDUAL_H
