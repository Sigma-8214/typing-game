#pragma once

#include <math.h>
#include <string>

#include "../tui_engine/tui.hpp"

class FallingWord {
    std::string word;
    Point2f spawn_position;
    Point2f position;

    float32_t speed;
    float32_t phase;
    float32_t hue;

    size_t typed;

  public:
    static FallingWord create(std::string word, Point2f position);

    bool is_complete() const;
    bool is_out_of_range(Gui &gui) const;

    void type(char character);
    void render(Gui &gui);
};
