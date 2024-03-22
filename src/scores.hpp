#pragma once

#include <string>
#include <vector>

class Score {
    // max name length is 255
    std::string name;
    uint32_t score;

  public:
    static Score create(std::string name, uint32_t score);

    std::string get_name() const;
    uint32_t get_score() const;
};

class Scores {
    // Max count is 255
    // Must be sorted by score (descending)
    std::vector<Score> scores;

  public:
    static Scores load(std::string filename);
    void save(std::string filename);

    bool is_high_score(uint32_t score, uint32_t count) const;

    std::vector<Score> get_scores(size_t count) const;
    std::vector<Score> get_scores() const;
    void add_score(const Score&);
};