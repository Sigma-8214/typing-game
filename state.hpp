#pragma once

#include <chrono>
#include <string>
#include <vector>

class State {
  public:
    std::chrono::time_point<std::chrono::system_clock> start_time;
    std::vector<std::string> words;

    static State create(std::vector<std::string> words);
};
