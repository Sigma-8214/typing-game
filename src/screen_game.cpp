#include <iostream>

#include "consts.hpp"
#include "game.hpp"
#include "screen_confirm.hpp"
#include "screen_game.hpp"
#include "screen_new_high_score.hpp"

GameScreen GameScreen::create(Ui &ui) {
    auto self = GameScreen();
    self.shower = Game::create(ui.state.words);
    self.game_started = std::chrono::steady_clock::now();
    self.hue = 0;
    return self;
}

void GameScreen::render(Ui &ui, Gui &gui) {
    auto size = gui.get_size();

    if (shower.get_lives() == 0) {
        auto score = shower.get_typed();

        if (score > 0 &&
            ui.state.scores.is_high_score(score, HIGH_SCORES_COUNT))
            ui.swap_screen(std::make_unique<NewHighScoreScreen>(
                NewHighScoreScreen::create(123)
            ));
        else
            ui.push_screen(
                std::make_unique<ConfirmScreen>(ConfirmScreen::create(
                    "Nice try. You got " + std::to_string(score) + " words!",
                    {"Home"}, [](Ui &ui, uint8_t option) { ui.pop_screen(2); }
                ))
            );
        return;
    }

    shower.draw(gui);

    gui.draw_text(
        size - Point2i::create(11, 1), "[ESC] Quit", Style::unstyled()
    );

    auto delta = std::chrono::steady_clock::now() - game_started;
    auto seconds =
        std::chrono::duration_cast<std::chrono::seconds>(delta).count();

    auto words = std::to_string(shower.get_typed()) + " words";
    gui.draw_text(
        Point2i::create(size.x / 2 - words.size() / 2, 0), words,
        Style::unstyled()
    );

    auto time = std::to_string(seconds) + "s";
    gui.draw_text(
        Point2i::create(size.x / 2 - time.size() / 2, 1), time,
        Style::unstyled()
    );

    for (auto i = 0; i < shower.get_lives(); i++)
        gui.set(
            '*', Point2i::create(0, i),
            Style::unstyled().with_fg(Color::create(243, 70, 49))
        );
}

void GameScreen::on_key(Ui &ui, KEY_EVENT_RECORD key) {
    if (!key.bKeyDown)
        return;

    if (key.wVirtualKeyCode == VK_ESCAPE) {
        auto score = shower.get_typed();
        ui.push_screen(std::make_unique<ConfirmScreen>(ConfirmScreen::create(
            "Are you sure you want to exit?", {"No", "Yes"},
            [score](Ui &ui, uint8_t option) {
                if (option == 1) {
                    auto screen = NewHighScoreScreen::create(score);
                    ui.pop_screen(2);
                    if (score > 0 && ui.state.scores.is_high_score(
                                         score, HIGH_SCORES_COUNT
                                     )) {
                        ui.push_screen(
                            std::make_unique<NewHighScoreScreen>(screen)
                        );
                    }
                } else if (option == 0)
                    ui.pop_screen();
            }
        )));
        return;
    }

    auto typed_char = key.uChar.AsciiChar;
    if (typed_char >= 32 && typed_char <= 126)
        shower.typed_char(typed_char);
}
