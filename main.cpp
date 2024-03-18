#include "Windows.h"
#include <filesystem>
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

void draw_rainbow_text(
    Point2i pos, std::string text, float32_t *hue, Gui &gui
) {
    *hue += 0.25f * gui.get_delta_time();
    for (auto i = 0; i < text.size(); i++) {
        auto local_hue = *hue - (static_cast<float32_t>(i) / text.size() / 2);
        if (local_hue < 0)
            local_hue += 1;

        auto color = COLOR_START.hue_shift(local_hue * 2 * PI).to_color();

        gui.set(
            text[i], pos + Point2i::create(i, 0),
            Style::unstyled().with_fg(color)
        );
    }
}

int main() {
    auto words = load_words("../words/output.txt");
    auto state = State::create(words);
    auto ui = Ui::create(state);

    ui.run();
    return 0;
}
