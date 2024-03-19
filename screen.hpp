#pragma once

#include "Windows.h"
#include <string>
#include <math.h>

#include "../tui_engine/tui.hpp"

#include "state.hpp"
#include "misc.hpp"

class Screen {
  public:
    virtual void render(State &state, Gui &gui) = 0;
    virtual void on_key(State &state, KEY_EVENT_RECORD key) = 0;
};

struct Ball {
  Point2f pos;
  Point2f vel;

  static Ball create(Point2i pos) {
    auto self = Ball();
    self.pos = static_cast<Point2f>(pos);
    auto x_vel = 20 * static_cast<float32_t>(rand()) / RAND_MAX - 10;
    auto y_vel = 10 * static_cast<float32_t>(rand()) / RAND_MAX - 5;
    self.vel = Point2f::create(x_vel, y_vel);
    return self;
  }

  Ball clone() {
    auto self = Ball();
    self.pos = pos;
    self.vel = vel;
    return self;
  }

  void render(Gui& gui) {
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
};

class MenuScreen : public Screen {
  float32_t title_hue;
  std::vector<Ball> balls;

  public:
    static MenuScreen create();

    void render(State &state, Gui &gui);
    void on_key(State &state, KEY_EVENT_RECORD key);
};
