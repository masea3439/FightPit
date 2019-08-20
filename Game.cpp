#include "Game.h"

Game::Game(): m_Window(sf::VideoMode(640, 360), "Fight Pit")
, m_Player(0.0f, 345.0f, 15, 15, std::vector<int> {3, 1, 1, 1, 1, 1})
{
    m_PlayerTexture.loadFromFile("assets\\slime.png");
    m_Player.m_Sprite.setTexture(m_PlayerTexture);
}

void Game::gameLoop()
{
    m_Clock.restart();
    float dt;

    while (m_Window.isOpen())
    {
        if (m_Clock.getElapsedTime().asSeconds() >= 1.0f/60.0f)
        {
            dt = m_Clock.restart().asSeconds();
            processInput();
            update(dt);
            draw();
        } else {
            sf::sleep(sf::seconds(1.0f/60.0f - m_Clock.getElapsedTime().asSeconds()));
        }
    }
}

void Game::processInput()
{
    sf::Event event;
    while (m_Window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_Window.close();
    }
    if (sf::Joystick::isConnected(0))
    {
        m_Player.processInput();
    }
}

void Game::update(float dt)
{
    m_Player.update(dt);
}

void Game::draw()
{
    m_Window.clear();


    m_Window.draw(m_Player.m_Sprite);
    m_Window.display();
}
