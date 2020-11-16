//
// Created by yang on 2020/11/13.
//

#ifndef NSGA2_PROBLEM_ZDT1_H_
#define NSGA2_PROBLEM_ZDT1_H_

#include "Problem.h"
class ZDT1 : public Problem {
 public:
  ZDT1();
 private:
  void evaluation(std::vector<float> &, std::vector<float> &);
  void reference();
};

#endif //NSGA2_PROBLEM_ZDT1_H_
