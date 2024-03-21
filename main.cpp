#include "Windows.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "../tui_engine/tui.hpp"

#include "consts.hpp"
#include "falling_word.hpp"
#include "lab_color.hpp"
#include "screen.hpp"
#include "ui.hpp"
#include "word_shower.hpp"

std::vector<std::string> load_words(std::string path) {
    auto words = std::vector<std::string>();
    auto file = std::ifstream(path);

    for (std::string line; std::getline(file, line);)
        words.push_back(line);

    return words;
}

int main() {
    auto words = load_words("../words/output.txt");
    auto state = State::create(words, "scores.bin");
    auto ui = Ui::create(state);

    ui.run();
    return 0;
}
