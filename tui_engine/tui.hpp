#pragma once

#include <chrono>
#include <string>

using time_point = std::chrono::system_clock::time_point;
using float32_t = float;

class Point2f;

class Point2i {
  public:
    int32_t x;
    int32_t y;

    static Point2i origin();
    static Point2i create(int32_t x, int32_t y);

    explicit operator Point2f();
    Point2i operator+(Point2i other);
    Point2i operator-(Point2i other);
};

class Point2f {
  public:
    float32_t x;
    float32_t y;

    static Point2f origin();
    static Point2f create(float32_t x, float32_t y);

    explicit operator Point2i();
    Point2f operator+(Point2f other);
    Point2f operator-(Point2f other);
};

Point2i get_screen_size();

class Color {
  public:
    uint8_t r;
    uint8_t g;
    uint8_t b;

    static Color create(uint8_t r, uint8_t g, uint8_t b);
    static Color
    from_hsv(float32_t hew, float32_t saturation, float32_t lightness);
};

class Style {
  public:
    Color fg;
    Color bg;

    bool underline;
    bool bold;

    static Style unstyled();

    Style with_fg(Color fg);
    Style with_bg(Color bg);

    Style with_underline(bool underline);
    Style with_underline();
    Style with_bold(bool bold);
    Style with_bold();

    bool eq(Style other);
};

class Gui {
    uint16_t width;
    uint16_t height;

    Style *style;
    char *screen;

    time_point last_update;

    uint16_t frame_rate = 30;
    float32_t delta_time = 1.0f / frame_rate;

    size_t to_index(Point2i position);
    void refresh_screen();

  public:
    static Gui create();
    void cleanup();

    Gui with_target_frame_rate(uint16_t frame_rate);

    float32_t get_delta_time();
    uint16_t get_width();
    uint16_t get_height();
    Point2i get_size();

    void set(char character, Point2i position, Style style);
    void draw_text(Point2i position, std::string text, Style style);
    void draw_circle(Point2i center, int32_t radius, char chr, Style style);

    void update();
};
