#pragma once

#include "ui.hpp"

class ScoresScreen : public Screen {
  public:
    static ScoresScreen create(Ui &ui);

    void render(Ui &ui, Gui &gui);
    void on_key(Ui &ui, KEY_EVENT_RECORD key);
};
