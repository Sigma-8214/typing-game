#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <thread>
#include <windows.h>

#include "tui.hpp"

using time_point = std::chrono::system_clock::time_point;
using float32_t = float;

Point2i Point2i::origin() { return Point2i::create(0, 0); }

Point2i Point2i::create(int32_t x, int32_t y) {
    auto self = Point2i();
    self.x = x;
    self.y = y;
    return self;
}

Point2i::operator Point2f() { return Point2f::create(x, y); }

Point2i Point2i::operator+(Point2i other) {
    return Point2i::create(x + other.x, y + other.y);
}

Point2i Point2i::operator-(Point2i other) {
    return Point2i::create(x - other.x, y - other.y);
}

Point2f Point2f::origin() { return Point2f::create(0, 0); }

Point2f Point2f::create(float32_t x, float32_t y) {
    auto self = Point2f();
    self.x = x;
    self.y = y;
    return self;
}

Point2f::operator Point2i() { return Point2i::create(roundf(x), roundf(y)); }

Point2f Point2f::operator+(Point2f other) {
    return Point2f::create(x + other.x, y + other.y);
}

Point2f Point2f::operator-(Point2f other) {
    return Point2f::create(x - other.x, y - other.y);
}

Point2i get_screen_size() {
    CONSOLE_SCREEN_BUFFER_INFO buffer_info;
    uint16_t columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buffer_info);
    columns = buffer_info.srWindow.Right - buffer_info.srWindow.Left + 1;
    rows = buffer_info.srWindow.Bottom - buffer_info.srWindow.Top + 1;

    return Point2i::create(columns, rows);
}

Color Color::create(uint8_t r, uint8_t g, uint8_t b) {
    auto self = Color();
    self.r = r;
    self.g = g;
    self.b = b;
    return self;
}

Color Color::from_hsv(
    float32_t hew, float32_t saturation, float32_t lightness
) {
    auto i = std::floor(hew * 6);
    auto f = hew * 6 - i;
    auto p = lightness * (1 - saturation);
    auto q = lightness * (1 - f * saturation);
    auto t = lightness * (1 - (1 - f) * saturation);

    switch (static_cast<int>(i) % 6) {
    case 0:
        return Color::create(lightness * 255, t * 255, p * 255);
    case 1:
        return Color::create(q * 255, lightness * 255, p * 255);
    case 2:
        return Color::create(p * 255, lightness * 255, t * 255);
    case 3:
        return Color::create(p * 255, q * 255, lightness * 255);
    case 4:
        return Color::create(t * 255, p * 255, lightness * 255);
    case 5:
        return Color::create(lightness * 255, p * 255, q * 255);
    default:
        // Will never happen, but complier is complaining...
        return Color();
    }
}

Style Style::unstyled() {
    auto self = Style();
    self.fg = Color::create(255, 255, 255);
    self.bg = Color::create(0, 0, 0);
    self.underline = false;
    self.bold = false;
    return self;
}

Style Style::with_fg(Color fg) {
    auto self = Style();
    self.fg = fg;
    self.bg = bg;
    self.underline = underline;
    self.bold = bold;
    return self;
}

Style Style::with_bg(Color bg) {
    auto self = Style();
    self.fg = fg;
    self.bg = bg;
    self.underline = underline;
    self.bold = bold;
    return self;
}

Style Style::with_underline(bool underline) {
    auto self = Style();
    self.fg = fg;
    self.bg = bg;
    self.underline = underline;
    self.bold = bold;
    return self;
}

Style Style::with_underline() { return with_underline(true); }

Style Style::with_bold(bool bold) {
    auto self = Style();
    self.fg = fg;
    self.bg = bg;
    self.underline = underline;
    self.bold = bold;
    return self;
}

Style Style::with_bold() { return with_bold(true); }

bool Style::eq(Style other) {
    return fg.r == other.fg.r && fg.g == other.fg.g && fg.b == other.fg.b &&
           bg.r == other.bg.r && bg.g == other.bg.g && bg.b == other.bg.b &&
           underline == other.underline && bold == other.bold;
}

size_t Gui::to_index(Point2i position) {
    return position.y * width + position.x;
}

