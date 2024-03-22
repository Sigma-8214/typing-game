#pragma once

#include "../tui_engine/tui.hpp"

class Rgb;

class OkLab {
  public:
    float32_t L;
    float32_t a;
    float32_t b;

    static OkLab create(float32_t L, float32_t a, float32_t b);
    static OkLab from_lsrgb(Rgb c);

    OkLab hue_shift(float32_t hue_shift) const;

    Rgb to_rgb() const;
    Color to_color() const;
};

class Rgb {
  public:
    float32_t r;
    float32_t g;
    float32_t b;

    static Rgb create(float32_t r, float32_t g, float32_t b);
    static Rgb from_oklab(OkLab c);
    static Rgb from_color(Color color);

    Color to_color() const;
};
