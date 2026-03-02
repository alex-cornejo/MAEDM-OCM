/*******************************************************************************************
Authors: J. A. Cornejo-Acosta 
Description: Load input configuration for the experimentation.  
********************************************************************************************/

#pragma once

#include <fstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

class Input {

public:
  enum class ReplacementType { BNP, TRUN, GEN, ELIT };

private:
  nlohmann::json json; // for json parsing
  std::string inputFile;
  std::string outputFile;
  double timeLimit;
  std::string replacement;
  std::vector<int> perturbations;
  int seed;
  ReplacementType replacementType;
  bool tracing;
  int traceCount;
  bool reduction;
  int cuttingMult;

public:
  Input(const std::string &configFilePath) {
    std::ifstream i(configFilePath);
    // check if open
    if (!i.is_open()) {
      throw std::invalid_argument(
          "Could not open config file: " + configFilePath + "\n");
    }
    i >> this->json;

    this->inputFile = this->json["inputFile"].get<std::string>();
    this->outputFile = this->json["outputFile"].get<std::string>();
    this->timeLimit = this->json["timeLimit"].get<double>();
    this->seed = this->json["seed"].get<int>();
    if (this->json.contains("cuttingMult")) {
      this->cuttingMult = this->json["cuttingMult"].get<int>();
    } else {
      // Use recommended default if cuttingMult is not provided in the config.
      this->cuttingMult = 45;
    }
    this->tracing = this->json["tracing"].get<bool>();
    if (this->tracing) {
      this->traceCount = this->json["traceCount"].get<int>();
    }
    this->reduction = this->json["reduction"].get<bool>();

    // perturbations
    for (int pert : this->json["perturbations"].get<std::vector<int>>()) {
      this->perturbations.push_back(pert);
    }

    // replacement type
    std::string replacement = this->json["replacement"].get<std::string>();
    if (replacement == "BNP") {
      this->replacementType = ReplacementType::BNP;
    } else if (replacement == "TRUN") {
      this->replacementType = ReplacementType::TRUN;
    } else if (replacement == "GEN") {
      this->replacementType = ReplacementType::GEN;
    } else if (replacement == "ELIT") {
      this->replacementType = ReplacementType::ELIT;
    } else {
      throw std::invalid_argument("Invalid replacement type in config file!\n");
    }
  }

  // getters
public:
  int getSeed() const { return this->seed; }
  double getTimeLimit() const { return this->timeLimit; }
  nlohmann::json toJson() const { return this->json; }
  std::string getOutputFile() const { return this->outputFile; }
  std::string getInputFile() const { return this->inputFile; }
  ReplacementType getReplacementType() const { return this->replacementType; }
  const std::vector<int> &getPerturbations() const {
    return this->perturbations;
  }
  bool getTracing() const { return this->tracing; }
  int getTraceCount() const { return this->traceCount; }
  bool getReduction() const { return this->reduction; }
  int getCuttingMult() const { return this->cuttingMult; }

};