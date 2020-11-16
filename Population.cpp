#include "pch.h"
#include "Population.h"

Population::Population(int _popSize,
                       float probCrossover,
                       float probMutation,
                       Problem *problem) {
  populationSize = _popSize;
  geneSize = problem->x_num;
  crossover_prob = probCrossover;
  mutation_prob = probMutation;
  problem_ = problem;
}

void Population::initialize() {
  for (int i = 0; i < populationSize; i++) {
    individualSet.push_back(Individual(geneSize, problem_));
  }
}

void Population::clear() {
  individualSet.clear();
}

Population Population::copy() {
  return Population(populationSize, crossover_prob, mutation_prob, problem_);
}

Population Population::copy_all() {
  Population _tmpPop(populationSize, crossover_prob, mutation_prob, problem_);
  _tmpPop.individualSet = individualSet;
  return _tmpPop;
}

Population Population::combination(Population q) {
  Population _tmp(populationSize * 2, crossover_prob, 1 / (populationSize * 2), problem_);
  _tmp.individualSet = individualSet;

  for (auto ind : q.individualSet) {
    _tmp.individualSet.push_back(ind);
  }
  return _tmp;
}

void Population::evaluation() {
  for (auto &ind : individualSet) {
    ind.evaluation();
  }
}