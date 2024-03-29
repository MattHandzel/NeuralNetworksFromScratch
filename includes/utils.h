#ifndef UTILS
#define UTILS
#include <functional>
#include <random>
#include <chrono>
#include <iostream>
#include <cstdlib> // this is where srand() is defined
#include <ctime>   // this is where time() is defined
#include <limits>  // Getting the limit for a double
#include "math.h"
#include <stdexcept>
#include <limits>
#include <map>
#include <thread>
#include "Eigen/Core"

#include <cassert>

double derivative(std::function<double(double)> func, double x);

void timeIt(std::function<void(void)> func);

double generateRandomNumber(double high, double low);

enum WeightGenerationType{
  RANDOM,
  FIXED,
  
};

std::vector<std::vector<double>> generateWeights(int layer_1_size, int layer_2_size, WeightGenerationType type, double weight = 0);

std::vector<double> generateRandomNumberVector(int size, double high = 1, double low = -1);
long long now();
double computeError(double _true, double _pred);
double computeError(std::vector<double> _true, std::vector<double> _pred);

#endif