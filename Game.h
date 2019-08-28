#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Enemy.h"

class Game
{
private:
    sf::RenderWindow m_Window;
    sf::Texture m_PlayerTexture;
    sf::Texture m_KnightTexture;
    sf::Clock m_Clock;
    Player m_Player;
    Knight m_Knight;
    void processInput(const float &dt);
    void update(const float &dt);
    void draw();
public:
    Game();
    void gameLoop();
};

#endif // GAME_H_INCLUDED
