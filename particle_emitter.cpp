#include <math.h>
#include <vector>

#include "../tui_engine/tui.hpp"
#include "drawing.hpp"
#include "particle_emitter.hpp"

ParticleConfig ParticleConfig::create(
    float32_t gravity, float32_t lifetime, std::vector<char> characters
) {
    auto self = ParticleConfig();
    self.gravity = gravity;
    self.lifetime = lifetime;
    self.characters = characters;
    return self;
}

float32_t ParticleConfig::get_gravity() { return gravity; }
float32_t ParticleConfig::get_lifetime() { return lifetime; }
char ParticleConfig::get_character() {
    return characters[rand() % characters.size()];
}

Particle Particle::create(
    ParticleConfig &config, Point2f position, Point2f velocity,
    std::chrono::time_point<std::chrono::steady_clock> birth
) {
    auto self = Particle();
    self.position = position;
    self.velocity = velocity;
    self.phase = static_cast<float32_t>(rand()) / RAND_MAX;
    self.birth = birth;
    self.character = config.get_character();
    return self;
}

bool Particle::is_alive(ParticleConfig &config) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - birth)
            .count();
    return elapsed < config.get_lifetime() * 1000;
}

void Particle::render(ParticleConfig &config, Gui &gui, float32_t hue) {
    position.y += velocity.y * gui.get_delta_time();
    position.x += velocity.x * gui.get_delta_time();
    velocity.y += config.get_gravity() * gui.get_delta_time();

    auto pos = Point2i::create(round(position.x), round(position.y));
    auto color = rainbow_color(hue, phase);
    gui.set(character, pos, Style::unstyled().with_fg(color));
}

ParticleEmitter ParticleEmitter::create(ParticleConfig config) {
    auto self = ParticleEmitter();
    self.config = config;
    return self;
}

void ParticleEmitter::emit(Point2f position, Point2f velocity) {
    auto birth = std::chrono::steady_clock::now() +
                 std::chrono::milliseconds(rand() % 500 - 250);
    auto particle = Particle::create(config, position, velocity, birth);
    particles.push_back(particle);
}

void ParticleEmitter::render(Gui &gui) {
    hue += 0.25 * gui.get_delta_time();

    for (auto i = 0; i < particles.size(); i++) {
        auto &particle = particles[i];

        if (particle.is_alive(config)) {
            particle.render(config, gui, hue);
            continue;
        }

        particles.erase(particles.begin() + i--);
    }
}
