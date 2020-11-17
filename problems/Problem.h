//
// Created by yang on 2020/11/13.
//


#ifndef NSGA2__PROBLEM_H_
#define NSGA2__PROBLEM_H_
#include "pch.h"

class Problem {
 public:
  int x_num;
  int objective_size;
  //定义域
  std::vector<std::vector<float>> domain;
  float x_min, x_max;
  virtual void evaluation(std::vector<float> &, std::vector<float> &) = 0;
  virtual void reference() = 0;
};

#endif
