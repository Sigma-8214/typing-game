#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../tui_engine/tui.hpp"

#include "screen.hpp"
#include "state.hpp"

class Screen;

class Ui {
    Gui gui;
    bool running;
    std::shared_ptr<Screen> screen;

    void handle_inputs();

  public:
    State state;

    static Ui create(State state);
    void run();

    void set_screen(std::shared_ptr<Screen> screen);
    std::shared_ptr<Screen> get_screen();
    void exit();
};
