#pragma once

#include "ui.hpp"

class ScoresScreen : public Screen {
    float32_t top_score_hue = 0;

  public:
    static ScoresScreen create(Ui &ui);

    void render(Ui &ui, Gui &gui);
    void on_key(Ui &ui, KEY_EVENT_RECORD key);
};
