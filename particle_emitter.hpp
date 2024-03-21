#pragma once

#include <vector>

#include "../tui_engine/tui.hpp"
#include "drawing.hpp"

class ParticleConfig {
    float32_t gravity;
    float32_t lifetime;
    std::vector<char> characters;

  public:
    static ParticleConfig
    create(float32_t gravity, float32_t lifetime, std::vector<char> characters);

    float32_t get_gravity();
    float32_t get_lifetime();
    char get_character();
};

class Particle {
    Point2f position;
    Point2f velocity;
    float32_t phase;
    char character;
    std::chrono::time_point<std::chrono::steady_clock> birth;

  public:
    static Particle create(
        ParticleConfig &config, Point2f position, Point2f velocity,
        std::chrono::time_point<std::chrono::steady_clock> birth
    );

    bool is_alive(ParticleConfig &config);

    void render(ParticleConfig &config, Gui &gui, float32_t hue);
};

class ParticleEmitter {
    ParticleConfig config;
    std::vector<Particle> particles;
    float32_t hue;

  public:
    static ParticleEmitter create(ParticleConfig config);

    void emit(Point2f position, Point2f velocity);
    void render(Gui &gui);
};
