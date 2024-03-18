#include "state.hpp"

State State::create(std::vector<std::string> words) {
    auto self = State();
    self.words = words;
    return self;
}