#include "pch.h"
#include "GeneticAlgorithm.h"

void GeneticAlgorithm::SBX(int eta, Population &p) {
  vector<Individual> _tmpSet;
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<> dist_real(0.0, 1.0);
  double yl = p.problem_->x_min;
  double yu = p.problem_->x_max;
  double y1, y2;
  double alpha, beta, betaq;

  for (int i = 0; i < p.populationSize / 2; i++) {
    Individual p1 = p.individualSet[i];
    Individual p2 = p.individualSet[i + (p.populationSize / 2)];

    Individual c1(p.geneSize, p.problem_);
    Individual c2(p.geneSize, p.problem_);

    if (dist_real(rng) <= p.crossover_prob) {
      for (int j = 0; j < p.geneSize; j++) {
        if (abs(p1.genes[j] - p2.genes[2]) > numeric_limits<float>::epsilon()) {
          if (dist_real(rng) < 0.5) {
            if (p1.genes[j] < p2.genes[j]) {
              y1 = p1.genes[j];
              y2 = p2.genes[j];
            } else {
              y1 = p2.genes[j];
              y2 = p1.genes[j];
            }

            double rand = dist_real(rng);
            beta = 1.0 + (2.0 * (y1 - yl) / (y2 - y1));
            alpha = 2.0 - pow(beta, -(eta + 1.0));
            if (rand <= (1.0 / alpha)) {
              betaq = pow((rand * alpha), (1.0 / (eta + 1.0)));
            } else {
              betaq = pow((1.0 / (2.0 - rand * alpha)), (1.0 / (eta + 1.0)));
            }
            double c11, c22;
            c11 = 0.5 * ((y1 + y2) - betaq * (y2 - y1));

            beta = 1.0 + (2.0 * (yu - y2) / (y2 - y1));
            alpha = 2.0 - pow(beta, -(eta + 1.0));
            if (rand <= (1.0 / alpha)) {
              betaq = pow((rand * alpha), (1.0 / (eta + 1.0)));
            } else {
              betaq = pow((1.0 / (2.0 - rand * alpha)), (1.0 / (eta + 1.0)));
            }
            c22 = 0.5 * ((y1 + y2) + betaq * (y2 - y1));

            if (c11 < yl)
              c11 = yl;
            if (c22 < yl)
              c22 = yl;
            if (c11 > yu)
              c11 = yu;
            if (c22 > yu)
              c22 = yu;
            if (dist_real(rng) <= 0.5) {
              c1.genes[j] = c22;
              c2.genes[j] = c11;
            } else {
              c1.genes[j] = c11;
              c2.genes[j] = c22;
            }

          } else {
            c1.genes[j] = p1.genes[j];
            c2.genes[j] = p2.genes[j];
          }
        } else {
          c1.genes[j] = p1.genes[j];
          c2.genes[j] = p2.genes[j];
        }
      }

      _tmpSet.push_back(c1);
      _tmpSet.push_back(c2);
    } else {
      _tmpSet.push_back(p1);
      _tmpSet.push_back(p2);
    }
  }
  p.individualSet = _tmpSet;
}

void GeneticAlgorithm::PLM(int eta, Population &p) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<> dist_real(0, 1);
  float lower = p.problem_->x_min;
  float upper = p.problem_->x_max;

  for (int i = 0; i < p.individualSet.size(); i++) {
    Individual &ind = p.individualSet[i];
    if (dist_real(rng) <= p.mutation_prob) {
      for (int j = 0; j < p.geneSize; j++) {
        //开始变异
        float u = dist_real(rng);
        float delta;
        if (u < 0.5) {
          delta = pow(2.0 * u, 1 / (1.0 + eta)) - 1;
        } else {
          delta = pow(1.0 - (2 * (1 - u)), 1 / (1.0 + eta));
        }
        ind.genes[j] += delta;

        if (ind.genes[j] < lower) {
          ind.genes[j] = lower;
        }
        if (ind.genes[j] > upper) {
          ind.genes[j] = upper;
        }

      }

    }
  }
}

//快速非支配排序
vector<Front> GeneticAlgorithm::fastNonDominatedSort(Population &r) {
  vector<Front> front;
  front.resize(1);

  for (int i = 0; i < r.populationSize; i++) {
    Individual *p = &r.individualSet[i];
    p->dominatedSet.clear();
    p->dominatedCount = 0;

    for (int j = 0; j < r.populationSize; j++) {
      if (i == j) { continue; }
      Individual *q = &r.individualSet[j];
      if (p->dominate(*q)) {
        p->dominatedSet.push_back(q);
      } else if (q->dominate(*p)) {
        p->dominatedCount++;
      }
    }

    if (p->dominatedCount == 0) {
      p->rank = 0;
      front[0].push_back(p);
    }
  }

  int i = 0;
  while (front[i].size() != 0) {
    vector<Individual *> _tmpSet;
    _tmpSet.resize(0);
    for (int j = 0; j < front[i].size(); j++) {
      Individual *p = front[i][j];
      for (int k = 0; k < p->dominatedSet.size(); k++) {
        Individual *q = p->dominatedSet[k];
        q->dominatedCount--;
        if (q->dominatedCount == 0) {
          q->rank = i + 1;
          _tmpSet.push_back(q);
        }
      }
    }
    i++;
    front.push_back(_tmpSet);
  }
  return front;
}

