//
// Created by yang on 2020/11/13.
//
#include "pch.h"
#include "ZDT1.h"
//f1 = float(x[0])
//sum1 = 0.0
//for i in range(x_num - 1):
//sum1 = sum1 + x[i + 1]
//g = float(1 + 9 * (sum1 / (x_num - 1)))
//f2 = g * (1 - (f1 / g) ** (0.5))
void ZDT1::evaluation(std::vector<float> &genes, std::vector<float> &objectiveSet) {
  int x_num = genes.size();
  objectiveSet.resize(2);

  float f1 = genes[0];
  objectiveSet[0] = f1;

  float sum = 0;
  for (int i = 1; i < x_num; ++i) {
    sum += genes[i];
  }
  float g = 1.0 + 9.0 * (sum / float(x_num - 1));
  float f2 = g * (1.0 - pow(f1 / g, 0.5));
  objectiveSet[1] = f2;

}

void ZDT1::reference() {
  int size = POPULATION_SIZE;
  double x[size];
  x[0] = 0;
  for (int i = 1; i < size; ++i) {
    x[i] = x[i - 1] + 1.0 / size;
  }

  double y[size];
  for (int i = 0; i < size; ++i) {
    y[i] = 1 - sqrt(x[i]);
  }

  std::ofstream os("reference.txt");
  for (int i = 0; i < size; ++i) {
    os << x[i] << "\t" << y[i] << std::endl;
  }
  os.close();

}
ZDT1::ZDT1() {
  x_min = 0.0;
  x_max = 1.0;
  x_num = 30;
  objective_size = 2;
//  domain = reinterpret_cast<float **>(new float[x_num][2]);
}


