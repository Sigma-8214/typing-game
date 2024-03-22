#include "screen_new_high_score.hpp"
#include "consts.hpp"
#include "drawing.hpp"
#include "particle_emitter.hpp"
#include "screen_confirm.hpp"
#include "screen_scores.hpp"

const auto CONFETTI = ParticleConfig::create(8, 3, {'*', '~', '.', '-'});

NewHighScoreScreen NewHighScoreScreen::create(uint32_t score) {
    auto self = NewHighScoreScreen();
    self.score = score;
    self.confetti = ParticleEmitter::create(CONFETTI);
    return self;
}

void NewHighScoreScreen::render(Ui &ui, Gui &gui) {
    auto size = gui.get_size();
    auto center = Point2i::create(size.x / 2, size.y / 3);

    confetti.render(gui);

    const auto title = std::string("New High Score!");
    gui.draw_text(
        center - Point2i::create(title.length() / 2, 0), title,
        Style::unstyled()
    );

    if (init) {
        init = false;

        auto offset = Point2i::create(title.length() / 2, 0);
        auto title_right = center + offset;
        auto title_left = center - offset;

        for (auto i = 0; i < 20; i++) {
            auto velocity = Point2f::create(
                5 * static_cast<float32_t>(rand()) / RAND_MAX + 10,
                5 * static_cast<float32_t>(rand()) / RAND_MAX - 10
            );

            confetti.emit(static_cast<Point2f>(title_right), velocity);

            velocity.x *= -1;
            confetti.emit(static_cast<Point2f>(title_left), velocity);
        }
    }

    const auto message = std::string("Enter your name then press [Enter]");
    gui.draw_text(
        center + Point2i::create(-message.length() / 2, 2), message,
        Style::unstyled()
    );

    draw_rainbow_text(
        center + Point2i::create(-name.length() / 2, 3 + 1), name, &name_hue,
        gui
    );
}

void NewHighScoreScreen::on_key(Ui &ui, KEY_EVENT_RECORD key) {
    if (!key.bKeyDown)
        return;

    if (key.wVirtualKeyCode == VK_ESCAPE) {
        ui.push_screen(std::make_unique<ConfirmScreen>(ConfirmScreen::create(
            "You really don't want to save your score?", {"Back", "Exit"},
            [](Ui &ui, uint8_t option) {
                if (option == 0)
                    ui.pop_screen();
                else if (option == 1)
                    ui.pop_screen(2);
            }
        )));
        return;
    }

    if (key.wVirtualKeyCode == VK_RETURN) {
        ui.state.scores.add_score(Score::create(name, score));
        ui.swap_screen(std::make_unique<ScoresScreen>());
    }

    if (key.wVirtualKeyCode == VK_BACK && name.length() > 0)
        name.pop_back();

    if (key.uChar.UnicodeChar >= 32 && key.uChar.UnicodeChar <= 126 &&
        name.length() < 255)
        name += key.uChar.UnicodeChar;
}