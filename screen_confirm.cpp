#include "screen_confirm.hpp"
#include "consts.hpp"
#include "drawing.hpp"

ConfirmScreen ConfirmScreen::create(
    std::string message, std::vector<std::string> options,
    std::function<void(Ui &ui, uint8_t)> handler
) {
    auto self = ConfirmScreen();
    self.message = message;
    self.options = options;
    self.handler = handler;
    self.option_width = options.size() - 1;
    for (auto &option : options)
        self.option_width += option.length();
    return self;
}

void ConfirmScreen::render(Ui &ui, Gui &gui) {
    auto size = gui.get_size();
    auto center = Point2i::create(size.x / 2, size.y / 3);

    gui.draw_text(
        center - Point2i::create(message.length() / 2, 0), message,
        Style::unstyled()
    );

    message_hue += 0.25 * gui.get_delta_time();
    auto color = COLOR_START.hue_shift(message_hue * 2 * PI).to_color();

    auto x = -static_cast<int32_t>(option_width) / 2;

    for (auto i = 0; i < options.size(); i++) {
        auto option = options[i];

        auto style = Style::unstyled();
        if (i == selected_option)
            style = style.with_bg(color);
        gui.draw_text(center + Point2i::create(x, 2), option, style);
        x += option.length() + 1;
    }
}

void ConfirmScreen::on_key(Ui &ui, KEY_EVENT_RECORD key) {
    if (!key.bKeyDown)
        return;

    if (key.wVirtualKeyCode == VK_ESCAPE) {
        handler(ui, 0);
        return;
    }

    if (key.wVirtualKeyCode == VK_RETURN) {
        handler(ui, selected_option);
        return;
    }

    if (key.wVirtualKeyCode == VK_UP || key.wVirtualKeyCode == VK_LEFT)
        selected_option = (selected_option - 1) % options.size();

    if (key.wVirtualKeyCode == VK_DOWN || key.wVirtualKeyCode == VK_RIGHT)
        selected_option = (selected_option + 1) % options.size();
}
