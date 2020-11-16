#include <problems/ZDT1.h>
#include "pch.h"
#include "Population.h"
#include "Individual.h"
#include "GeneticAlgorithm.h"

#define POPULATION_SIZE 100
#define CROSSOVER_PROB 0.9
#define MUTATION_PROB (1 / 30)

typedef vector<Individual *> Front;

bool descending(Individual *, Individual *);

int main() {
  int generation = 200;
  GeneticAlgorithm ga;
  Problem *problem = new ZDT1();
  problem->reference();
  Population _tmpParent(POPULATION_SIZE, CROSSOVER_PROB, MUTATION_PROB, problem);

  _tmpParent.initialize();
  _tmpParent.evaluation();

  for (int i = 0; i < generation; i++) {
    Population _tmpChild = _tmpParent.copy_all();

    //交叉变异
    ga.SBX(20, _tmpChild);
//    ga.PLM(20, -(pow(10.0, 3.0)), pow(10.0, 3.0), _tmpChild);
    ga.PLM(20, _tmpChild);
    _tmpChild.evaluation();

    Population _tmpComb = _tmpParent.combination(_tmpChild);
    //非支配排序
    vector<Front> front = ga.fastNonDominatedSort(&_tmpComb);
    _tmpParent.clear();

    int j = 0;
    while (_tmpParent.individualSet.size() + front[j].size() <= POPULATION_SIZE) {
      if (front[j].size() == 0) {
        cin.get();
        break;
      }
//      ga.crowdingDistanceAssignment(front[j], _tmpParent);
      for (Individual *ind : front[j]) {
        _tmpParent.individualSet.push_back(*ind);
      }
      j++;
    }
    ga.crowdingDistanceAssignment(front[j], _tmpParent);
    sort(front[j].begin(), front[j].end(), descending);
    int size = _tmpParent.individualSet.size();
    for (int k = 0; k < (POPULATION_SIZE - size); k++) {
      _tmpParent.individualSet.push_back(*front[j][k]);
    }
    cout << "Parent size: " << _tmpParent.individualSet.size() << endl;
    cout << "Complete generation [" << i << "]" << endl;
  }

  ofstream os("result.txt");
  if (os.is_open()) {
    for (auto i : _tmpParent.individualSet) {
      os << i.objectiveSet[0] << "\t" << i.objectiveSet[1] << endl;
    }
  }
  os.close();
}

bool descending(Individual *a, Individual *b) {
  if ((a->rank < b->rank) || ((a->rank == b->rank) && (a->distance > b->distance))) {
    return true;
  } else {
    return false;
  }
}