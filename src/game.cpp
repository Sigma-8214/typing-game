#include <string>
#include <utility>
#include <vector>

#include "../tui_engine/tui.hpp"

#include "game.hpp"

const auto CONFETTI = ParticleConfig::create(2, 1, {'*', '~', '.', '-'});

void Game::spawn_word(uint16_t width) {
    std::string word;
    uint16_t attempts = 20;
    do
        word = word_list[rand() % word_list.size()];
    while (!first_chars.insert(word[0]).second && attempts-- > 0);

    if (attempts == 0)
        return;

    auto offset = word.size() + 10;
    auto max_width = std::max(static_cast<size_t>(width), offset) - offset;
    auto position = Point2f::create(rand() % max_width, 0);

    words.push_back(GameWord::create(word, position));
}

Game Game::create(std::vector<std::string> words) {
    auto self = Game();
    self.word_list = std::move(words);
    self.confetti = ParticleEmitter::create(CONFETTI);
    return self;
}

void Game::draw(Gui &gui) {
    auto local_max_words = max_words + typed / 10;
    while (words.size() < local_max_words)
        spawn_word(gui.get_width());

    while (words.size() > local_max_words)
        words.pop_back();

    confetti.render(gui);

    for (auto i = 0; i < words.size(); i++) {
        auto &word = words[i];
        auto current = current_word.has_value() && i == current_word.value();

        word.render(gui, current);

        auto out_of_range = word.is_out_of_range(gui);
        auto was_typed = word.is_complete();
        if (lives > 0)
            lives -= out_of_range;
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

        if (out_of_range || was_typed) {
            if (current)
                current_word = std::nullopt;

            first_chars.erase(word.get_word()[0]);
            words.erase(words.begin() + i--);
        }
    }
}

void Game::typed_char(char character) {
    if (current_word.has_value()) {
        words[current_word.value()].type(character);
        return;
    }

    for (auto i = 0; i < words.size(); i++) {
        if (words[i].get_word()[0] == character) {
            current_word = i;
            words[i].type(character);
            break;
        }
    }
}

uint8_t Game::get_lives() const { return lives; }

uint32_t Game::get_typed() const { return typed; }
