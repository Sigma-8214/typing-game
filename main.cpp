#include "Windows.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "../tui_engine/tui.hpp"

#include "falling_word.hpp"
#include "word_shower.hpp"

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
