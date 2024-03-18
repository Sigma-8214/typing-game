#include <math.h>

#include "consts.hpp"
#include "lab_color.hpp"

OkLab OkLab::create(float32_t L, float32_t a, float32_t b) {
    auto self = OkLab();
    self.L = L;
    self.a = a;
    self.b = b;
    return self;
}

// From https://bottosson.github.io/posts/oklab
OkLab OkLab::from_lsrgb(Rgb c) {
    float l =
        0.4122214708f * c.r + 0.5363325363f * c.g + 0.0514459929f * c.b;
    float m =
        0.2119034982f * c.r + 0.6806995451f * c.g + 0.1073969566f * c.b;
    float s =
        0.0883024619f * c.r + 0.2817188376f * c.g + 0.6299787005f * c.b;

    float l_ = cbrtf(l);
    float m_ = cbrtf(m);
    float s_ = cbrtf(s);

    return {
        0.2104542553f * l_ + 0.7936177850f * m_ - 0.0040720468f * s_,
        1.9779984951f * l_ - 2.4285922050f * m_ + 0.4505937099f * s_,
        0.0259040371f * l_ + 0.7827717662f * m_ - 0.8086757660f * s_,
    };
}

// Shift shoulb be an angle [0, 2pi)
OkLab OkLab::hue_shift(float32_t hue_shift) const {
    auto hue = atan2(this->b, this->a);
    auto chroma = sqrt(this->a * this->a + this->b * this->b);

    hue = fmod(hue + hue_shift, 2 * PI);

    auto a = chroma * cos(hue);
    auto b = chroma * sin(hue);

    return OkLab::create(this->L, a, b);
}

Rgb OkLab::to_rgb() const { return Rgb::from_oklab(*this); }

Color OkLab::to_color() const { return this->to_rgb().to_color(); }

Rgb Rgb::create(float32_t r, float32_t g, float32_t b) {
    auto self = Rgb();
    self.r = r;
    self.g = g;
    self.b = b;
    return self;
}

// From https://bottosson.github.io/posts/oklab
Rgb Rgb::from_oklab(OkLab c) {
    float l_ = c.L + 0.3963377774f * c.a + 0.2158037573f * c.b;
    float m_ = c.L - 0.1055613458f * c.a - 0.0638541728f * c.b;
    float s_ = c.L - 0.0894841775f * c.a - 1.2914855480f * c.b;

    float l = l_ * l_ * l_;
    float m = m_ * m_ * m_;
    float s = s_ * s_ * s_;

    return {
        +4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s,
        -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s,
        -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s,
    };
}

Rgb Rgb::from_color(Color color) {
    return Rgb::create(color.r / 255.0, color.g / 255.0, color.b / 255.0);
}

Color Rgb::to_color() const {
    return Color::create(this->r * 255, this->g * 255, this->b * 255);
}
