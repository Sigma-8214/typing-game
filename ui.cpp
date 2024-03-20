#include "Windows.h"

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
            this->screen->on_key(*this, event_buffer[i].Event.KeyEvent);
    }
}

Ui Ui::create(State state) {
    auto self = Ui();
    self.gui = Gui::create();
    self.running = true;
    self.state = state;
    self.screen = std::make_shared<MenuScreen>(MenuScreen::create());
    return self;
}

void Ui::run() {
    for (;;) {
        if (!running) {
            this->gui.cleanup();
            break;
        }

        this->handle_inputs();
        this->screen->render(*this, gui);

        // Will block to reach target framerate
        gui.update();
    }
}

void Ui::set_screen(std::shared_ptr<Screen> screen) {
    this->screen = std::move(screen);
}

std::shared_ptr<Screen> Ui::get_screen() { return this->screen; }

void Ui::exit() { this->running = false; }
