#include <chrono>
#include <filesystem>
#include <string>
#include <vector>

#include "state.hpp"

State State::create(std::vector<std::string> words, std::string score_path) {
    auto self = State();
    self.words = words;
    self.start_time = std::chrono::system_clock::now();

    self.score_path = score_path;
    self.scores = Scores::load(score_path);
    return self;
}