void Gui::refresh_screen() {
    auto screen_size = get_screen_size();

    if (screen_size.x != width || screen_size.y != height) {
        width = screen_size.x;
        height = screen_size.y;

        delete[] style;
        delete[] screen;

        style = new Style[width * height];
        screen = new char[width * height];
    }

    for (auto y = 0; y < height; y++) {
        for (auto x = 0; x < width; x++) {
            auto index = to_index(Point2i::create(x, y));
            screen[index] = 0;
            style[index] = Style::unstyled();
        }
    }
}

Gui Gui::create() {
    auto self = Gui();
    std::cout << "\x1b[?25l";
    self.refresh_screen();
    return self;
}

void Gui::cleanup() { std::cout << "\x1b[?25h"; }

Gui Gui::with_target_frame_rate(uint16_t frame_rate) {
    this->frame_rate = frame_rate;
    if (frame_rate == -1)
        delta_time = 1.0f;
    else
        delta_time = 1.0f / frame_rate;
    return *this;
}

void Gui::set(char character, Point2i position, Style style) {
    auto index = to_index(position);
    if (index < 0 || index >= width * height)
        return;

    screen[index] = character;
    this->style[index] = style;
}

void Gui::draw_text(Point2i position, const std::string text, Style style) {
    size_t idx = 0;
    auto chr = text.begin();

    for (; chr != text.end(); idx++, chr++) {
        auto x = position.x + idx;
        auto y = position.y;

        if (x < 0 || x >= width || y < 0 || y >= height)
            continue;

        set(*chr, Point2i::create(x, y), style);
    }
}

// Modified From: Circle drawing algorithm in C
// https://dev.to/sjmulder/circle-drawing-algorithm-in-c-4057
void Gui::draw_circle(Point2i center, int32_t radius, char chr, Style style) {
    int32_t x, y;

    for (x = 0, y = radius; x < y; x++)
        for (; y >= 0; y--) {
            set(chr, Point2i::create(center.x + x * 2, center.y + y), style);
            set(chr, Point2i::create(center.x + x * 2, center.y - y), style);
            set(chr, Point2i::create(center.x - x * 2, center.y + y), style);
            set(chr, Point2i::create(center.x - x * 2, center.y - y), style);

            set(chr, Point2i::create(center.x + y * 2, center.y + x), style);
            set(chr, Point2i::create(center.x + y * 2, center.y - x), style);
            set(chr, Point2i::create(center.x - y * 2, center.y + x), style);
            set(chr, Point2i::create(center.x - y * 2, center.y - x), style);

            if (x * x + (y - 1) * (y - 1) < radius * radius)
                break;
        }
}

float32_t Gui::get_delta_time() { return delta_time; }

uint16_t Gui::get_width() { return width; }

uint16_t Gui::get_height() { return height; }

Point2i Gui::get_size() { return Point2i::create(width, height); }

void Gui::update() {
    auto now = std::chrono::system_clock::now();
    if (last_update.time_since_epoch().count() != 0)
        delta_time =
            std::chrono::duration<float32_t>(now - last_update).count();

    auto next_update =
        last_update + std::chrono::milliseconds(1000 / frame_rate);

    if (frame_rate != -1)
        std::this_thread::sleep_until(next_update);
    last_update = now;

    auto out = std::string();
    auto underline = false;
    auto bold = false;
    auto last_style = Style::unstyled();

    for (auto y = 0; y < height; y++) {
        for (auto x = 0; x < width; x++) {
            auto index = to_index(Point2i::create(x, y));
            auto style = this->style[index];

            if (!style.eq(last_style)) {
                last_style = style;

                out += "\x1b[38;2;";
                out += std::to_string(style.fg.r) + ";";
                out += std::to_string(style.fg.g) + ";";
                out += std::to_string(style.fg.b) + "m";

                out += "\x1b[48;2;";
                out += std::to_string(style.bg.r) + ";";
                out += std::to_string(style.bg.g) + ";";
                out += std::to_string(style.bg.b) + "m";

                if (style.underline != underline) {
                    underline = style.underline;
                    out += underline ? "\x1b[4m" : "\x1b[24m";
                }

                if (style.bold != bold) {
                    bold = style.bold;
                    out += bold ? "\x1b[1m" : "\x1b[22m";
                }
            }

            if (screen[index] == 0)
                out += " ";
            else
                out += screen[index];
        }
        out += "\n";
    }

    out.pop_back();
    std::cout << "\x1b[H" << out << std::flush;

    refresh_screen();
}
