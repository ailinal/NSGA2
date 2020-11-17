#include "pch.h"
#include "Individual.h"

Individual::Individual() {}

Individual::Individual(int geneSize, Problem *problem) {
  problem_ = problem;
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_real_distribution<> dist_float(problem->x_min, problem->x_max);

  for (int i = 0; i < geneSize; i++) {
    genes.push_back(dist_float(rng));
  }
}

void Individual::evaluation() {


//  objectiveSet.resize(2);
//  for (int i = 0; i < genes.size(); i++) {
//    float f1 = pow(genes[i], 2.0);
//    objectiveSet[0] = f1;
//
//    float f2 = pow((genes[i] - 2.0), 2.0);
//    objectiveSet[1] = f2;
//  }
  problem_->evaluation(genes, objectiveSet);

}

bool Individual::dominate(Individual q) {
  for (int i = 0; i < objectiveSet.size(); i++) {
    if (objectiveSet[i] > q.objectiveSet[i]) {
      return false;
    }
  }
  return true;
}
bool Individual::descending(Individual* a, Individual* b) {
  if ((a->rank < b->rank) || ((a->rank == b->rank) && (a->distance > b->distance))) {
    return true;
  } else {
    return false;
  }
}