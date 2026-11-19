/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda, J. A. Cornejo-Acosta

Description: it is the entry point. It initializes the paramters and decides to use
the ILS of the memetic algorithm depending on the size of the problem to be solved
(after reduction). It also incorporates the part related to the management of signals
for the PaceChallenge 2024.
*******************************************************************************************/

using namespace std;

#include "Individual.h"
#include "Instance.h"
#include "MA.h"
#include "Problem.h"
#include "ProblemLL.h"
#include "io/Input.h"
#include "io/Output.h"
#include "utils/utils.h"

#include <bits/stdc++.h>
#include <iostream>
#include <sys/resource.h>
#include <sys/time.h>

bool volatile finished = false;

void printer(int signal) { finished = true; }

bool isBig = false;
long long crossRemoved = 0;
Problem* Individual::problem;
ProblemLL* Individual::problemll;
Instance* Individual::instance;
int Individual::permBlock;
int Individual::dim;
string Individual::initialization;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cerr << "Input parameters error...\n";
        return 1;
    }
    const string confFile = argv[1];
    Input input(confFile);
    Result result(input);
    const int seed = input.getSeed();
    const double timeLimit = input.getTimeLimit();
    const int neighborCov = input.getNeighborCov();

    signal(SIGTERM, printer);
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    double initialTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec) / 1.0e6;
    srandom(seed);
    Instance* inst = new Instance(true, result);
    Problem* p = NULL;
    ProblemLL* pll = new ProblemLL();
    bool reqLongLong = true;
    if (!isBig) {
        long long max_gain = 0;
        for (int i = 0; i < inst->numNodesB; i++) {
            for (int j = 0; j < inst->numNodesB; j++) {
                pll->instance[i][j] = inst->pair_matrix[i][j];
                pll->gain[i][j] = inst->pair_matrix[i][j] - inst->pair_matrix[j][i];
                max_gain = max(max_gain, llabs(pll->gain[i][j]));
            }
        }
        if (max_gain <= ((1ll << 31) - 1)) {
            reqLongLong = false;
        }
    }
    if (!reqLongLong) {
        delete pll;
        pll = NULL;
        p = new Problem();
        for (int i = 0; i < inst->numNodesB; i++) {
            for (int j = 0; j < inst->numNodesB; j++) {
                p->instance[i][j] = inst->pair_matrix[i][j];
                p->gain[i][j] = inst->pair_matrix[i][j] - inst->pair_matrix[j][i];
            }
        }
    }

    Individual::dim = inst->numNodesB, Individual::problem = p;
    Individual::problemll = pll;
    Individual::instance = inst;

    double ils_time, pc;
    const double initialDiversityFactor = input.getInitialDiversityFactor();
    auto& algorithmConf = input.getAlgorithmConf();
    int f1Time;
    const int swaps = 6;
    const int N = 36;
    if (isBig) {
        Individual::initialization = "Score";

        // force the execution to use 100% ILS (standalone ILS)
        algorithmConf.ilsTime = 1.0;
        algorithmConf.maTime = 0.0;

    } else {
        ils_time = 0.05;
        pc = 0.85;
        f1Time = timeLimit * algorithmConf.maTime; // running time for phase 1
        Individual::initialization = "Random";
    }
    Individual::permBlock = 60;

    gettimeofday(&currentTime, NULL);
    double cTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec) / 1.0e6;

    if (algorithmConf.containsMA()) {
        MA ma(N, pc, "cx", "SWM", initialDiversityFactor, ils_time, f1Time, "out.txt", neighborCov,
              swaps, reqLongLong, result);
        ma.run();
        gettimeofday(&currentTime, NULL);
        cTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec) / 1.0e6;
        long long lastLsCallsCount = 0;
        if (timeLimit - (cTime - initialTime) > 0 and algorithmConf.containsILS()) {
            lastLsCallsCount = ma.getBestGlobal()->intensify(
                "SWM", timeLimit - (cTime - initialTime), neighborCov * 3, swaps, reqLongLong,
                input.getPerturbations());
        }

        for (int nv : ma.getBestGlobal()->S) {
            result.addToSolution(inst->numNodesA + 1 + inst->newToOrig[nv]);
            for (int o : inst->similarInOrig[inst->newToOrig[nv]]) {
                result.addToSolution(inst->numNodesA + 1 + o);
            }
        }
        result.setLsCallsCount(ma.getLSCallsCount() + lastLsCallsCount);
    } else if (algorithmConf.standaloneILS()) {
        // Estimate medwin
        long long times = 1000000;
        vector<long long> values;
        for (int i = 0; i < times; i++) {
            int f = 0, s = 0;
            while (f == s) {
                f = random() % inst->numNodesB;
                s = random() % inst->numNodesB;
            }
            values.push_back(abs(inst->pair_cost(f, s)));
        }
        sort(values.begin(), values.end());
        inst->medWin = values[values.size() / 2];
        Individual* ei = new Individual();
        ei->initialize_heuristic();
        gettimeofday(&currentTime, NULL);
        cTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec) / 1.0e6;
        ei->ils_edges(timeLimit - (cTime - initialTime), neighborCov, swaps);
        for (int nv : ei->S) {
            result.addToSolution(inst->numNodesA + 1 + inst->newToOrig[nv]);
            for (int o : inst->similarInOrig[inst->newToOrig[nv]]) {
                result.addToSolution(inst->numNodesA + 1 + o);
            }
        }
    } else {
        throw std::invalid_argument("No algorithms to run were found in the config file!");
    }
    gettimeofday(&currentTime, NULL);
    cTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec) / 1.0e6;
    result.setIsBig(isBig);
    result.setRuntime(cTime - initialTime);
    result.setILSTime(algorithmConf.ilsTime);
    result.setMATime(algorithmConf.maTime);
    result.write(); // write json with results
    return 0;
}
