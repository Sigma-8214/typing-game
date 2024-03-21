#pragma once

#include <string>

#include "particle_emitter.hpp"
#include "ui.hpp"

class NewHighScoreScreen : public Screen {
    std::string name;
    uint32_t score;

    ParticleEmitter confetti;
    float32_t name_hue = 0;
    bool init = true;

  public:
    static NewHighScoreScreen create(uint32_t score);

    void render(Ui &ui, Gui &gui);
    void on_key(Ui &ui, KEY_EVENT_RECORD key);
};