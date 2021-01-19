#include "Game.h"

Game::Game(): m_Window(sf::VideoMode(640, 360), "Fight Pit")
, m_Player(0.0f, 330.0f, std::vector<AnimationInfo> {AnimationInfo{0, 0, 15, 15, 3, 0.5f}, AnimationInfo{0, 15, 15, 15, 1, 0.5f},
           AnimationInfo{0, 30, 15, 15, 1, 0.5f}, AnimationInfo{0, 45, 15, 15, 1, 0.5f}, AnimationInfo{0, 60, 15, 15, 1, 0.5f},
           AnimationInfo{0, 75, 15, 15, 1, 0.5f}})
, m_Knight(500.0f, 315.0f, std::vector<AnimationInfo> {AnimationInfo{0, 0, 51, 45, 4, 0.3f}, AnimationInfo{0, 45, 73, 66, 7, 0.1f},
           AnimationInfo{0, 111, 61, 45, 2, 0.1f}, AnimationInfo{146, 45, 73, 66, 1, 1.0f}}, m_Player)
{
    m_PlayerTexture.loadFromFile("assets\\slime.png");
    m_KnightTexture.loadFromFile("assets\\knight.png");
    m_Player.m_Sprite.setTexture(m_PlayerTexture);
    m_Knight.m_Sprite.setTexture(m_KnightTexture);
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
            processInput(dt);
            update(dt);
            checkCollision();
            draw();
        } else {
            sf::sleep(sf::seconds(1.0f/60.0f - m_Clock.getElapsedTime().asSeconds()));
        }
    }
}

void Game::processInput(const float &dt)
{
    sf::Event event;
    while (m_Window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_Window.close();
    }
    if (sf::Joystick::isConnected(0))
    {
        m_Player.processInput(dt);
    }
}

void Game::update(const float &dt)
{
    m_Player.update(dt);
    m_Knight.update(dt);
}

void Game::checkCollision() {
    if (m_Player.m_InvTime <= 0.0f) {
        if (m_Player.m_Hitbox.intersects(m_Knight.m_Hitbox)) {
            m_Player.handleEnemyCollision(m_Knight.m_Hitbox);
        }
    }
}

void Game::draw()
{
    m_Window.clear();

    m_Window.draw(m_Player.m_Sprite);
    m_Window.draw(m_Knight.m_Sprite);

    sf::RectangleShape hitbox = sf::RectangleShape();
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(3.0f);
    hitbox.setSize(sf::Vector2f(m_Knight.m_Hitbox.width, m_Knight.m_Hitbox.height));
    hitbox.setPosition(m_Knight.m_Hitbox.left, m_Knight.m_Hitbox.top);
    m_Window.draw(hitbox);
    hitbox.setSize(sf::Vector2f(m_Player.m_Hitbox.width, m_Player.m_Hitbox.height));
    hitbox.setPosition(m_Player.m_Hitbox.left, m_Player.m_Hitbox.top);
    m_Window.draw(hitbox);

    m_Window.display();
}
