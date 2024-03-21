#pragma once

#include "ui.hpp"
#include "word_shower.hpp"

class GameScreen : public Screen {
    WordShower shower;
    std::chrono::time_point<std::chrono::steady_clock> game_started;
    float32_t hue;

  public:
    static GameScreen create(Ui &ui);

    void render(Ui &ui, Gui &gui);
    void on_key(Ui &ui, KEY_EVENT_RECORD key);
};
