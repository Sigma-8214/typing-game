#include "Windows.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "../tui_engine/tui.hpp"

struct FallingWord {
    std::string word;
    Point2f spawn_position;
    Point2f position;

    float32_t speed;
    float32_t phase;
    float32_t hue;

    size_t typed;

    static FallingWord create(std::string word, Point2f position) {
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

    void render(Gui &gui) {
        gui.draw_text(
            static_cast<Point2i>(position), word.substr(0, typed),
            Style::unstyled().with_fg(Color::from_hsv(hue + phase, 1, 1))
        );

        gui.draw_text(
            static_cast<Point2i>(position + Point2f::create(typed, 0)),
            word.substr(typed, word.size() - typed), Style::unstyled()
        );

        this->position.y += this->speed * gui.get_delta_time();
        this->position.x = this->spawn_position.x +
                           10 * std::sin(this->position.y / 10 + phase);
        this->hue += 0.25 * gui.get_delta_time();
    }
};

struct WordShower {
    uint8_t max_words = 5;

    std::vector<FallingWord> words;
    std::vector<std::string> word_list;

    uint32_t typed;

    void spawn_word(uint16_t width) {
        auto word = word_list[rand() % word_list.size()];

        auto offset = word.size() + 10;
        auto max_width = std::max(static_cast<size_t>(width), offset) - offset;
        auto position = Point2f::create(rand() % max_width, 0);

        words.push_back(FallingWord::create(word, position));
    }

    static WordShower create(std::vector<std::string> words) {
        auto self = WordShower();
        self.word_list = words;
        return self;
    }

    void draw(Gui &gui) {
        while (words.size() < max_words)
            spawn_word(gui.get_width());

        while (words.size() > max_words)
            words.pop_back();

        for (auto i = 0; i < words.size(); i++) {
            auto word = &words[i];
            word->render(gui);

            auto was_typed = word->typed == word->word.size();
            if (word->position.y > gui.get_height() || was_typed) {
                words.erase(words.begin() + i--);
                typed += was_typed;
            }
        }
    }

    void typed_char(char character) {
        for (auto &word : words) {
            if (word.word[word.typed] == character)
                word.typed++;
        }
    }
};

std::vector<std::string> load_words(std::string path) {
    auto words = std::vector<std::string>();
    auto file = std::ifstream(path);

    for (std::string line; std::getline(file, line);)
        words.push_back(line);

    return words;
}

int main() {
    auto gui = Gui::create();

    auto words = load_words("../words/output.txt");
    auto shower = WordShower::create(words);

    auto hue = 0.0f;

    for (;;) {
        // == Handle input events ==
        auto events = new DWORD();
        GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), events);

        auto event_buffer = new INPUT_RECORD[*events];
        ReadConsoleInput(
            GetStdHandle(STD_INPUT_HANDLE), event_buffer, *events, events
        );

        for (auto i = 0; i < *events; i++) {
            if (event_buffer[i].EventType != KEY_EVENT)
                continue;

            auto key_event = event_buffer[i].Event.KeyEvent;
            if (key_event.bKeyDown) {
                if (key_event.wVirtualKeyCode == VK_ESCAPE) {
                    gui.cleanup();
                    return 0;
                }

                if (key_event.wVirtualKeyCode == VK_OEM_PLUS)
                    shower.max_words++;

                if (key_event.wVirtualKeyCode == VK_OEM_MINUS)
                    shower.max_words--;

                auto typed_char = key_event.uChar.AsciiChar;
                if (typed_char >= 32 && typed_char <= 126)
                    shower.typed_char(typed_char);
            }
        }

        // == Draw the game ==
        shower.draw(gui);

        // == Draw the title ==
        hue += 0.25f * gui.get_delta_time();
        const auto text = std::string("Word Shower");
        for (auto i = 0; i < text.size(); i++) {
            auto local_hue =
                hue - (static_cast<float32_t>(i) / text.size() / 2);
            if (local_hue < 0)
                local_hue += 1;

            gui.set(
                text[i], Point2i::create(i, 0),
                Style::unstyled().with_fg(Color::from_hsv(local_hue, 1, 1))
            );
        }

        // == Draw bottom bar ==
        gui.draw_text(
            Point2i::create(0, gui.get_height() - 1),
            "[ESC] Quit | [+/-] Increase/Decrease Max Words ", Style::unstyled()
        );

        // == Show Stats ==
        gui.draw_text(
            Point2i::create(0, 2), "Words: " + std::to_string(shower.max_words),
            Style::unstyled()
        );
        gui.draw_text(
            Point2i::create(0, 3), "Typed: " + std::to_string(shower.typed),
            Style::unstyled()
        );

        gui.update();
    }
    return 0;
}