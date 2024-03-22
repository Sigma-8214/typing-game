#pragma once

#include <optional>
#include <set>
#include <vector>

#include "game_word.hpp"
#include "particle_emitter.hpp"

class Game {
    std::vector<GameWord> words;
    std::set<char> first_chars;
    std::optional<size_t> current_word;
    uint8_t lives = 3;
    uint32_t typed = 0;

    std::vector<std::string> word_list;
    ParticleEmitter confetti;

    void spawn_word(uint16_t width);

  public:
    uint8_t max_words = 5;

    static Game create(std::vector<std::string> words);

    void draw(Gui &gui);
    void typed_char(char character);

    uint8_t get_lives() const;
    uint32_t get_typed() const;
};
