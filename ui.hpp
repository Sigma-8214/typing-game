#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../tui_engine/tui.hpp"

#include "screen.hpp"
#include "state.hpp"

class Ui {
    Gui gui;
    State state;
    std::unique_ptr<Screen> screen;

    void handle_inputs();

  public:
    static Ui create(State state);
    void run();
};
