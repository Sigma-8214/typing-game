#include "drawing.hpp"
#include "consts.hpp"

void draw_rainbow_text(
    Point2i pos, std::string text, float32_t *hue, Gui &gui
) {
    *hue += 0.25f * gui.get_delta_time();
    for (auto i = 0; i < text.size(); i++) {
        auto local_hue = *hue - (static_cast<float32_t>(i) / text.size() / 2);
        if (local_hue < 0)
            local_hue += 1;

        auto color = COLOR_START.hue_shift(local_hue * 2 * PI).to_color();

        gui.set(
            text[i], pos + Point2i::create(i, 0),
            Style::unstyled().with_fg(color)
        );
    }
}
