#pragma once


class ComponentScore : public Component {
private:
    int score;
    int highScore;

public:
    ComponentScore(Entity* entity, int initialScore = 0);

    void Logs() override;

    void SetScore(int newScore);
    int GetScore() const;

    void SetHighScore(int newHighScore);
    int GetHighScore() const;

    void AddPoints(int points);
    void ResetScore();
};