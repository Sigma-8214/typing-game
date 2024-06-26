#include "drawing.hpp"
#include "screen.hpp"

#include "screen_game.hpp"
#include "screen_menu.hpp"
#include "screen_scores.hpp"

MenuScreen MenuScreen::create() {
    auto self = MenuScreen();
    return self;
}

void MenuScreen::render(Ui &ui, Gui &gui) {
    auto size = gui.get_size();
    auto center = Point2i::create(size.x / 2, size.y / 3);

    if (balls.empty()) {
        for (auto i = 0; i < 50; i++)
            balls.push_back(Ball::create(size));
    }

    for (auto &ball : balls)
        ball.render(gui);

    title_hue += 0.25 * gui.get_delta_time();
    draw_rainbow_text(
        center - Point2i::create(10, 0), std::string("Generic Typing Game"),
        &title_hue, gui
    );

    const auto lines = {
        std::make_tuple(std::string("[ENTER] Start"), 6),
        std::make_tuple(std::string("[ESC] Quit"), 4),
        std::make_tuple(std::string("[S] Scores"), 2),
    };

    for (auto line = lines.begin(); line != lines.end(); line++) {
        auto line_num = line - lines.begin();
        gui.draw_text(
            center - Point2i::create(std::get<1>(*line), -line_num - 2),
            std::get<0>(*line), Style::unstyled()
        );
    }

    gui.draw_text(
        gui.get_size() - Point2i::create(24, 1), "Created By: Connor Slade",
        Style::unstyled()
    );
}

void MenuScreen::on_key(Ui &ui, KEY_EVENT_RECORD key) {
    if (!key.bKeyDown)
        return;

    // clang-format off
    if (key.wVirtualKeyCode == VK_ESCAPE)
        ui.exit();
    else if (key.wVirtualKeyCode == VK_RETURN)
        ui.push_screen(std::make_unique<GameScreen>(GameScreen::create(ui)));
    else if (key.wVirtualKeyCode == 'S')
        ui.push_screen(std::make_unique<ScoresScreen>(ScoresScreen::create(ui)));
    // clang-format on
}

MenuScreen::Ball MenuScreen::Ball::create(Point2i size) {
    auto self = Ball();
    self.pos =
        static_cast<Point2f>(Point2i::create(rand() % size.x, rand() % size.y));
    auto x_vel = 20 * static_cast<float32_t>(rand()) / RAND_MAX - 10;
    auto y_vel = 10 * static_cast<float32_t>(rand()) / RAND_MAX - 5;
    self.vel = Point2f::create(x_vel, y_vel);
    return self;
}

MenuScreen::Ball MenuScreen::Ball::clone() {
    auto self = Ball();
    self.pos = pos;
    self.vel = vel;
    return self;
}

void MenuScreen::Ball::render(Gui &gui) {
    pos.x += vel.x * gui.get_delta_time();
    pos.y += vel.y * gui.get_delta_time();

    auto int_pos = Point2i::create(lround(pos.x), lround(pos.y));
    auto size = gui.get_size();

    if (int_pos.x >= size.x || int_pos.x <= 0) {
        vel.x *= -1;
        pos.x = max(pos.x, 1.0f);
        pos.x = min(pos.x, static_cast<float32_t>(size.x) - 1);
    }

    if (int_pos.y >= size.y || int_pos.y <= 0) {
        vel.y *= -1;
        pos.y = max(pos.y, 1.0f);
        pos.y = min(pos.y, static_cast<float32_t>(size.y) - 1);
    }

    gui.set('*', int_pos, Style::unstyled());
}
