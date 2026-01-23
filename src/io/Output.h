#pragma once

#include <vector>

#include "Input.h"
#include <nlohmann/json.hpp>

class Result {

private:
  Input &input;
  std::vector<int> solution;
  bool reduced;
  bool isBig;
  double runtime;
  int ngen;
  long long lsCallsCount; // number of local search calls
  struct TraceEntry {
    long long fitness;
    double diversity;
    double ediversity;
    double elapsedTime;

    nlohmann::json toJson() const {
      nlohmann::json j;
      j["fitness"] = fitness;
      j["diversity"] = diversity;
      j["elapsedTime"] = elapsedTime;
      j["ediversity"] = ediversity;
      return j;
    }
  };
  std::vector<TraceEntry> trace;

  bool save(const std::string &output_path, const std::string &content) const {
    std::ofstream output_file(output_path);
    if (!output_file.is_open()) return false;
    output_file << content;
    output_file.close();
    return true;
  }

public:
  Result(Input &input_) : input(input_) {}

  void addToSolution(int val) { solution.push_back(val); }
  void setReduced(bool val) { reduced = val; }
  Input& getInput() { return input; }
  void setIsBig(bool val) { isBig = val; }
  void setRuntime(double runtime_) { runtime = runtime_; }
  void setNgen(int ngen_) { ngen = ngen_; }
  void setLsCallsCount(long long count) { lsCallsCount = count; }
  void addTrace(long long fitness, double diversity, double ediversity, double elapTime) {
    trace.push_back({fitness, diversity, ediversity,elapTime});
  }

  nlohmann::json toJson() const {
    nlohmann::json bodyJson;
    bodyJson["input"] = input.toJson();
    bodyJson["solution"] = solution;
    bodyJson["reduced"] = reduced;
    bodyJson["isBig"] = isBig;
    bodyJson["runtime"] = runtime;
    bodyJson["ngen"] = ngen;
    bodyJson["lsCallsCount"] = lsCallsCount;
    nlohmann::json traceJson;
    for (const auto &entry : trace) {
      traceJson.push_back(entry.toJson());
    }
    bodyJson["trace"] = traceJson;
    return bodyJson;
  }

  void write() const {
    std::string outputFilePath = input.getOutputFile();
    std::string content = toJson().dump();
    save(outputFilePath, content);
  }
};