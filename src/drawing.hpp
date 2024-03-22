#pragma once

#include "../tui_engine/tui.hpp"

void draw_rainbow_text(Point2i pos, std::string text, float32_t *hue, Gui &gui);
Color rainbow_color(float32_t hue, float32_t percent);