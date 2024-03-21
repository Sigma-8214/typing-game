#include "falling_word.hpp"
#include "consts.hpp"
#include "lab_color.hpp"

FallingWord FallingWord::create(std::string word, Point2f position) {
    auto self = FallingWord();

    self.word = word;
    self.spawn_position = position;
    self.position = position;

    self.speed = 1 + static_cast<float32_t>(rand()) / RAND_MAX;
    self.phase = static_cast<float32_t>(rand()) / RAND_MAX;
    self.hue = 0;

    self.typed = 0;

    return self;
}

void FallingWord::type(char character) {
    if (word[typed] == character)
        typed++;
}

bool FallingWord::is_complete() const { return typed == word.size(); }

bool FallingWord::is_out_of_range(Gui &gui) const {
    return position.y > gui.get_height();
}

size_t FallingWord::get_length() const { return word.size(); }

Point2f FallingWord::get_position() const { return position; }

void FallingWord::render(Gui &gui) {
    gui.draw_text(
        static_cast<Point2i>(position), word.substr(0, typed),
        Style::unstyled().with_fg(
            COLOR_START.hue_shift((hue + phase) * 2 * PI).to_color()
        )
    );

    gui.draw_text(
        static_cast<Point2i>(position + Point2f::create(typed, 0)),
        word.substr(typed, word.size() - typed), Style::unstyled()
    );

    this->position.y += this->speed * gui.get_delta_time();
    this->position.x =
        this->spawn_position.x + 10 * std::sin(this->position.y / 10 + phase);
    this->hue += 0.25 * gui.get_delta_time();
}
