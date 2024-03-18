#include "screen.hpp"

MenuScreen MenuScreen::create() { return MenuScreen(); }

void MenuScreen::render(State &state, Gui &gui) {
    gui.draw_text(
        Point2i::origin(), std::string("Menu Screen"), Style::unstyled()
    );
}

void MenuScreen::on_key(State &state, KEY_EVENT_RECORD key) {
    if (key.bKeyDown && key.wVirtualKeyCode == VK_ESCAPE)
        state.exit = true;
}
