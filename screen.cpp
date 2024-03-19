#include "screen.hpp"
#include "drawing.hpp"

MenuScreen MenuScreen::create() {
    auto self = MenuScreen();
    return self;
}

void MenuScreen::render(State &state, Gui &gui) {
    auto center = gui.get_size();
    center = Point2i::create(center.x / 2, center.y / 3);

    if (balls.empty()) {
        for (auto i = 0; i < 50; i++)
            balls.push_back(Ball::create(center));
    }

    for (auto& ball: balls)
        ball.render(gui);
        
    title_hue += 0.25 * gui.get_delta_time();
    draw_rainbow_text(center - Point2i::create(10, 0), std::string("Generic Typing Game"), &title_hue, gui);

    gui.draw_text(center - Point2i::create(7, -2), std::string("[ENTER] Start"), Style::unstyled());
    gui.draw_text(center - Point2i::create(5, -3), std::string("[ESC] Quit"), Style::unstyled());

    gui.draw_text(gui.get_size() - Point2i::create(25, 1), "Created By: Connor Slade", Style::unstyled());
}

void MenuScreen::on_key(State &state, KEY_EVENT_RECORD key) {
    if (key.bKeyDown && key.wVirtualKeyCode == VK_ESCAPE)
        state.exit = true;
}