bool comparator_0(Individual *a, Individual *b) {
  return a->objectiveSet[0] < b->objectiveSet[0];
}

bool comparator_1(Individual *a, Individual *b) {
  return a->objectiveSet[1] < b->objectiveSet[1];
}

//计算拥挤度
void GeneticAlgorithm::crowdingDistanceAssignment(Front f, Population &p) {
  int objectiveSize = 2;
  float max_min_table[2][objectiveSize];
  //初始化
  for (int i = 0; i < 2; ++i) {
    for (int kI = 0; kI < objectiveSize; ++kI) {
      if (i == 0) {
        //max
        max_min_table[i][kI] = numeric_limits<float>::min();
      } else {
        //min
        max_min_table[i][kI] = numeric_limits<float>::max();
      }
    }
  }

  for (auto &f1 : f) {
    for (int kI = 0; kI < objectiveSize; ++kI) {
      //max
      if (max_min_table[0][kI] < f1->objectiveSet[kI]) {
        max_min_table[0][kI] = f1->objectiveSet[kI];
      }
      //min
      if (max_min_table[1][kI] > f1->objectiveSet[kI]) {
        max_min_table[1][kI] = f1->objectiveSet[kI];
      }
    }
  }

  int l = f.size();
  for (int i = 0; i < l; i++) {
    f[i]->distance = 0;
  }
  for (int m = 0; m < objectiveSize; m++) {
    if (m == 0) {
      sort(f.begin(), f.end(), comparator_0);
    } else if (m == 1) {
      sort(f.begin(), f.end(), comparator_1);
    }
    f[0]->distance = numeric_limits<float>::infinity();
    f[l - 1]->distance = numeric_limits<float>::infinity();
    for (int j = 2; j < l - 1; j++) {
      f[j]->distance = f[j]->distance
          + (f[j + 1]->objectiveSet[m] - f[j - 1]->objectiveSet[m]) / (max_min_table[0][m] - max_min_table[1][m]);
//      / (f_max.objectiveSet[m] - f_min.objectiveSet[m]);
    }
  }
}

//二元竞标赛选择
void GeneticAlgorithm::tournament_selection(Population &p) {
  int n = 2;
  int pop_size = p.populationSize;
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<> dist_real(0, 1);
  vector<Individual> _temp_individual_set;
  vector<Individual *> candidates;

  for (int kI = 0; kI < pop_size; ++kI) {
    candidates.clear();
    //随机选取两个
    for (int kJ = 0; kJ < n; ++kJ) {
      int index = int(dist_real(rng) * pop_size);
      Individual *candidate = &p.individualSet[index];
      candidates.push_back(candidate);
    }
    //选择一个进入

    sort(candidates.begin(), candidates.end(), Individual::descending);
    _temp_individual_set.push_back(*candidates[0]);
  }

  p.individualSet = _temp_individual_set;
}
Population GeneticAlgorithm::next_new_pop(Population &p, int eta_c, int eta_m) {
  Population _tmp = p.copy_all();

  tournament_selection(_tmp);
  SBX(eta_c, _tmp);
  PLM(eta_m, _tmp);
  _tmp.evaluation();
  return _tmp;
}
void GeneticAlgorithm::elitism(Population &p) {
  vector<Front> front = fastNonDominatedSort(p);
  vector<Individual> _tmp_individual_set;

  int j = 0;
  while (_tmp_individual_set.size() + front[j].size() <= POPULATION_SIZE) {
    if (front[j].size() == 0) {
      cin.get();
      break;
    }

    for (Individual *ind : front[j]) {
      _tmp_individual_set.push_back(*ind);
    }
    j++;
  }
  crowdingDistanceAssignment(front[j], p);
  sort(front[j].begin(), front[j].end(), Individual::descending);
  int size = _tmp_individual_set.size();
  for (int k = 0; k < (POPULATION_SIZE - size); k++) {
    _tmp_individual_set.push_back(*front[j][k]);
  }

  p.individualSet = _tmp_individual_set;
  p.populationSize = POPULATION_SIZE;
}
