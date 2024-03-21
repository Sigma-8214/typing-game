#include <string>
#include <vector>

#include "../tui_engine/tui.hpp"

#include "word_shower.hpp"

const auto CONFETTI = ParticleConfig::create(2, 1, {'*', '~', '.', '-'});

void WordShower::spawn_word(uint16_t width) {
    auto word = word_list[rand() % word_list.size()];

    auto offset = word.size() + 10;
    auto max_width = std::max(static_cast<size_t>(width), offset) - offset;
    auto position = Point2f::create(rand() % max_width, 0);

    words.push_back(FallingWord::create(word, position));
}

WordShower WordShower::create(std::vector<std::string> words) {
    auto self = WordShower();
    self.word_list = words;
    self.confetti = ParticleEmitter::create(CONFETTI);
    return self;
}

void WordShower::draw(Gui &gui) {
    while (words.size() < max_words)
        spawn_word(gui.get_width());

    while (words.size() > max_words)
        words.pop_back();

    confetti.render(gui);

    for (auto i = 0; i < words.size(); i++) {
        auto &word = words[i];
        word.render(gui);

        auto was_typed = word.is_complete();
        typed += was_typed;

        if (was_typed) {
            for (auto i = 0; i < 10; i++)
                confetti.emit(
                    word.get_position() +
                        Point2f::create(word.get_length() / 2, 0),
                    Point2f::create(
                        5 * static_cast<float32_t>(rand()) / RAND_MAX - 2.5,
                        5 * static_cast<float32_t>(rand()) / RAND_MAX - 2.5
                    )
                );
        }

        if (word.is_out_of_range(gui) || was_typed)
            words.erase(words.begin() + i--);
    }
}

void WordShower::typed_char(char character) {
    for (auto &word : words)
        word.type(character);
}
