#pragma once

#include <math.h>

#include "ui.hpp"

class MenuScreen : public Screen {
    struct Ball {
        Point2f pos;
        Point2f vel;

        static Ball create(Point2i pos);

        Ball clone();
        void render(Gui &gui);
    };

    float32_t title_hue;
    std::vector<Ball> balls;

  public:
    static MenuScreen create();

    void render(Ui &ui, Gui &gui);
    void on_key(Ui &ui, KEY_EVENT_RECORD key);
};
