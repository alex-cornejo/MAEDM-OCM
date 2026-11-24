/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda

Description: some internal operations required by MA:
- getCost as the negative of the cost because it is a maximization problem (LOP)
- clone individuals
********************************************************************************************/

#include "Individual.h"

Individual::Individual() { S.resize(dim); }

TFitness Individual::getCost() { return -cost; }

bool Individual::needsRestart(double elapsedTime) {
    if (restartTimes.empty())
        return false;
    if (elapsedTime >= restartTimes.back()) {
        return true;
    }
    return false;
}

void Individual::restart() {
    initialize_Random();
    evaluate();
    if (restartTimes.empty()) {
        throw runtime_error("Invalid restart call for standalone ILS");
    }
    restartTimes.pop_back();
}

void Individual::initRestartTimes(const int numRestarts, const double timeLimit) {
    if (numRestarts <= 0)
        return;
    restartTimes.reserve(numRestarts);
    double delta;
    delta = timeLimit / (numRestarts + 1);
    for (int i = numRestarts; i >= 1; --i) {
        restartTimes.push_back(delta * i);
    }
}

Individual Individual::internalClone() {
    Individual tempInd;
    tempInd.S = S;
    tempInd.cost = cost;
    tempInd.distance = distance;
    return tempInd;
}
