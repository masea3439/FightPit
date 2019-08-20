#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Game
{
private:
    sf::RenderWindow m_Window;
    sf::Texture m_PlayerTexture;
    sf::Clock m_Clock;
    Player m_Player;
    void processInput();
    void update(float dt);
    void draw();
public:
    Game();
    void gameLoop();
};

#endif // GAME_H_INCLUDED
