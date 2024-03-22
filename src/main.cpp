#include <fstream>
#include <string>
#include <vector>

#include "ui.hpp"

std::vector<std::string> load_words(std::string path) {
    auto words = std::vector<std::string>();
    auto file = std::ifstream(path);

    for (std::string line; std::getline(file, line);)
        words.push_back(line);

    return words;
}

int main() {
    srand(time(0));

    auto words = load_words("../words/output.txt");
    auto state = State::create(words, "scores.bin");
    auto ui = Ui::create(state);

    ui.run();
    return 0;
}
