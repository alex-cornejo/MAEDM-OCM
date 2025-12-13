/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda

Description: class for the Memetic Algorithm with Explicit Diversity Management (MA-EDM)
published in the paper "A Diversity-aware Memetic Algorithm for the Linear Ordering Problem".
It was modified for the Pace Challenge 2024 with the following features:
- More complex perturbations
- More efficient local search with the use of gain matrices and fast break
- Use of long long only when required
********************************************************************************************/

#ifndef __MA_H__
#define __MA_H__

#include "Individual.h"
#include "io/Output.h"
#include <bits/stdc++.h>

class MA {
public:
  MA(int N_, double pc_, const string &crossType_, const string &perturbationType_, double DIfactor_, double ils_time_, double finalTime_, const string &outputFile, int cuttingMult, int swaps, bool reqLongLong, Result &result_);
  void run();
  // Parameters of MA
  int N;                   // Population Size
  double pc;               // crossover probability
  string crossType;        // crossover type
  string perturbationType; // mutation type
  double finalTime;        // Seconds
  string outputFile;
  double DIfactor;
  double ils_time;
  int cuttingMult;
  int swaps;
  bool reqLongLong;
  int ngen;
  long long lsCallsCount = 0;
  

  // Basic procedures of MA
  void initPopulation();
  void initDI();
  void selectParents();
  void crossover();
  void intensify();
  void local_search();
  double computeDiversity();

  // replacement operators
  void replacement(); // BNP replacement
  bool genReplacement(); // Generational replacement
  void elitReplacement(); // Elit replacement
  void trunReplacement(); // Truncation replacement

  // Internal attributes of MA
  vector<Individual *> population;
  vector<Individual *> parents;
  vector<Individual *> offspring;
  double initialTime;
  double DI;
  double elapsedTime;

  // getters
  Individual *getBestGlobal() { return bestGlobal; }
  long long getLSCallsCount() { return lsCallsCount; }

private:
  Individual * bestGlobal = nullptr;
  Result &result;
  vector<double> tracingSteps; // for tracking convergence and diversity behavior
  void initTrack();
  void evalTrace(double elapsedTime); 
};

#endif
