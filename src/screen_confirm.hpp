#pragma once

#include <functional>

#include "ui.hpp"

class ConfirmScreen : public Screen {
    std::string message;
    std::vector<std::string> options;
    uint32_t option_width;

    uint8_t selected_option = 0;
    float32_t message_hue = 0;
    std::function<void(Ui &ui, uint8_t)> handler;

  public:
    static ConfirmScreen create(
        std::string message, std::vector<std::string> options,
        std::function<void(Ui &ui, uint8_t)> handler
    );

    void render(Ui &ui, Gui &gui);
    void on_key(Ui &ui, KEY_EVENT_RECORD key);
};
