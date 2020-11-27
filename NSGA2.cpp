#include <problems/ZDT1.h>
#include "pch.h"
#include "Population.h"
#include "Individual.h"
#include "GeneticAlgorithm.h"


int main() {
  int generation = 2000;
  GeneticAlgorithm ga;
  Problem *problem = new ZDT1();
  problem->reference();
  Population _tmpParent(POPULATION_SIZE, CROSSOVER_PROB, MUTATION_PROB, problem);

  _tmpParent.initialize();
  _tmpParent.evaluation();

  //非支配排序 
  ga.fastNonDominatedSort(_tmpParent);
  Population _tmpChild = ga.next_new_pop(_tmpParent, ETA_C, ETA_M);

  for (int i = 0; i < generation; i++) {
    Population _tmpComb = _tmpParent.combination(_tmpChild);
    ga.elitism(_tmpComb);
    _tmpParent = _tmpComb;
    _tmpChild = ga.next_new_pop(_tmpParent, ETA_C, ETA_M);
    cout << "Parent size: " << _tmpParent.individualSet.size() << endl;
    cout << "Complete generation [" << i << "]" << endl;
  }

  ofstream os("result_" + to_string(POPULATION_SIZE) + to_string(generation) + ".txt");
  if (os.is_open()) {
    for (auto i : _tmpParent.individualSet) {
      os << i.objectiveSet[0] << "\t" << i.objectiveSet[1] << endl;
    }
  }
  os.close();
}

