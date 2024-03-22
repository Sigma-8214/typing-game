#pragma once

#include "Windows.h"
#include <math.h>

#include "../tui_engine/tui.hpp"

#include "ui.hpp"

class Ui;

class Screen {
  public:
    virtual void render(Ui &ui, Gui &gui) = 0;
    virtual void on_key(Ui &ui, KEY_EVENT_RECORD key) = 0;
};

