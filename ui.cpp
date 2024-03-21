#include "Windows.h"
#include <iostream>

#include "screen_menu.hpp"
#include "ui.hpp"

void Ui::handle_inputs() {
    auto events = new DWORD();
    GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), events);

    auto event_buffer = new INPUT_RECORD[*events];
    ReadConsoleInput(
        GetStdHandle(STD_INPUT_HANDLE), event_buffer, *events, events
    );

    for (auto i = 0; i < *events; i++) {
        if (event_buffer[i].EventType == KEY_EVENT)
            this->screens.back()->on_key(*this, event_buffer[i].Event.KeyEvent);
    }
}

Ui Ui::create(State state) {
    auto self = Ui();
    self.gui = Gui::create();
    self.running = true;
    self.state = state;
    self.screens.push_back(std::make_unique<MenuScreen>(MenuScreen::create()));
    return self;
}

void Ui::run() {
    std::cout << "\x1b\[?1049h";

    for (;;) {
        if (!running) {
            this->gui.cleanup();
            std::cout << "\x1b\[?1049l";
            break;
        }

        this->handle_inputs();
        this->screens.back()->render(*this, gui);

        // Will block to reach target framerate
        gui.update();
    }
}

void Ui::push_screen(std::unique_ptr<Screen> screen) {
    this->screens.push_back(std::move(screen));
}

std::unique_ptr<Screen> Ui::pop_screen() {
    auto old = std::move(this->screens.back());
    this->screens.pop_back();
    return old;
}

std::unique_ptr<Screen> Ui::swap_screen(std::unique_ptr<Screen> screen) {
    auto old = std::move(this->screens.back());
    this->screens.pop_back();
    this->screens.push_back(std::move(screen));
    return old;
}

void Ui::exit() { this->running = false; }
