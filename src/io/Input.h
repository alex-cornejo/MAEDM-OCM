/*******************************************************************************************
Authors: J. A. Cornejo-Acosta
Description: Load input configuration for the experimentation.
********************************************************************************************/

#pragma once

#include "../nlohmann/json.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct AlgorithmConf {
    const double epsilon = 1e-9;
    double maTime = 0;
    double ilsTime = 0;
    AlgorithmConf() = default; // Add default constructor
    bool isValid() const { return std::abs(maTime + ilsTime - 1.0) < epsilon; }
    bool containsMA() const { return maTime > epsilon; }
    bool containsILS() const { return ilsTime > epsilon; }
    bool standaloneILS() const { return not containsILS(); }
};

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
    int neighborCov; // neighborhood coverage
    double initialDiversityFactor;
    AlgorithmConf algorithmConf; // algorithm configuration to run and their time limit

  public:
    explicit Input(const std::string& configFilePath) {
        std::ifstream i(configFilePath);
        // check if open
        if (!i.is_open()) {
            throw std::invalid_argument("Could not open config file: " + configFilePath + "\n");
        }
        i >> this->json;

        this->inputFile = this->json["inputFile"].get<std::string>();
        this->outputFile = this->json["outputFile"].get<std::string>();
        this->timeLimit = this->json["timeLimit"].get<double>();
        this->seed = this->json["seed"].get<int>();
        if (this->json.contains("neighborCov")) {
            this->neighborCov = this->json["neighborCov"].get<int>();
        } else {
            // Use recommended default if it is not provided in the config.
            this->neighborCov = 45;
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

        // initial diversity factor
        if (this->json.contains("initialDiversityFactor")) {
            this->initialDiversityFactor = this->json["initialDiversityFactor"].get<double>();
            if (this->initialDiversityFactor < 0 || this->initialDiversityFactor > 1) {
                throw std::invalid_argument("Invalid initial diversity factor in config file!\n");
            }
        } else {
            std::cout
                << "initialDiversityFactor not found, using default initialDiversityFactor=0.4\n";
            this->initialDiversityFactor = 0.4;
        }

        // algorithm configuration
        setAlgorithmConf();
    }

    // getters
  public:
    int getSeed() const { return this->seed; }
    double getTimeLimit() const { return this->timeLimit; }
    nlohmann::json toJson() const { return this->json; }
    std::string getOutputFile() const { return this->outputFile; }
    std::string getInputFile() const { return this->inputFile; }
    ReplacementType getReplacementType() const { return this->replacementType; }
    const std::vector<int>& getPerturbations() const { return this->perturbations; }
    bool getTracing() const { return this->tracing; }
    int getTraceCount() const { return this->traceCount; }
    bool getReduction() const { return this->reduction; }
    int getNeighborCov() const { return this->neighborCov; }
    double getInitialDiversityFactor() const { return this->initialDiversityFactor; }
    AlgorithmConf& getAlgorithmConf() { return this->algorithmConf; }

    // setters
    void setAlgorithmConf() {
        if (this->json.contains("algorithm")) {
            nlohmann::json algorithmJson = this->json["algorithm"].get<nlohmann::json>();
            algorithmConf.ilsTime = algorithmJson["ilsTime"].get<double>();
            algorithmConf.maTime = algorithmJson["maTime"].get<double>();

            if (not algorithmConf.isValid()) {
                throw std::invalid_argument("Total runtime of algorithms must be 1.0!");
            }
        } else {
            throw std::invalid_argument("No algorithms to run were found in the config file!");
        }
    }
};