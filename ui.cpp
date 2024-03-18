#include "Windows.h"

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
            this->screen->on_key(state, event_buffer[i].Event.KeyEvent);
    }
}

Ui Ui::create(State state) {
    auto self = Ui();
    self.gui = Gui::create();
    self.state = state;
    self.screen = std::make_unique<MenuScreen>(MenuScreen::create());
    return self;
}

void Ui::run() {
    for (;;) {
        this->handle_inputs();
        this->screen->render(state, gui);

        // Will block to reach target framerate
        gui.update();
    }
}