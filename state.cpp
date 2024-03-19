#include <chrono>

#include "state.hpp"

State State::create(std::vector<std::string> words) {
    auto self = State();
    self.words = words;
    self.start_time = std::chrono::system_clock::now();
    return self;
}
