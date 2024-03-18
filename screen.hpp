#pragma once

#include "Windows.h"
#include <string>

#include "../tui_engine/tui.hpp"

#include "state.hpp"

class Screen {
  public:
    virtual void render(State &state, Gui &gui) = 0;
    virtual void on_key(State &state, KEY_EVENT_RECORD key) = 0;
};

class MenuScreen : public Screen {
  public:
    static MenuScreen create();

    void render(State &state, Gui &gui);
    void on_key(State &state, KEY_EVENT_RECORD key);
};
