#include "screen_game.hpp"
#include "consts.hpp"
#include "drawing.hpp"
#include "screen_confirm.hpp"
#include "screen_new_high_score.hpp"
#include "word_shower.hpp"

GameScreen GameScreen::create(Ui &ui) {
    auto self = GameScreen();
    self.shower = WordShower::create(ui.state.words);
    self.hue = 0;
    return self;
}

void GameScreen::render(Ui &ui, Gui &gui) {
    shower.draw(gui);

    // == Draw the title ==
    draw_rainbow_text(Point2i::origin(), "Word Shower", &hue, gui);

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
}

void GameScreen::on_key(Ui &ui, KEY_EVENT_RECORD key) {
    if (!key.bKeyDown)
        return;

    if (key.wVirtualKeyCode == VK_ESCAPE) {
        auto score = shower.typed;
        ui.push_screen(std::make_unique<ConfirmScreen>(ConfirmScreen::create(
            "Are you sure you want to exit?", {"No", "Yes"},
            [score](Ui &ui, uint8_t option) {
                if (option == 1) {
                    ui.pop_screen();
                    ui.pop_screen();
                    if (ui.state.scores.is_high_score(score, HIGH_SCORES_COUNT))
                        ui.push_screen(std::make_unique<NewHighScoreScreen>(
                            NewHighScoreScreen::create(score)
                        ));
                } else if (option == 0)
                    ui.pop_screen();
            }
        )));
        return;
    }

    if (key.wVirtualKeyCode == VK_OEM_PLUS)
        shower.max_words++;

    if (key.wVirtualKeyCode == VK_OEM_MINUS)
        shower.max_words--;

    auto typed_char = key.uChar.AsciiChar;
    if (typed_char >= 32 && typed_char <= 126)
        shower.typed_char(typed_char);
}
