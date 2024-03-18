#pragma once

#include <vector>

#include "falling_word.hpp"

class WordShower {
    std::vector<FallingWord> words;
    std::vector<std::string> word_list;

    void spawn_word(uint16_t width);

  public:
    uint8_t max_words = 5;
    uint32_t typed;

    static WordShower create(std::vector<std::string> words);

    void draw(Gui &gui);
    void typed_char(char character);
};
