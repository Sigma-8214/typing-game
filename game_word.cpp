#include "game_word.hpp"
#include "consts.hpp"
#include "lab_color.hpp"

GameWord GameWord::create(std::string word, Point2f position) {
    auto self = GameWord();

    self.word = word;
    self.spawn_position = position;
    self.position = position;

    self.speed = 1 + static_cast<float32_t>(rand()) / RAND_MAX;
    self.phase = static_cast<float32_t>(rand()) / RAND_MAX;
    self.hue = 0;

    self.typed = 0;

    return self;
}

void GameWord::type(char character) {
    if (word[typed] == character)
        typed++;
}

bool GameWord::is_complete() const { return typed == word.size(); }

bool GameWord::is_out_of_range(Gui &gui) const {
    return position.y > gui.get_height();
}

size_t GameWord::get_length() const { return word.size(); }

Point2f GameWord::get_position() const { return position; }

std::string GameWord::get_word() const { return word; }

void GameWord::render(Gui &gui, bool current_word) {
    auto style = Style::unstyled();
    if (current_word)
        style = style.with_underline();

    gui.draw_text(
        static_cast<Point2i>(position), word.substr(0, typed),
        style.with_fg(COLOR_START.hue_shift((hue + phase) * 2 * PI).to_color())
    );

    gui.draw_text(
        static_cast<Point2i>(position + Point2f::create(typed, 0)),
        word.substr(typed, word.size() - typed), style
    );

    this->position.y += this->speed * gui.get_delta_time();
    this->position.x =
        this->spawn_position.x + 10 * sin(this->position.y / 10 + phase);
    this->hue += 0.25 * gui.get_delta_time();
}
