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
    std::vector<std::unique_ptr<Screen>> screens;

    void handle_inputs();

  public:
    State state;

    static Ui create(State state);
    void run();

    void push_screen(std::unique_ptr<Screen> screen);
    std::unique_ptr<Screen> pop_screen();
    std::unique_ptr<Screen> swap_screen(std::unique_ptr<Screen> screen);
    void exit();
};
