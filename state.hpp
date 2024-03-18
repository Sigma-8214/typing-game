#pragma once

#include <string>
#include <vector>

class State {
  public:
    bool exit;
    std::vector<std::string> words;

    static State create(std::vector<std::string> words);
};