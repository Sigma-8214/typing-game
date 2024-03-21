#include "screen_scores.hpp"
#include "consts.hpp"
#include "drawing.hpp"

ScoresScreen ScoresScreen::create(Ui &ui) { return ScoresScreen(); }

void ScoresScreen::render(Ui &ui, Gui &gui) {
    auto size = gui.get_size();
    auto center = Point2i::create(size.x / 2, size.y / 3);

    const auto title = std::string("High Scores!");
    gui.draw_text(
        center - Point2i::create(title.length() / 2, 0), title,
        Style::unstyled()
    );

    const auto message = std::string("Press [Esc] to go back");
    gui.draw_text(
        size - Point2i::create(message.length(), 1), message, Style::unstyled()
    );

    auto scores = ui.state.scores;
    auto top_scores = scores.get_scores(HIGH_SCORES_COUNT);

    if (top_scores.empty()) {
        const auto message = std::string("No scores yet");
        gui.draw_text(
            center + Point2i::create(-message.length() / 2, 2), message,
            Style::unstyled()
        );
        return;
    }

    for (auto i = 0; i < top_scores.size(); i++) {
        auto score = top_scores[i];
        auto text =
            score.get_name() + " - " + std::to_string(score.get_score());
        auto pos = center + Point2i::create(-text.length() / 2, i + 2);

        if (i == 0)
            draw_rainbow_text(pos, text, &top_score_hue, gui);
        else
            gui.draw_text(pos, text, Style::unstyled());
    }
}

void ScoresScreen::on_key(Ui &ui, KEY_EVENT_RECORD key) {
    if (key.bKeyDown && key.wVirtualKeyCode == VK_ESCAPE)
        ui.pop_screen();
}