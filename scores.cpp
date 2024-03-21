#include <fstream>

#include "scores.hpp"

Score Score::create(std::string name, uint32_t score) {
    auto self = Score();
    self.name = name;
    self.score = score;
    return self;
}

std::string Score::get_name() const { return name; }

uint32_t Score::get_score() const { return score; }

Scores Scores::load(std::string filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    if (!file.is_open())
        return Scores();

    file.get();
    file.get();

    auto count = file.get();
    auto scores = Scores();
    for (auto i = 0; i < count; i++) {
        auto name_len = file.get();
        auto name = std::string();
        for (auto j = 0; j < name_len; j++)
            name.push_back(file.get());

        auto score = 0;
        for (auto j = 0; j < 4; j++)
            score = (score << 8) | file.get();

        scores.scores.push_back(Score::create(name, score));
    }
    file.close();

    return scores;
}

void Scores::save(std::string filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::out);

    file.put('T');
    file.put('G');

    file.put(scores.size());
    for (auto &score : scores) {
        file.put(score.get_name().size());
        for (auto &c : score.get_name())
            file.put(c);

        for (auto i = 3; i >= 0; i--)
            file.put((score.get_score() >> (8 * i)) & 0xFF);
    }
    file.close();
}

bool Scores::is_high_score(uint32_t score, uint32_t count) const {
    if (scores.size() < count)
        return true;

    for (auto i = 0; i < count; i++) {
        if (score > scores[i].get_score())
            return true;
    }

    return false;
}

std::vector<Score> Scores::get_scores(size_t count) const {
    auto result = std::vector<Score>();
    for (auto i = 0; i < count && i < scores.size(); i++)
        result.push_back(scores[i]);
    return result;
}

std::vector<Score> Scores::get_scores() const { return scores; }

void Scores::add_score(Score score) {
    auto found = false;
    for (auto i = 0; i < scores.size(); i++) {
        if (score.get_score() > scores[i].get_score()) {
            scores.insert(scores.begin() + i, score);
            found = true;
            break;
        }
    }

    if (!found)
        scores.push_back(score);

    if (scores.size() > 255)
        scores.pop_back();
}
