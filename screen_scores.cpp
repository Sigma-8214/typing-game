#include "screen_scores.hpp"

ScoresScreen ScoresScreen::create(Ui &ui) { return ScoresScreen(); }

void ScoresScreen::render(Ui &ui, Gui &gui) {
    gui.draw_text(Point2i::origin(), "Scores Screen", Style::unstyled());
}

void ScoresScreen::on_key(Ui &ui, KEY_EVENT_RECORD key) {}