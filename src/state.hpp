#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "scores.hpp"

class State {
  public:
    std::chrono::time_point<std::chrono::system_clock> start_time;
    std::vector<std::string> words;

    std::string score_path;
    Scores scores;

    static State create(std::vector<std::string> words, std::string score_path);
};
