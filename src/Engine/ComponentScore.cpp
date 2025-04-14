#include "pch.h"
#include "ComponentScore.h"

ComponentScore::ComponentScore(Entity* entity, int initialScore)
    : Component(entity), score(initialScore), highScore(initialScore)
{
}

void ComponentScore::Logs() {
    HEAD("sys_SCORE") << "Entite " << GetEntity()->GetID() << " : "
        << "Score actuel: " << score << " | Meilleur score: " << highScore << ENDL;
}

void ComponentScore::SetScore(int newScore)
{
    score = newScore;
    if (score > highScore) {
        highScore = score;
    }
}

int ComponentScore::GetScore() const
{
    return score;
}

void ComponentScore::SetHighScore(int newHighScore)
{
    highScore = newHighScore;
}

int ComponentScore::GetHighScore() const
{
    return highScore;
}

void ComponentScore::AddPoints(int points) {
    score += points;
    if (score > highScore) {
        highScore = score;
    }
}

void ComponentScore::ResetScore() {
    score = 0;
}