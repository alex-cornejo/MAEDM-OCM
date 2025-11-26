#pragma once

#include <vector>

#include "Input.h"
#include <nlohmann/json.hpp>

class Output {

private:
  Input input;
  std::vector<int> solution;
  std::vector<double> diversity;
  bool save(const std::string &output_path, const std::string &content) const {
    std::ofstream output_file(output_path);
    if (!output_file.is_open()) return false;
    output_file << content;
    output_file.close();
    return true;
  }

public:
  Output(const Input &input_) : input(input_) {}

  void addToSolution(int val) { solution.push_back(val); }
  void addToDiversity(double val) { diversity.push_back(val); }

  nlohmann::json toJson() const {
    nlohmann::json bodyJson;
    bodyJson["input"] = input.toJson();
    bodyJson["solution"] = solution;
    bodyJson["diversity"] = diversity;
    return bodyJson;
  }

  void write() const {
    std::string outputFilePath = input.getOutputFile();
    std::string content = toJson().dump();
    save(outputFilePath, content);
  }
};