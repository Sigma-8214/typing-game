#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "ui.hpp"

std::vector<std::string> load_words(const std::string& path) {
    auto words = std::vector<std::string>();
    auto file = std::ifstream(path);

    for (std::string line; std::getline(file, line);)
        words.push_back(line);

    return words;
}

int main() {
    srand(time(0));

    auto words = load_words("data/words.txt");
    if (words.empty()) {
        std::cerr << "No words loaded, exiting." << std::endl;
        return -1;
    }

    auto state = State::create(words, "scores.bin");
    auto ui = Ui::create(state);

    ui.run();
    return 0;
}

